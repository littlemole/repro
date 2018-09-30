#ifndef DEFINE_MOL_HTTP_SERVER_PROMISE_RESUMABLE_DEF_GUARD_DEFINE_
#define DEFINE_MOL_HTTP_SERVER_PROMISE_RESUMABLE_DEF_GUARD_DEFINE_

#include <future>
#ifdef _WIN32
#include <experimental/resumable>
#else
#include "reprocpp/coroutine.h"
#endif

namespace repro {


// awaitable optional type that holds either a value or an exception
// once it is ready

template<class T>
class ValueOrException
{
public:

	void set(T t)
	{
		::new ((T*)&t_) T(std::move(t));
		is_ready_ = true;
	}

	void set(std::exception_ptr e)
	{
		e_ = std::move(e);
		is_ready_ = true;
	}

	T get()
	{
		if (e_)
		{
			std::rethrow_exception(e_);
		}
		T t(std::move(*(T*)&t_));
		((T*)&t_)->~T();
		return std::move(t);
	}

	bool is_ready()
	{
		return is_ready_;
	}

private:
	bool is_ready_ = false;
	typename std::aligned_storage<sizeof(T), alignof(T)>::type t_;
	std::exception_ptr e_;
};


template<class T>
class ValueOrException<T&>
{
public:

	void set(T& t)
	{
		t_ = &t;
		is_ready_ = true;
	}

	void set(std::exception_ptr e)
	{
		e_ = std::move(e);
		is_ready_ = true;
	}

	T& get()
	{
		if (e_)
		{
			std::rethrow_exception(e_);
		}
		return *t_;
	}

	bool is_ready()
	{
		return is_ready_;
	}

private:
	bool is_ready_ = false;
	T* t_ = nullptr;
	std::exception_ptr e_;
};

template<>
class ValueOrException<void>
{
public:

	void set()
	{
		is_ready_ = true;
	}

	void set(std::exception_ptr e)
	{
		e_ = std::move(e);
		is_ready_ = true;
	}

	void get()
	{
		if (e_)
		{
			std::rethrow_exception(e_);
		}
	}

	bool is_ready()
	{
		return is_ready_;
	}

private:
	bool is_ready_ = false;
	std::exception_ptr e_;
};


template<class ... Args>
class CoroPromiseMixin : public PromiseMixin<Args...>
{
	friend class Future<Args...>;
public:

	CoroPromiseMixin() noexcept
	{}

	Future<Args...> get_return_object()
	{
		return Future<Args...>(*(this->state_), true);
	}

	auto initial_suspend() const
	{
		return std::experimental::suspend_never{};
	}

	auto final_suspend() const
	{
		return std::experimental::suspend_never{};
	}

	void unhandled_exception()
	{
		auto eptr = std::current_exception();
		this->reject(eptr);
	}
};

template<class T>
class Promise< T> : public CoroPromiseMixin<T>
{
	friend class Future<T>;
public:

	Promise() noexcept
	{}

	template<class P>
	void return_value(P&& value)
	{
		this->resolve(std::forward<P&&>(value));
	}
	
};

template<class T>
class Promise<T&> : public CoroPromiseMixin<T&>
{
	friend class Future<T&>;
public:

	Promise() noexcept
	{}

	void return_value(T& value)
	{
		this->resolve(value);
	}
};

template<>
class Future<>;

template<>
class Promise<> : public CoroPromiseMixin<>
{
	friend class Future<>;
public:

	Promise() noexcept
	{}

	void return_void()
	{
		this->resolve();
	}
};

template<class V>
auto make_otherwise(V* value)
{
	return [value](const std::exception& ex)
	{
		const Ex* e = dynamic_cast<const Ex*>(&ex);
		if(e)
		{
			value->set(e->make_exception_ptr());
		}
		else
		{
			value->set(std::make_exception_ptr(ex));
		}
		return true;
	};
}

template<class V>
auto make_resuming_otherwise(V* value,std::experimental::coroutine_handle<> resume_cb)
{
	return [value,resume_cb](const std::exception& ex)
	{
		const Ex* e = dynamic_cast<const Ex*>(&ex);
		if(e)
		{
			value->set(e->make_exception_ptr());
		}
		else
		{
			value->set(std::make_exception_ptr(ex));
		}
		resume_cb.resume();
		return true;
	};
}

template<class T>
class Future<T> : public FutureMixin<T>
{
public:

	typedef typename FutureMixin<T>::PromiseType promise_type;

	Future(PromiseState<T>& p, bool isCoro = false) noexcept
		: FutureMixin<T>(p)
	{
		if (isCoro)
		{
			this->promise_->cb_ = [this](T t)
			{
				value_.set(std::move(t));
			};

			this->promise_->err_ = make_otherwise(&value_);			
		}
	}

	Future(const Future<T>& rhs) noexcept
	{
		this->promise_ = rhs.promise_;
		this->value_ = rhs.value_;
	}

	Future() noexcept
	{}

	Future& operator=(const Future& rhs)
	{
		this->promise_ = rhs.promise_;
		this->value_ = rhs.value_;
		return *this;
	}

	// make Future<T> awaitable

	T  await_resume()
	{
		return std::move(value_.get());
	}

	bool  await_ready()
	{
		return  value_.is_ready();
	}

	void  await_suspend(std::experimental::coroutine_handle<> resume_cb)
	{
		this->then([resume_cb, this](T t)
		{
			value_.set(std::move(t));
			resume_cb.resume();
		});

		this->promise_->err_ = make_resuming_otherwise(&value_,resume_cb);
	}

private:

	ValueOrException<T> value_;
};

template<class T>
class Future<T&> : public FutureMixin<T&>
{
public:

	typedef typename FutureMixin<T&>::PromiseType promise_type;

	Future(PromiseState<T&>& p, bool isCoro = false) noexcept
		: FutureMixin<T&>(p)
	{
		if (isCoro)
		{
			this->promise_->cb_ = [this](T& t)
			{
				value_.set(t);
			};

			this->promise_->err_ = make_otherwise(&value_);
		}
	}

	Future(const Future<T>& rhs) noexcept
	{
		this->promise_ = rhs.promise_;
		this->value_ = rhs.value_;
	}

	Future() noexcept
	{}

	Future& operator=(const Future& rhs)
	{
		this->promise_ = rhs.promise_;
		this->value_ = rhs.value_;
		return *this;
	}

	// make Future<T> awaitable

	T&  await_resume()
	{
		return value_.get();
	}

	bool  await_ready()
	{
		return  value_.is_ready();
	}

	void  await_suspend(std::experimental::coroutine_handle<> resume_cb)
	{
		this->then([resume_cb, this](T& t)
		{
			value_.set(t);
			resume_cb.resume();
		});

		this->promise_->err_ = make_resuming_otherwise(&value_,resume_cb);
	}

private:

	ValueOrException<T&> value_;
};


template<>
class Future<> : public FutureMixin<>
{
public:

	typedef typename FutureMixin<>::PromiseType promise_type;

	Future(PromiseState<>& p, bool isCoro = false) noexcept
		: FutureMixin<>(p)
	{
		if (isCoro)
		{
			this->promise_->cb_ = [this]()
			{
				value_.set();
			};

			this->promise_->err_ = make_otherwise(&value_);
		}
	}

	Future(const Future<>& rhs) noexcept
	{
		this->promise_ = rhs.promise_;
		this->value_ = rhs.value_;
	}

	Future& operator=(const Future& rhs)
	{
		this->promise_ = rhs.promise_;
		this->value_ = rhs.value_;
		return *this;
	}

	// make Future<> awaitable

	void  await_resume()
	{
		value_.get();
	}

	bool  await_ready()
	{
		return  value_.is_ready();
	}

	void  await_suspend(std::experimental::coroutine_handle<> resume_cb)
	{
		this->then([resume_cb, this]()
		{
			value_.set();
			resume_cb.resume();
		});

		this->promise_->err_ = make_resuming_otherwise(&value_,resume_cb);
	}

private:

	ValueOrException<void> value_;
};


}

#endif


