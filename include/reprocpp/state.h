#ifndef MOL_DEF_GUARD_DEFINE_REPRO_CPP_STATE_DEF_GUARD_
#define MOL_DEF_GUARD_DEFINE_REPRO_CPP_STATE_DEF_GUARD_

#include "reprocpp/promise_mixin.h"

namespace repro {
namespace impl {

    template<class ...Args>
    class promise_state :
        public impl::promise_state_mixin<Args...>
    {
    public:

        template<class F, typename std::enable_if<traits::returns_void<F, Args...>::value>::type * = nullptr>
        void then(F&& cb)
        {
            this->cb_ = std::forward<F>(cb);
            if (value_.has_value())
            {
                std::apply(this->cb_, std::move(value_.value()));
            }
        }

        template<class F, typename std::enable_if<!traits::returns_void<F, Args...>::value>::type * = nullptr>
        auto then(F&& cb);

        template<class ... VArgs>
        void resolve(VArgs&& ... args) 
        {
            auto stabilize_ref_count = this->shared_from_this();

            value_.emplace(std::forward<VArgs>(args)...);
            if (this->resume_cb_)
            {
                this->resume_cb_.resume();
            }
            else
                if (this->cb_)
                {
                    try {
                        std::apply(this->cb_, std::move(value_.value()));
                    }
                    catch (...)
                    {
                        this->reject(std::current_exception());
                    }
                }
        }

        // coro support

        std::tuple<Args...> resume() const
        {
            if (this->ex_.has_value())
            {
                std::rethrow_exception(this->ex_.value());
            }
            return std::move(value_.value());
        }

        bool ready() const
        {
            return  value_.has_value() || this->ex_.has_value();
        }

    private:

        mutable std::optional<std::tuple<Args...>> value_;
        std::function<void(Args...)> cb_;

    };


    template<class T>
    class promise_state<T> :
        public impl::promise_state_mixin<T>
    {
    public:

        template<class F, typename std::enable_if<traits::returns_void<F, T>::value>::type * = nullptr>
        void then(F&& cb)
        {
            this->cb_ = std::forward<F>(cb);
            if (value_.has_value())
            {
                this->cb_(std::move(value_.value()));
            }
        }

        template<class F, typename std::enable_if<!traits::returns_void<F, T>::value>::type * = nullptr>
        auto then(F&& cb);

        template<class P>
        void resolve(P&& p)  
        {
            auto stabilize_ref_count = this->shared_from_this();

            value_.emplace(std::forward<P>(p));
            if (this->resume_cb_)
            {
                this->resume_cb_.resume();
            }
            else
                if (this->cb_)
                {
                    try {
                        this->cb_(std::move(value_.value()));
                    }
                    catch (...)
                    {
                        this->reject(std::current_exception());
                    }
                }
        }

        // coro support

        T resume() const
        {
            if (this->ex_.has_value())
            {
                std::rethrow_exception(this->ex_.value());
            }

            return std::move(value_.value());
        }

        bool ready() const
        {
            return  value_.has_value() || this->ex_.has_value();
        }

    private:

        mutable std::optional<T> value_;
        std::function<void(T)> cb_;

    };



    template<>
    class promise_state<void> :
        public impl::promise_state_mixin<void>
    {
    public:

        template<class F, typename std::enable_if<traits::returns_void<F>::value>::type * = nullptr>
        void then(F cb)
        {
            this->cb_ = cb;
            if (done_)
            {
                this->cb_();
            }
        }

        template<class F, typename std::enable_if<!traits::returns_void<F>::value>::type * = nullptr>
        auto then(F&& cb);

        void resolve() 
        {
            auto stabilize_ref_count = this->shared_from_this();

            done_ = true;
            if (this->resume_cb_)
            {
                this->resume_cb_.resume();
            }
            else
                if (this->cb_)
                {
                    try
                    { 
                        this->cb_();
                    }
                    catch (...)
                    {
                        this->reject(std::current_exception());
                    }
                }
        }

        // coro support

        void resume() const
        {
            if (this->ex_.has_value())
            {
                std::rethrow_exception(this->ex_.value());
            }
        }

        bool ready() const
        {
            return  done_ || this->ex_.has_value();
        }

    private:

        mutable bool done_ = false;
        std::function<void()> cb_;
    };


    template<class T, class E, class F, class ... Args>
    auto bind(T& cb_,E& err_, F&& cb)
    {
        using FutureType = typename std::invoke_result<F,Args...>::type;
        using PromiseType = typename FutureType::PromiseType;

        auto p = PromiseType();

        std::function<bool(std::exception_ptr)> chain = err_;
		err_ = [p,chain](std::exception_ptr e)
		{
			if(chain && chain(e))
			{
				return true;
			}			
			p.reject(e);
			return true;
		};

        cb_ = [p, cb](auto&& ... args)
        {
            auto r = cb(std::forward<decltype(args)>(args)...);
            p.resolve(r);
        };


        return p.future();
    }


    template<class ...Args>
    template<class F, typename std::enable_if<!traits::returns_void<F, Args...>::value>::type*>
    auto promise_state<Args...>::then(F&& cb)
    {
        return ::repro::impl::bind<std::function<void(Args...)>,decltype(this->err_),F,Args...>(this->cb_,this->err_,std::forward<F>(cb));

    }

    template<class T>
    template<class F, typename std::enable_if<!traits::returns_void<F, T>::value>::type*>
    auto promise_state<T>::then(F&& cb)
    {
        return ::repro::impl::bind<std::function<void(T)>,decltype(this->err_),F,T>(this->cb_,this->err_,std::forward<F>(cb));

    }


    template<class F, typename std::enable_if<!traits::returns_void<F>::value>::type*>
    auto promise_state<void>::then(F&& cb)
    {
        return bind<std::function<void()>,decltype(this->err_),F, void>(this->cb_,this->err_,std::forward<F>(cb));

    }

}} // end namespaces


#endif

