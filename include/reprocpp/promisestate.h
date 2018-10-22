#ifndef DEFINE_MOL_PROMISE_STATE_DEF_GUARD_DEFINE_
#define DEFINE_MOL_PROMISE_STATE_DEF_GUARD_DEFINE_

#include "reprocpp/future.h"
#include "reprocpp/mempool.h"
#include <atomic>

/*
 * shared promise state
 */

namespace repro     {


template<class ... Args>
class PromiseState 
{
	friend class Future<Args...>;
	friend class FutureMixin<Args...>;
	friend class Promise<Args...>;

public:

	typedef PromiseState<Args ...>* Ptr;

	PromiseState()
		: refcount_(1)
	{
		LITTLE_MOLE_ADDREF_DEBUG_REF_CNT(promises);

		cb_ = [](Args...) {};
		err_ = [](std::exception_ptr ex) {
			return false;
		};
	}

	~PromiseState()
	{
		LITTLE_MOLE_RELEASE_DEBUG_REF_CNT(promises);
	}

	// return future for this promise

	Future<Args...> future() noexcept
	{
		return Future<Args...>(*this);
	}

	// resolve a value through this promise

	template<class ... VArgs>
	void resolve(VArgs&& ... args)
	{
		// stabilize ref count so we have 
		// a valid this pointer until end of
		// method
		Ptr ptr = this->addref();

		try
		{
			cb_(std::forward<VArgs&&>(args)...);
		}
		catch (...)
		{
			reject(std::current_exception());
		}

		ptr->release();
	}

	// resolve a Future through this promise.
	// this will effectively chain promises.

	void resolve(Future<Args...>& f) noexcept
	{
		Ptr ptr = this->addref();

		PromiseStateHolder<Args...> holder(ptr);

		f.then([holder](Args ... args)
		{
			holder->resolve(args...);
		});

		f.promise_->err_ = [holder](std::exception_ptr eptr)
		{
			holder->reject(eptr);
			return true;
		};
	}
	
	// reject a promise
	void reject(std::exception_ptr eptr)
	{
		// stabilize ref count so we have 
		// a valid this pointer until end of
		// method

		Ptr ptr = this->addref();

		try
		{
			bool handled = err_(eptr);
			if(!handled)
			{
				std::rethrow_exception(eptr);
			}
		}
		catch (...)
		{
			ptr->release();
			throw;
		}

		ptr->release();
	}

	// custom memory management via memory pool

	void* operator new(size_t s)
	{
		return mempool<sizeof(PromiseState<Args...>)>().alloc();  
	}
	
	void operator delete(void* p)
	{
		return mempool<sizeof(PromiseState<Args...>)>().free(p);
	}

	// single threaded refcounting support

	Ptr addref()
	{
		refcount_++;
		return this;
	}

	void release()
	{
		refcount_--;
		if(refcount_==0)
		{
			delete this;
		}
	}

protected:

	std::function<void(Args ...)> cb_;
	std::function<bool(std::exception_ptr)> err_;

	PromiseState(const PromiseState<Args ...>& rhs) = delete;
	PromiseState(PromiseState<Args ...>&& rhs) = delete;
	PromiseState& operator=(PromiseState<Args ...>&& rhs) = delete;
	PromiseState& operator=(const PromiseState<Args ...>& rhs) = delete;

	std::atomic<long> refcount_;
};

// simple smart pointer to handle refcounting of PromiseState objects
//  in a RAII style

template<class ... Args>
class PromiseStateHolder 
{
public:

	typedef PromiseState<Args ...>* Ptr;


	PromiseStateHolder( Ptr p) : ptr_(p) {}

	~PromiseStateHolder()
	{
		if(ptr_)
		{
			ptr_->release();
			ptr_ = nullptr;
		}
	}

	PromiseStateHolder( const PromiseStateHolder& rhs)
	{
		if(this == &rhs) return;
		ptr_ = rhs->addref();
	}

	PromiseStateHolder& operator=( const PromiseStateHolder& rhs)
	{
		if(this == &rhs) return *this;

		if(ptr_)
		{
			ptr_->release();
			ptr_ = nullptr;
		}

		ptr_ = rhs->addref();
		return *this;
	}

	Ptr operator->() const
	{
		return ptr_;
	}

	Ptr get() const
	{
		return ptr_;
	}

	PromiseStateHolder( PromiseStateHolder&& rhs)
	{
		if(this == &rhs) return;

		ptr_ = rhs.ptr_;
		rhs.ptr_ = nullptr;
	}

	PromiseStateHolder& operator=(PromiseStateHolder&& rhs)
	{
		if(this == &rhs) return *this;

		if(ptr_)
		{
			ptr_->release();
			ptr_ = nullptr;
		}

		ptr_ = rhs.ptr_;
		rhs.ptr_ = nullptr;
		return *this;
	}

private:

	Ptr ptr_ = nullptr;	
};


} // end namespace org


#endif


