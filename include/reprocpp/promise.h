#ifndef MOL_DEF_GUARD_DEFINE_REPRO_CPP_PROMISE_DEF_GUARD_
#define MOL_DEF_GUARD_DEFINE_REPRO_CPP_PROMISE_DEF_GUARD_

#include "reprocpp/future.h"
#include "reprocpp/mempool.h"

namespace repro {

template<class ... Args>
class Future;

namespace impl {

    template<class State>
    class Promise_mixin
    {
    public:

        Promise_mixin()
            : state_(
                std::allocate_shared<State>(
                    impl::mempool<State>()
                )
            )
        {}

        template<class ... VArgs>
        void resolve(VArgs&& ... args) const
        {
            state_->resolve(std::forward<VArgs>(args)...);
        }

        template<class ... VArgs>
        typename State::FutureType resolved(VArgs&& ... args) const
        {
            state_->resolve(std::forward<VArgs>(args)...);
            return future();
        }

        template<class ... VArgs>
        void resolve(Future<VArgs...> f) const
        {
            auto s = state_;
            f.then([s](auto&& ... vargs)
            {
                s->resolve(std::forward<decltype(vargs)>(vargs)...);
            })
            .otherwise([s](const std::exception_ptr& eptr)
            {
                s->reject(eptr);
            });
        }

        template<class E>
        void reject(E&& e) const
        {
            auto eptr = std::make_exception_ptr(e);
            state_->reject(eptr);
        }

        void reject(std::exception_ptr eptr) const
        {
            state_->reject(eptr);
        }

        template<class E>
        typename State::FutureType rejected(E&& e) const
        {
            auto eptr = std::make_exception_ptr(e);
            state_->reject(eptr);
            return future();
        }

        typename State::FutureType rejected(std::exception_ptr eptr) const
        {
            state_->reject(eptr);
            return future();
        }

        typename State::FutureType future() const
        {
            return typename State::FutureType(state_);
        }

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED

        // coro impl

        typename State::FutureType get_return_object() const
        {
            return typename State::FutureType(state_);
        }

        auto initial_suspend() const
        {
            return std::experimental::suspend_never{};
        }

        auto final_suspend() const
        {
            return std::experimental::suspend_never{};
        }

        void unhandled_exception() const
        {
            auto eptr = std::current_exception();
            this->reject(eptr);
        }

#endif

    protected:
        std::shared_ptr<State> state_;
    };

} // end namespace impl

template<class ... Args>
class Promise : public impl::Promise_mixin< impl::promise_state<Args...>>
{
public:

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED

    template<class ...VArgs>
    void return_value(VArgs&& ... vargs)
    {
        this->resolve(std::forward<VArgs>(vargs)...);
    }
#endif    
};


template<class T>
class Promise<T> : public impl::Promise_mixin<impl::promise_state<T>>
{
public:

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED

    template<class P>
    void return_value(P&& p)
    {
        this->resolve(std::forward<P>(p));
    }
#endif    
};

template<>
class Promise<void> : public impl::Promise_mixin< impl::promise_state<void>>
{
 public:
 #ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
    void return_void()
    {
        this->resolve();
    }
#endif    
};

template<>
class Promise<> : public impl::Promise_mixin< impl::promise_state<>>
{
public:
#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
    void return_void()
    {
        this->resolve();
    }
#endif    
};
template<class ... Args>
auto promise()
{
    return Promise<Args...>();
}

template<class P>
auto reject(P p)
{
    return [p](const std::exception_ptr& e)
    {
        p.reject(e);
    };
}

template<class P>
auto resolve(P p)
{
    return [p](auto&& t)
    {
        p.resolve(std::forward<decltype(t)>(t));
    };
}



template<class ...Args, class F>
auto future(F fun)
{
    auto p = promise<Args...>();

    auto resolve = [p](auto&& ... args) { p.resolve(std::forward<decltype(args)>(args)...); };
    auto reject = [p](auto&& e) { p.reject(std::forward<decltype(e)>(e)); };
    
    try
    {
        fun(resolve, reject);
    }
    catch (...)
    {
        p.reject(std::current_exception());
    }

    return p.future();
}

using Async = Future<>;

} // end namespace repro

#endif
