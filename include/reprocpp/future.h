#ifndef DEFINE_MOL_PROMISE_FUTURE_DEF_GUARD_DEFINE_
#define DEFINE_MOL_PROMISE_FUTURE_DEF_GUARD_DEFINE_

#include "reprocpp/traits.h"

/** \file future.h
 * future impl.
 */

namespace repro     {

/// \class FutureMixin
/// common Future code shared with coro impl

template<class ... Args>
class FutureMixin
{
friend PromiseState<Args...>;
public:

	typedef Promise<Args ...> PromiseType;

	FutureMixin(PromiseState<Args...>& p) noexcept
		: promise_(&p)
	{}

	FutureMixin() noexcept
		: promise_(nullptr)
	{}

	/// simple then() case passing a callback returning void.
	/// in this case we return this so client code can call otherwise()
	/// this terminates any chain of continuation handlers.
	template<class F, typename std::enable_if<ReturnsVoid<F, Args...>::value>::type* = nullptr>
	Future<Args...>& then(F f) noexcept
	{
#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
		// if this was a coroutine produced promise, it will
		// have default error handler that needs reset IF
		// you attach a then/otherwise chain manually.
		// this allows mixing of coroutine and then/otherwise
		// style of handling promises
		promise_->err_ = [](std::exception_ptr){ return false; };
#endif		
		promise_->cb_ = f; // swap upwards?
		return *(Future<Args...>*)(this);
	}

	/// then() implementation that gets called when passing a callback
	/// returning a Future<Args...>. used to create chains of then()
	/// calls.
	template<class F, typename std::enable_if<(!ReturnsVoid<F, Args...>::value) && (IsFuture<typename std::result_of<F(Args...)>::type>::value)>::type* = nullptr>
	auto then(F f) noexcept -> typename std::result_of<F(Args ...)>::type
	{
		typedef typename std::result_of<F(Args ...)>::type FutureType;
		typedef typename FutureType::PromiseType PromiseType;

		auto p = PromiseType::create();

		std::function<bool(std::exception_ptr)> chain = promise_->err_;
		promise_->err_ = [p,chain](std::exception_ptr e)
		{
			if(chain && chain(e))
			{
				return true;
			}			
			p.reject(e);
			return true;
		};

		promise_->cb_ = [f, p](Args ... args)
		{
			auto r = f(args...);
			p.resolve(r);
		};

		return p.future();
	}

	/// promise gets rejected
	template<class E>
	Future<Args...>& otherwise(E e) noexcept
	{
		std::function<void(first_argument<E>)> f = e;

        otherwise_impl(f);

		return *(Future<Args...>*)(this);
	}

protected:

	PromiseState<Args...>* promise_;

	template<class E>
    void otherwise_impl(std::function<void(const E&)> fun)  noexcept 
    {
        std::function<bool(std::exception_ptr)> chain = promise_->err_;

		promise_->err_ = [chain,fun](std::exception_ptr eptr)
		{
			if(chain && chain(eptr))
			{
				return true;
			}

			try
			{
				std::rethrow_exception(eptr);
			}
			catch(const std::exception& e)
			{
				const E* ex = dynamic_cast<const E*>(&e);
				if(ex)
				{
					fun(*ex);
					return true;
				}
			}
			
			return false;
		};
    }			
};


/**
* \copydoc FutureMixin
* \brief Future part of the async completion.
*
* Continuation reference that provides clients with then() and otherwise()
* accessors.
*/

template<class ... Args>
class Future : public FutureMixin<Args...>
{
public:
    
    Future(PromiseState<Args...>& p) noexcept
		: FutureMixin<Args...>(p)
    {}

    Future() noexcept
    {}

    Future& operator=(const Future& rhs)
    {
        this->promise_ = rhs.promise_;
        return *this;
    }
};

} // end namespace org


#endif


