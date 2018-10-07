#ifndef DEFINE_MOL_CORO_FUTURE_PROMISE_RESUMABLE_DEF_GUARD_DEFINE_
#define DEFINE_MOL_CORO_FUTURE_PROMISE_RESUMABLE_DEF_GUARD_DEFINE_

#include "reprocpp/coro/corovalue.h"

namespace repro {

template<>
class Future<>;

template<class V>
auto make_otherwise(V* value)
{
	return [value](std::exception_ptr ex)
	{
		value->set(ex);
		return true;
	};
}

template<class V>
auto make_resuming_otherwise(V* value,std::experimental::coroutine_handle<> resume_cb)
{
	return [value,resume_cb](std::exception_ptr ex)
	{
		value->set(ex);
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


