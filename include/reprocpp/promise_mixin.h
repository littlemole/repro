#ifndef MOL_DEF_GUARD_DEFINE_REPRO_CPP_MIXIN_DEF_GUARD_
#define MOL_DEF_GUARD_DEFINE_REPRO_CPP_MIXIN_DEF_GUARD_

#include <optional>
#include <atomic>

#ifdef _WIN32
#include <experimental/resumable>
#else
#ifdef __clang__
#include "reprocpp/coroutine.h"
//#include <experimental/coroutine>
#else
#include <coroutine>
#endif
#endif


#include "reprocpp/traits.h"
#include "reprocpp/debug.h"

namespace repro {

template<class ...Args>
class Future;

template<class ...Args>
class Promise;

template<class E>
void otherwise_chain(std::function<bool(std::exception_ptr)>& err, E&& fun) 
{
    std::function<bool(std::exception_ptr)> chain = err;

    using ET = std::remove_const_t<std::remove_reference_t<repro::traits::first_argument<E>>>;

    err = [chain, fun](std::exception_ptr eptr)
    {
        if constexpr (std::is_same<ET, std::exception_ptr>::value)
        {
            fun(eptr);
            return true;
        }
        else
        {
            try
            {
                if (chain && chain(eptr) )
                {
                    return true;
                }

                std::rethrow_exception(eptr);
            }
            catch (const std::exception & e)
            {
                const ET* ex = dynamic_cast<const ET*>(&e);
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

            otherwise_chain(err_,f);

            if (ex_.has_value())
            {
                reject(std::move(ex_.value()));
            }
        }

        void reject(const std::exception_ptr& eptr) 
        {
            auto stabilize_ref_count = this->shared_from_this();
            ex_.emplace(eptr);

            if (resume_cb_)
            {
                resume_cb_.resume();
            }
            else
                if (err_)
                {

                    try
                    {
                        err_(eptr);
                    }
                    catch (...)
                    {
                        throw;
                    }
                }
        }


        // promise coro impl

        void suspend(std::coroutine_handle<> resume_cb)
        {
            this->resume_cb_ = resume_cb;
        }

    protected:

        std::optional<std::exception_ptr> ex_;
        std::coroutine_handle<> resume_cb_;
        std::function<bool(std::exception_ptr)> err_;
    };

}} // end namespaces

#endif

