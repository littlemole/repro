#ifndef DEFINE_MOL_PROMISE_PROMISE_DEF_GUARD_DEFINE_
#define DEFINE_MOL_PROMISE_PROMISE_DEF_GUARD_DEFINE_

#include "reprocpp/traits.h"
#include "reprocpp/future.h"
#include "reprocpp/promisestate.h"


/**
 * \file promise.h
 * Main promise header. Provides Promise<...Args> template.
 */

namespace repro     {

/**  
	\class PromiseMixin
	\brief shared promise impl with coroutine impl
*/

template<class ... Args>
class PromiseMixin
{
public:

	PromiseMixin() noexcept
    : state_( new PromiseState<Args...>() )
    {
    }

	static Promise<Args...> create() noexcept
	{
		return Promise<Args...>();
	}

	///
    /// return the future
    Future<Args...> future() const noexcept 
    {
    	return state_->future();
    }

	///
    /// resolve the future and invoke the completion handler
	template<class ... VArgs>
    void resolve( VArgs&& ... args ) const noexcept 
    {
        state_->resolve(std::forward<VArgs>(args)...);
    }

	/// resolve the future from another future.
	/// chains promises and allows to combine futures.
    void resolve( Future<Args...>& f ) const noexcept 
    {
        state_->resolve(f);
    }

    void resolve( const Future<Args...>& f ) const noexcept 
    {
        state_->resolve(f);
    }

	///
    /// reject the future and specify exception
	template<class E>
	void reject(const E& e) const noexcept
	{
		const Ex* ex = dynamic_cast<const Ex*>(&e);
		if(ex)
		{
			state_->reject(ex->make_exception_ptr());
		}
		else
		{
			state_->reject(std::make_exception_ptr(e));
		}
	}

    void reject(std::exception_ptr eptr) const noexcept 
    {
		state_->reject(eptr);
	}
	
protected:

	PromiseStateHolder<Args...> state_;
};

/**
* 
* \brief main promise template.
* \copydoc PromiseMixin
*
* gives client a mean to resolve or reject the promise.
*/

template<class ... Args>
class Promise : public PromiseMixin<Args...>
{
	friend class Future<Args...>;
public:

	Promise() noexcept
	{}
};

//! returns a promise
//! 
template<class ... Args>
Promise<Args...> promise() noexcept
{
	return Promise<Args...>::create();
}



} // end namespace org

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
#include "reprocpp/coro/resumable.h"
#endif

#endif


