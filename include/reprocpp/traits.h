#ifndef MOL_DEF_GUARD_DEFINE_REPRO_CPP_TRAITS_DEF_GUARD_
#define MOL_DEF_GUARD_DEFINE_REPRO_CPP_TRAITS_DEF_GUARD_

#include <memory>
#include <functional>

namespace repro {
namespace traits
{

    template<class F, class ... Args>
    struct returns_void
    {
        using result_type = typename std::invoke_result<F,Args ...>::type;
        static const bool value = std::is_void<result_type>::value;
    };

    namespace impl
    {

        template<typename Ret, typename Arg, typename... Rest>
        Arg first_argument_helper(Ret(*) (Arg, Rest...));

        template<typename Ret, typename F, typename Arg, typename... Rest>
        Arg first_argument_helper(Ret(F::*) (Arg, Rest...));

        template<typename Ret, typename F, typename Arg, typename... Rest>
        Arg first_argument_helper(Ret(F::*) (Arg, Rest...) const);

        template <typename F>
        decltype(first_argument_helper(&F::operator())) first_argument_helper(F);
    }

    template <typename T>
    using first_argument = decltype(repro::impl::first_argument_helper(std::declval<T>()));

}} // end namespaces

#endif

