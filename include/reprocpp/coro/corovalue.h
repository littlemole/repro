#ifndef DEFINE_MOL_CORO_VALUE_PROMISE_RESUMABLE_DEF_GUARD_DEFINE_
#define DEFINE_MOL_CORO_VALUE_PROMISE_RESUMABLE_DEF_GUARD_DEFINE_

#ifdef _WIN32
#include <experimental/resumable>
#else
#include "reprocpp/coro/coroutine.h"
#endif

namespace repro {
namespace impl  {


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



}
}

#endif


