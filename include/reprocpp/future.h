#ifndef MOL_DEF_GUARD_DEFINE_REPRO_CPP_FUTURE_DEF_GUARD_
#define MOL_DEF_GUARD_DEFINE_REPRO_CPP_FUTURE_DEF_GUARD_


#include "reprocpp/state.h"

namespace repro {

template<class ... Args>
class Promise;

namespace impl {

    template<class ...Args>
    class Future_mixin
    {
    public:

        using PromiseType = Promise<Args...>;
        using promise_type = PromiseType;
        
        Future_mixin()
        {}

        Future_mixin(std::shared_ptr<impl::promise_state<Args...>> state)
            : state_(state)
        {}

        template<class F, typename std::enable_if<traits::returns_void<F, Args...>::value>::type * = nullptr>
        Future<Args...>& then(F&& cb)
        {
            state_->then(std::forward<F>(cb));
            return *static_cast<Future<Args...>*>(this);
        }

        template<class F, typename std::enable_if<!traits::returns_void<F, Args...>::value>::type * = nullptr>
        auto then(F&& cb)
        {
            return state_->then(std::forward<F>(cb));
        }


#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
        // coro impl

        bool  await_ready() const
        {
            return  state_->ready();
        }

        void  await_suspend(std::experimental::coroutine_handle<> resume_cb)
        {
            state_->suspend(resume_cb);
        }
#endif

    protected:
        std::shared_ptr<impl::promise_state<Args...>> state_;
    };

} // end namespace impl

template<class ...Args>
class Future : public impl::Future_mixin<Args...>
{
public:

    Future()
    {}

    Future(std::shared_ptr<impl::promise_state<Args...>> state)
        : impl::Future_mixin<Args...>(state)
    {}

    template<class E>
    auto otherwise(E&& e)
    {
        this->state_->otherwise(std::forward<E>(e));
        return *this;
    }

    template<class E, class ... VArgs>
    auto otherwise(E&& e, VArgs&& ... args)
    {
        this->state_->otherwise(std::forward<E>(e));
        this->otherwise(args...);
        return *this;
    }

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
    std::tuple<Args...>  await_resume()
    {
        return this->state_->resume();
    }
#endif    
};


template<class T>
class Future<T> : public impl::Future_mixin<T>
{
public:

    Future()
    {}

    Future(std::shared_ptr<impl::promise_state<T>> state)
        : impl::Future_mixin<T>(state)
    {}

    template<class E>
    auto otherwise(E&& e)
    {
        this->state_->otherwise(std::forward<E>(e));
        return *this;
    }

    template<class E, class ... VArgs>
    auto otherwise(E&& e, VArgs&& ... args)
    {
        this->state_->otherwise(std::forward<E>(e));
        this->otherwise(args...);
        return *this;
    }


#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
    T  await_resume()
    {
        return this->state_->resume();
    }
#endif    
};


template<>
class Future<void> : public impl::Future_mixin<>
{
public:

    Future()
    {}

    Future(std::shared_ptr<impl::promise_state<>> state)
        : impl::Future_mixin<>(state)
    {}

    template<class E>
    auto otherwise(E&& e)
    {
        this->state_->otherwise(std::forward<E>(e));
        return *this;
    }

    template<class E, class ... VArgs>
    auto otherwise(E&& e, VArgs&& ... args)
    {
        this->state_->otherwise(std::forward<E>(e));
        this->otherwise(args...);
        return *this;
    }


#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
    void await_resume()
    {
        state_->resume();
    }
#endif    
};


template<>
class Future<> : public impl::Future_mixin<>
{
public:

    Future()
    {}

    Future(std::shared_ptr<impl::promise_state<>> state)
        : impl::Future_mixin<>(state)
    {}

    template<class E>
    auto otherwise(E&& e)
    {
        this->state_->otherwise(std::forward<E>(e));
        return *this;
    }

    template<class E, class ... VArgs>
    auto otherwise(E&& e, VArgs&& ... args)
    {
        this->state_->otherwise(std::forward<E>(e));
        this->otherwise(args...);
        return *this;
    }

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
    void await_resume()
    {
        state_->resume();
    }
#endif    
};

} // end namespace repro

#endif
