#ifndef DEFINE_MOL_HTTP_SERVER_PROMISE_DEF_GUARD_DEFINE_
#define DEFINE_MOL_HTTP_SERVER_PROMISE_DEF_GUARD_DEFINE_

#include "reprocpp/ex.h"
#include "reprocpp/debug.h"
#include <memory>
#include <functional>

/*
 * Main promise header. Provides Promise<...Args> and Future<...Args>)
 * class templates.
 */

namespace repro     {


LITTLE_MOLE_DECLARE_DEBUG_REF_CNT(promises);

// forwards

template<class ... Args>
class Future;

template<class ... Args>
class Promise;

template<class ... Args>
class PromiseState;

// helper templates

template<class R>
struct IsFuture
{
    static const bool value = false;
};

template<class ... Args>
struct IsFuture<Future<Args...>>
{
    static const bool value = true;
};

template<class F, class ... Args>
struct ReturnsVoid
{
	typedef typename std::result_of<F(Args ...)>::type ResultType;
	static const bool value = std::is_void<ResultType>::value;
};


template<class ... Args>
class FutureMixin
{
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
		promise_->cb_ = f;
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

		promise_->err_ = [p](const std::exception& e)
		{
			p.reject(e);
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
		promise_->err_ = [e](const std::exception& ex)
		{
			const WrappedException* wep = dynamic_cast<const WrappedException*>(&ex);
			if(wep)
			{
				wep->raise(e);
			}
			else
			{
				e(ex);
			}
		};
		return *(Future<Args...>*)(this);
	}


protected:

	PromiseState<Args...>* promise_;
};


/**
* \class Future<...Args>
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


template<class ... Args>
class PromiseState : public std::enable_shared_from_this<PromiseState<Args...>>
{
	friend class Future<Args...>;
	friend class FutureMixin<Args...>;
	friend class Promise<Args...>;

public:

	typedef std::shared_ptr<PromiseState<Args ...>> Ptr;

	PromiseState()
	{
		LITTLE_MOLE_ADDREF_DEBUG_REF_CNT(promises);

		cb_ = [](Args...) {};
		err_ = [](const std::exception& ex) {
			throw;
		};
	}

	~PromiseState()
	{
		LITTLE_MOLE_RELEASE_DEBUG_REF_CNT(promises);
	}

	Future<Args...> future() noexcept
	{
		return Future<Args...>(*this);
	}

	template<class ... VArgs>
	void resolve(VArgs&& ... args)
	{
		// stabilize ref count so we have 
		// a valid this pointer until end of
		// method
		Ptr ptr = this->shared_from_this();

		try
		{
			cb_(std::forward<VArgs&&>(args)...);
		}
		catch (...)
		{
			reject(wrap_exception(std::current_exception()));
		}
	}

	void resolve(Future<Args...>& f) noexcept
	{
		Ptr ptr = this->shared_from_this();

		f.then([ptr](Args ... args)
		{
			ptr->resolve(args...);
		});

		f.otherwise([ptr](const std::exception& e)
		{
			ptr->reject(e);
		});
	}

	void reject(const std::exception& e)
	{
		// stabilize ref count so we have 
		// a valid this pointer until end of
		// method

		Ptr ptr = this->shared_from_this();

		try
		{
			err_(e);
		}
		catch (...)
		{
			throw;
		}
	}

protected:

	std::function<void(Args ...)> cb_;
	std::function<void(const std::exception&)> err_;

	PromiseState(const PromiseState<Args ...>& rhs) = delete;
	PromiseState(PromiseState<Args ...>&& rhs) = delete;
	PromiseState& operator=(PromiseState<Args ...>&& rhs) = delete;
	PromiseState& operator=(const PromiseState<Args ...>& rhs) = delete;
};


template<class ... Args>
class PromiseMixin
{
public:

	PromiseMixin() noexcept
    : state_( std::make_shared<PromiseState<Args...>>() )
    {
    }

	static Promise<Args...> create() noexcept
	{
		return Promise<Args...>();
	}


    /// return the future
    Future<Args...> future() const noexcept 
    {
    	return state_->future();
    }

    /// resolve the future and invoke the completion handler
	template<class ... VArgs>
    void resolve( VArgs&& ... args ) const noexcept 
    {
        state_->resolve(std::forward<VArgs&&>(args)...);
    }

    void resolve( Future<Args...>& f ) const noexcept 
    {
        state_->resolve(f);
    }

    void resolve( const Future<Args...>& f ) const noexcept 
    {
        state_->resolve(f);
    }

    /// reject the future and specify exception
	template<class E>
    void reject(const E& e) const noexcept 
    {
        state_->reject(wrap_exception(e));
    }

    void reject(std::exception_ptr eptr) const noexcept 
    {
		state_->reject(wrap_exception(eptr));
    }

protected:

    std::shared_ptr<PromiseState<Args...>> state_;
};

/**
* \class Promise<...Args>
* \brief main promise template.
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

/// returns a promise
template<class ... Args>
Promise<Args...> promise() noexcept
{
	return Promise<Args...>::create();
}

/// ES6 Ecma Script promise style
template<class ...Args,class T>
Future<Args...> future( T cb )    
{
    auto p = promise<Args...>();

    auto resolve = [p]( Args... args) 
    {
        p.resolve(args...);
    };

    auto reject = [p]( const std::exception& ex) 
    {
        p.reject(ex);
    };

    cb(resolve,reject);

    return p.future();
}

} // end namespace org

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
#include "reprocpp/resumable.h"
#endif

#endif


