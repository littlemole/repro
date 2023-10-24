#ifndef DEFINE_MOL_HTTP_SERVER_PROMISE_AFTER_DEF_GUARD_DEFINE_
#define DEFINE_MOL_HTTP_SERVER_PROMISE_AFTER_DEF_GUARD_DEFINE_

#include "reprocpp/promise.h"


namespace repro {
namespace impl {

template<class T, int I , int O = 0, class E = void>
class Assignor;

template<class T, int I, int O>
class Assignor<T, I, O, typename std::enable_if<(I>=0 && I < std::tuple_size<T>::value)>::type>
{
public:
	template<class ... Args>
	static void assign(T& values, std::tuple<Args...>& result)
	{
		std::get<I>(values) = std::get<O>(result);

		Assignor<T, I + 1, O + 1>::assign(values, result);
	}
};

template<class T, int I, int O>
class Assignor<T, I, O, typename std::enable_if<!(I>=0 && I<std::tuple_size<T>::value)>::type>
{
public:

	template<class ... Args>
	static void assign(T& values, std::tuple<Args...> result)
	{
		(void)values;
		(void)result;
	}
};

template<class ... Args>
struct BothCtx
{
	typedef std::shared_ptr<BothCtx<Args...>> Ptr;
	typedef std::tuple<Args...> combined_t;

	bool rejected = false;
	int outstanding = 2;
	combined_t values;

	template<class P>
	void resolve(P p)
	{
		if(rejected)
			return;
		
		outstanding--;
		if(outstanding==0)
		{
			p.resolve(values);
		}
	}

	template<class P>
	void reject(P p, const std::exception_ptr& ex)
	{
		if(rejected)
		{
			return;
		}

		rejected = true;
		p.reject(ex);
	}		
};

template<class T>
class Both;

template<>
class Both<void()>
{
public:

	typedef std::tuple<> combined_t;

	auto future(Future<> f1, Future<> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<>>();

		f1.then([p, ctx]() { resolve(p,ctx); });
		f1.otherwise([p,ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx]() { resolve(p, ctx); });
		f2.otherwise([p,ctx](const std::exception_ptr& ex) { reject(p,ctx,ex); });

		return p.future();
	}

	auto future(Future<> f1, Future<std::tuple<>> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<>>();

		f1.then([p, ctx]() { resolve(p, ctx); });
		f1.otherwise([p,ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx](std::tuple<>) { resolve(p, ctx); });
		f2.otherwise([p,ctx](const std::exception_ptr& ex) { reject(p,ctx,ex); });

		return p.future();
	}

private:

	template<class P>
	static void resolve(P p, BothCtx<>::Ptr ctx)
	{
		ctx->resolve(p);
	}

	template<class P>
	static void reject(P p, BothCtx<>::Ptr ctx, const std::exception_ptr& ex)
	{
		ctx->reject(p,ex);
	}		
};



template<class F>
class Both<void(F)>
{
public:

	typedef std::tuple<F> combined_t;

	auto future(Future<> f1, Future<F> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<F>>();

		f1.then([p, ctx]() { resolve(p, ctx); });
		f1.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx](F result) { resolve(p, ctx, result); });
		f2.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		return p.future();
	}

	auto future(Future<> f1, Future<std::tuple<F>> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<F>>();

		f1.then([p, ctx]() { resolve(p, ctx); });
		f1.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx](std::tuple<F> result) { resolve(p, ctx, result); });
		f2.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		return p.future();
	}

	auto future(Future<F> f1, Future<> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<F>>();

		f1.then([p, ctx](F result) { resolve(p, ctx, result); });
		f1.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx]() { resolve(p, ctx); });
		f2.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		return p.future();
	}

	auto future(Future<F> f1, Future<std::tuple<>> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<F>>();

		f1.then([p, ctx](F result) { resolve(p, ctx, result); });
		f1.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx](std::tuple<>) { resolve(p, ctx); });
		f2.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		return p.future();
	}

private:

	template<class P, class T>
	static void resolve(P p,typename BothCtx<F>::Ptr ctx, T t)
	{
		std::get<0>(ctx->values) = t;
		ctx->resolve(p);
	}

	template<class P, class T>
	static void resolve(P p, typename BothCtx<F>::Ptr ctx, std::tuple<T> t)
	{
		std::get<0>(ctx->values) = std::get<0>(t);
		ctx->resolve(p);
	}

	template<class P>
	static void resolve(P p, typename BothCtx<F>::Ptr ctx)
	{
		ctx->resolve(p);
	}

	template<class P>
	static void reject(P p, typename BothCtx<F>::Ptr ctx, const std::exception_ptr& ex)
	{
		ctx->reject(p,ex);
	}		
};




template<class T, class ... Args>
class Both<void(T, Args...)>
{
public:
	typedef std::tuple<T, Args...> combined_t;


	auto future(Future<> f1, Future<std::tuple<T, Args...>> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<T,Args...>>();

		f1.then([p, ctx]()
		{
			resolve(p, ctx);
		});
		f1.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx](std::tuple<T, Args...> result)
		{
			resolve(p, ctx, result);
		});
		f2.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		return p.future();
	}

	auto future(Future<T> f1, Future<Args...> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<T,Args...>>();

		f1.then([p, ctx]( T result)
		{
			resolve<0>(p, ctx,result);
		});
		f1.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx](Args... result)
		{
			resolve<1>(p, ctx, result...);
		});
		f2.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		return p.future();
	}

	auto future(Future<T> f1, Future<std::tuple<Args...>> f2)
	{
		auto p = promise<combined_t>();

		auto ctx = std::make_shared<BothCtx<T,Args...>>();

		f1.then([p, ctx]( T result)
		{
			resolve<0>(p, ctx,result);
		});
		f1.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		f2.then([p, ctx](std::tuple<Args...> result)
		{
			resolve(p, ctx, result);
		});
		f2.otherwise([p, ctx](const std::exception_ptr& ex) {reject(p,ctx,ex); });

		return p.future();
	}		

private:

	template<class P>
	static void resolve(P p, typename BothCtx<T,Args...>::Ptr ctx)
	{
		ctx->resolve(p);			
	}

	template<int I>
	static void resolve(Promise<combined_t> p, typename BothCtx<T,Args...>::Ptr ctx, T result)
	{
		std::get<I>(ctx->values) = result;
		ctx->resolve(p);
	}


	template<int I,class P>
	static void resolve(Promise<combined_t> p, typename BothCtx<T,Args...>::Ptr ctx, P result)
	{
		std::get<I>(ctx->values) = result;
		ctx->resolve(p);
	}


	static void resolve(Promise<combined_t> p, typename BothCtx<T,Args...>::Ptr ctx, std::tuple<T, Args...> result)
	{
		Assignor<combined_t,0>::assign(ctx->values, result);
		ctx->resolve(p);
	}

	static void resolve(Promise<combined_t> p, typename BothCtx<T,Args...>::Ptr ctx, std::tuple<Args...> result)
	{
		Assignor<combined_t,1>::assign(ctx->values, result);
		ctx->resolve(p);
	}

	template<class P>
	static void reject(P p, typename BothCtx<T,Args...>::Ptr ctx, const std::exception_ptr& ex)
	{
		ctx->reject(p,ex);
	}	
};

} // end namespace impl

// boths with no results
inline auto both(Future<> f1, Future<> f2)
{
	repro::impl::Both<void()> b;
	return b.future(f1, f2);
}

inline auto both(Future<> f1, Future<std::tuple<>> f2)
{
	repro::impl::Both<void()> b;
	return b.future(f1, f2);
}

// boths with one result
template<class T>
auto both(Future<> f1, Future<T> f2)
{
	repro::impl::Both<void(T)> b;
	return b.future(f1, f2);
}

template<class T>
auto both(Future<T> f1, Future<> f2)
{
	repro::impl::Both<void(T)> b;
	return b.future(f1, f2);
}


template<class T>
auto both(Future<T> f1, Future<std::tuple<>> f2)
{
	repro::impl::Both<void(T)> b;
	return b.future(f1, f2);
}

// boths with nresults > 1

template<class T, class ... Args>
auto both(Future<T> f1, Future<Args...> f2)
{
	repro::impl::Both<void(T, Args...)> b;
	return b.future(f1, f2);
}	

template<class T, class ... Args>
auto both(Future<T> f1, Future<std::tuple<Args...>> f2)
{
	repro::impl::Both<void(T, Args...)> b;
	return b.future(f1, f2);
}

template<class ... Args>
auto both(Future<> f1, Future<std::tuple<Args...>> f2)
{
	repro::impl::Both<void(Args...)> b;
	return b.future(f1, f2);
}


// high level interface

template<class T1, class T2>
auto after(T1 t1, T2 t2) noexcept
{
	return both(t1, t2);
}

template<class T1, class ... Args>
auto after(T1 t1, Args ... args) noexcept
{
	return both(t1, after(args...));
}



} // end namespace repro


#endif


