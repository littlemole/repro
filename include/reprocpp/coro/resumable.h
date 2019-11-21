#ifndef DEFINE_MOL_CORO_PROMISE_PROMISE_RESUMABLE_DEF_GUARD_DEFINE_
#define DEFINE_MOL_CORO_PROMISE_PROMISE_RESUMABLE_DEF_GUARD_DEFINE_

#include "reprocpp/coro/corofuture.h"

namespace repro {
namespace impl  {



template<class ... Args>
class CoroPromiseMixin : public PromiseMixin<Args...>
{
	friend class Future<Args...>;
public:

	CoroPromiseMixin() noexcept
	{}

	Future<Args...> get_return_object()
	{
		return Future<Args...>(*(this->state_.get()), true);
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

} // end ns impl

template<class T>
class Promise< T> : public impl::CoroPromiseMixin<T>
{
	friend class Future<T>;
public:

	Promise() noexcept
	{}

	template<class P>
	void return_value(P&& value)
	{
		this->resolve(std::forward<P>(value));
	}
	
};

template<class T>
class Promise<T&> : public impl::CoroPromiseMixin<T&>
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
class Promise<> : public impl::CoroPromiseMixin<>
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


}

#endif


