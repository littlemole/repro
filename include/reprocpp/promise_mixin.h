#ifndef MOL_DEF_GUARD_DEFINE_REPRO_CPP_MIXIN_DEF_GUARD_
#define MOL_DEF_GUARD_DEFINE_REPRO_CPP_MIXIN_DEF_GUARD_

#include <optional>
#include <atomic>

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
#include <experimental/resumable>
#endif


#include "reprocpp/traits.h"
#include "reprocpp/debug.h"

namespace repro {

template<class ...Args>
class Future;

template<class ...Args>
class Promise;


namespace impl {

    template<class ...Args>
    class promise_state_mixin : public std::enable_shared_from_this<promise_state_mixin<Args...>>
    {
    public:

        using FutureType = Future<Args...>;
        using PromiseType = Promise<Args...>;

        promise_state_mixin()
        {
            REPRO_MONITOR_INCR(promise);
        }

        ~promise_state_mixin()
        {
            REPRO_MONITOR_DECR(promise);
        }

        template<class E>
        void otherwise(E&& e)
        {
            std::function<void(traits::first_argument<E>)> f = e;

            otherwise_impl(f);

            if (ex_.has_value())
            {
                reject(std::move(ex_.value()));
            }
        }

        void reject(std::exception_ptr eptr) const
        {
            auto stabilize_ref_count = this->shared_from_this();
            ex_.emplace(eptr);

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
            if (resume_cb_)
            {
                resume_cb_.resume();
            }
            else
#endif            
                if (err_)
                {

                    try
                    {
                        bool handled = err_(eptr);
                        if (!handled)
                        {
                            std::rethrow_exception(eptr);
                        }
                    }
                    catch (...)
                    {
                        throw;
                    }
                }
        }


        // promise coro impl

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
        void suspend(std::experimental::coroutine_handle<> resume_cb)
        {
            this->resume_cb_ = resume_cb;
        }
#endif        

    protected:

        template<class E>
        void otherwise_impl(std::function<void(const E&)> fun)   noexcept
        {
            std::function<bool(std::exception_ptr)> chain = err_;

            err_ = [chain, fun](std::exception_ptr eptr)
            {
                if (chain && chain(eptr))
                {
                    return true;
                }

                if constexpr (std::is_same<E, std::exception_ptr>::value)
                {
                    fun(eptr);
                    return true;
                }
                else
                {
                    try
                    {
                        std::rethrow_exception(eptr);
                    }
                    catch (const std::exception & e)
                    {
                        const E* ex = dynamic_cast<const E*>(&e);
                        if (ex)
                        {
                            fun(*ex);
                            return true;
                        }
                    }
                }

                return false;
            };
        }

        mutable std::optional<std::exception_ptr> ex_;
#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED        
        std::experimental::coroutine_handle<> resume_cb_;
#endif        
        std::function<bool(std::exception_ptr)> err_;
    };

}} // end namespaces

#endif

