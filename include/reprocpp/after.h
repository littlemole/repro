#ifndef DEFINE_MOL_HTTP_SERVER_PROMISE_AFTER_DEF_GUARD_DEFINE_
#define DEFINE_MOL_HTTP_SERVER_PROMISE_AFTER_DEF_GUARD_DEFINE_

#include "reprocpp/promise.h"

/*
 * experimental example of combining promises. header only.
 *
 * provides an API "after" that combines 2-n promises
 * and returns a future to a combined result.
 *
 * public interface is:
 *
 *     template<class T1, class ... Args>
 *     auto after(T1 t1, Args ... args) noexcept;
 *
 * (this file is best groked bottom up)
 *
 */

namespace repro     {

namespace impl    {


template<class T>
struct One;

template<>
struct One<void()>
{
	bool resolved;

	One() noexcept
		: resolved(false)
	{}

	template<class B,class T>
	void resolve1(B& b, T& two) noexcept
	{
		this->resolved = true;
		if ( two.resolved )
		{
			two.resolve2(b);
		}
	}

	template<class B,class Two>
	void resolve1_recursive(B& b,Two& two) noexcept
	{
		this->resolved = true;
	}


	template<class B, class ... Args>
	void resolve2(B& b,Args&& ... args) const noexcept
	{
		b->p.resolve(std::forward<Args>(args)...);
	}

	template<class B, class ... VArgs>
	void resolve2_recursive(B& b,VArgs&& ... args) const noexcept
	{
		b->p.resolve(std::forward<VArgs>(args)...);
	}

	template<class P>
	void hook1(P ptr, Future<> f1) noexcept
	{
		f1.then([ptr]() {
			ptr->one.resolve1(ptr,ptr->two);
		})
		.otherwise([ptr](const std::exception& ex){
			ptr->reject(ex);
		});
	}

	template<class P>
	void hook2(P ptr, Future<> f1) noexcept
	{
		f1.then([ptr]() {
			ptr->two.resolve1(ptr,ptr->one);
		})
		.otherwise([ptr](const std::exception& ex){
			ptr->reject(ex);
		});
	}

	template<class T>
	static auto promise1() noexcept
	{
		return T:: template promise2<>();
	}

	template<class ... VArgs>
	static auto promise2() noexcept
	{
		return promise<VArgs...>();
	}
};


template<class T>
struct First;

template<>
struct First<void()> : public One<void()>
{

};

template<class T>
struct Second;

template<>
struct Second<void()> : public One<void()>
{

};

template<class T, class ... Args>
struct First<void(T,Args...)> : public First<void(Args...)>
{
	typedef First<void(Args...)> Base;

	T value;

	template<class B,class Two>
	void resolve1(B& b,Two& two,T&& t,Args&& ... args ) noexcept
	{
		if ( two.resolved )
		{
			two.resolve2(b,std::forward<T>(t),std::forward<Args>(args)...);
		}
		else {
			resolve1_recursive(b,two,std::forward<T>(t),std::forward<Args>(args)...);
		}
	}

	template<class B,class Two>
	void resolve1_recursive(B& b,Two& two,T&& t, Args&& ... args) noexcept
	{
		value = t;
		Base::resolve1_recursive(b,two,std::forward<Args>(args)...);
	}

	template<class B, class ... VArgs>
	void resolve2(B& b,VArgs&& ... args) const noexcept
	{
		resolve2_recursive(b,std::forward<VArgs>(args)...);
	}

	template<class B, class ... VArgs>
	void resolve2_recursive(B& b,VArgs&& ... args) const noexcept
	{
		Base::resolve2_recursive(b,std::move(value),std::forward<VArgs>(args)...);
	}


	template<class P>
	void hook1(P ptr, Future<T,Args...> f1) noexcept
	{
		f1.then([ptr](T&& t,Args&&... args) {
			ptr->one.resolve1(ptr,ptr->two,std::forward<T>(t),std::forward<Args>(args)...);
		})
		.otherwise([ptr](const std::exception& ex){
			ptr->reject(ex);
		});
	}

	template<class P>
	void hook2(P ptr, Future<T,Args...> f1) noexcept
	{
		f1.then([ptr](T&& t, Args&&...args) {
			ptr->two.resolve1(ptr,ptr->one,std::forward<T>(t),std::forward<Args>(args)...);
		})
		.otherwise([ptr](const std::exception& ex){
			ptr->reject(ex);
		});
	}

	template<class Two>
	static auto promise1() noexcept
	{
		return Two:: template promise2<T,Args...>();
	}

	template<class ... VArgs>
	static Promise<VArgs..., T, Args...> promise2() noexcept
	{
		return promise<VArgs...,T,Args...>();
	}
};


template<class T, class ... Args>
struct Second<void(T,Args...)> : public Second<void(Args...)>
{
	typedef Second<void(Args...)> Base;

	T value;

	template<class B,class Two>
	void resolve1(B& b,Two& two,T&& t,Args&& ... args ) noexcept
	{
		if ( two.resolved )
		{
			two.resolve2(b,std::forward<T>(t),std::forward<Args>(args)...);
		}
		else {
			resolve1_recursive(b,two,std::forward<T>(t),std::forward<Args>(args)...);
		}
	}

	template<class B,class Two>
	void resolve1_recursive(B& b,Two& two,T&& t, Args&& ... args) noexcept
	{
		value = t;
		Base::resolve1_recursive(b,two,std::forward<Args>(args)...);
	}

	template<class B, class ... VArgs>
	void resolve2(B& b,VArgs&& ... args) const noexcept
	{
		resolve2_recursive(b,std::forward<VArgs>(args)...);
	}

	template<class B, class ... VArgs>
	void resolve2_recursive(B& b,VArgs&& ... args) const noexcept
	{
		Base::resolve2_recursive(b,std::forward<VArgs>(args)...,std::move(value));
	}


	template<class P>
	void hook1(P ptr, Future<T,Args...> f1) noexcept
	{
		f1.then([ptr](T&& t,Args&&... args) {
			ptr->one.resolve1(ptr,ptr->two,std::forward<T>(t),std::forward<Args>(args)...);
		})
		.otherwise([ptr](const std::exception& ex){
			ptr->reject(ex);
		});
	}

	template<class P>
	void hook2(P ptr, Future<T,Args...> f1) noexcept
	{
		f1.then([ptr](T&& t, Args&&...args) {
			ptr->two.resolve1(ptr,ptr->one,std::forward<T>(t),std::forward<Args>(args)...);
		})
		.otherwise([ptr](const std::exception& ex){
			ptr->reject(ex);
		});
	}

	template<class Two>
	static auto promise1() noexcept
	{
		return Two:: template promise2<T,Args...>();
	}

	template<class ... VArgs>
	static auto promise2() noexcept
	{
		return promise<VArgs...,T,Args...>();
	}
};


template<class T>
struct BothMixin : public std::enable_shared_from_this<T>
{
	void reject(const std::exception& ex) noexcept
	{
		T* that = static_cast<T*>(this);
		that->p.reject(ex);
	}

	auto future() noexcept 
	{
		T* that = static_cast<T*>(this);
		return that->p.future();
	}

	template<class F1, class F2>
	void hook(F1 f1, F2 f2) noexcept
	{
		T* that = static_cast<T*>(this);
		typename T::Ptr ptr = that->shared_from_this();

		ptr->one.hook1(ptr,f1);
		ptr->two.hook2(ptr,f2);
	}

	static std::shared_ptr<T> create() noexcept
	{
		typename T::Ptr p;
		try
		{
			p = typename T::Ptr(new T);
		}
		catch(...)
		{
			std::terminate();
		}
		return p;
	}
};


template<class T>
struct Both
{
	typedef Promise<> PromiseType;
	Promise<> p;
};


template<class T1,class ...Args>
struct Both<void(First<void(T1)>,Second<void(Args...)>)>
: public BothMixin<Both<void(First<void(T1)>,Second<void(Args...)>)>>
{
	typedef std::shared_ptr<Both<void(First<void(T1)>,Second<void(Args...)>)>> Ptr;

	First<void(T1)> one;
	Second<void(Args...)> two;

	typedef Promise<T1,Args...> PromiseType;
	PromiseType p;

	Both() noexcept
		: p(First<void(T1)>:: template promise1<Second<void(Args...)>>())
	{}
};


template<class ...Args>
struct Both<void(First<void()>,Second<void(Args...)>)>
: public BothMixin<Both<void(First<void()>,Second<void(Args...)>)>>
{
	typedef std::shared_ptr<Both<void(First<void()>,Second<void(Args...)>)>> Ptr;

	First<void()> one;
	Second<void(Args...)> two;

	typedef Promise<Args...> PromiseType;
	Promise<Args...> p;

	Both() noexcept
		: p(First<void()>:: template promise1<Second<void(Args...)>>())
	{}
};

} // end namespace impl

template<class ...Args>
Future<Args...> both( Future<> f1, Future<Args...> f2) noexcept
{
	auto b = impl::Both<void(impl::First<void()>,impl::Second<void(Args...)>)>::create();

	b->hook(f1,f2);

	return b->p.future();
}


template<class T1, class ...Args>
Future<T1,Args...> both( Future<T1> f1, Future<Args...> f2) noexcept
{
	auto b = impl::Both<void(impl::First<void(T1)>,impl::Second<void(Args...)>)>::create();

	b->hook(f1,f2);

	return b->p.future();
}


template<class T1, class T2>
auto after(T1 t1, T2 t2) noexcept
{
	return both(t1,t2);
}

template<class T1, class ... Args>
auto after(T1 t1, Args ... args) noexcept
{
	return both( t1, after(args...) );
}


} // end namespace repro


#endif


