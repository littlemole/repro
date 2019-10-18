#ifndef DEFINE_MOL_PROMISE_TRAITS_DEF_GUARD_DEFINE_
#define DEFINE_MOL_PROMISE_TRAITS_DEF_GUARD_DEFINE_

#include "reprocpp/ex.h"
#include "reprocpp/debug.h"
#include <memory>
#include <functional>

/*
 * traits used for promise impls
 */

namespace repro     {


// forwards

template<class ... Args>
class Future;

template<class ... Args>
class Promise;

template<class ... Args>
class PromiseState;

template<class ... Args>
class PromiseStateHolder;

template<size_t S>
class MemPool;

template<size_t S>
MemPool<S>& mempool();

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

// first argument helper courtesy https://stackoverflow.com/a/35348334

template<typename Ret, typename Arg, typename... Rest>
Arg first_argument_helper(Ret(*) (Arg, Rest...));

template<typename Ret, typename F, typename Arg, typename... Rest>
Arg first_argument_helper(Ret(F::*) (Arg, Rest...));

template<typename Ret, typename F, typename Arg, typename... Rest>
Arg first_argument_helper(Ret(F::*) (Arg, Rest...) const);

template <typename F>
decltype(first_argument_helper(&F::operator())) first_argument_helper(F);

template <typename T>
using first_argument = decltype(first_argument_helper(std::declval<T>()));


} // end namespace org


#endif


