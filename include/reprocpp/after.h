#ifndef DEFINE_MOL_HTTP_SERVER_PROMISE_AFTER_DEF_GUARD_DEFINE_
#define DEFINE_MOL_HTTP_SERVER_PROMISE_AFTER_DEF_GUARD_DEFINE_

#include "reprocpp/promise.h"


namespace repro     {

	template<class T>
	class Both;

	template<>
	class Both<void()>
	{
	public:

		auto future(Future<> f1, Future<> f2)
		{
			auto p = promise<>();

			auto outstanding = std::make_shared<int>(2);

			f1.then([p, outstanding]() { resolve(p, outstanding); });
			f1.otherwise([p](const std::exception& ex) {p.reject(ex); });

			f2.then([p, outstanding]() { resolve(p, outstanding); });
			f2.otherwise([p](const std::exception& ex) {p.reject(ex); });

			return p.future();
		}

	private:

		template<class P>
		static void resolve(P p, std::shared_ptr<int> outstanding)
		{
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve();
			}
		}
	};



	template<class F>
	class Both<void(F)>
	{
	public:

		auto future(Future<> f1, Future<F> f2)
		{
			auto p = promise<F>();

			auto value = std::make_shared<F>();
			auto outstanding = std::make_shared<int>(2);

			f1.then([p, value, outstanding]() { resolve(p, value, outstanding); });
			f1.otherwise([p](const std::exception& ex) {p.reject(ex); });

			f2.then([p, value, outstanding](F result) { resolve(p, value, outstanding, result); });
			f2.otherwise([p](const std::exception& ex) {p.reject(ex); });

			return p.future();
		}

		auto future(Future<F> f1, Future<> f2)
		{
			auto p = promise<F>();

			auto value = std::make_shared<F>();
			auto outstanding = std::make_shared<int>(2);

			f1.then([p, value, outstanding](F result) { resolve(p, value, outstanding, result); });
			f1.otherwise([p](const std::exception& ex) {p.reject(ex); });

			f2.then([p, value, outstanding]() { resolve(p, value, outstanding); });
			f2.otherwise([p](const std::exception& ex) {p.reject(ex); });

			return p.future();
		}

	private:

		template<class P, class T>
		static void resolve(P p, std::shared_ptr<F> value, std::shared_ptr<int> outstanding, T t)
		{
			*value = t;
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*value);
			}
		}

		template<class P>
		static void resolve(P p, std::shared_ptr<F> value, std::shared_ptr<int> outstanding)
		{
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*value);
			}
		}
	};



	template<class T1, class T2>
	class Both<void(T1, T2)>
	{
	public:
		typedef std::tuple<T1, T2> combined_t;

		auto future(Future<T1> f1, Future<T2> f2)
		{
			auto p = promise<combined_t>();

			auto values = std::make_shared<combined_t>();
			auto outstanding = std::make_shared<int>(2);

			f1.then([p, values, outstanding](T1 result)
			{
				resolve<0>(p, values, outstanding, result);
			});
			f1.otherwise([p](const std::exception& ex) {p.reject(ex); });

			f2.then([p, values, outstanding](T2 result)
			{
				resolve<1>(p, values, outstanding, result);
			});
			f2.otherwise([p](const std::exception& ex) {p.reject(ex); });

			return p.future();
		}

	private:

		template<int I, class P, class T>
		static void resolve(P p, std::shared_ptr<combined_t> values,
			std::shared_ptr<int> outstanding, T t)
		{
			std::get<I>(*values) = t;
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}
	};

	template<class T, int I = 1, class E = void>
	class Assignor;

	template<class T, int I>
	class Assignor<T, I, typename std::enable_if<(I < std::tuple_size<T>::value)>::type>
	{
	public:
		template<class ... Args>
		static void assign(std::shared_ptr<T>& values, std::tuple<Args...>& result)
		{
			std::get<I>(*values) = std::get<I - 1>(result);

			Assignor<T, I + 1>::assign(values, result);
		}
	};

	template<class T, int I>
	class Assignor<T, I, typename std::enable_if<!(I<std::tuple_size<T>::value)>::type>
	{
	public:

		template<class ... Args>
		static void assign(std::shared_ptr<T> values, std::tuple<Args...> result)
		{
		}
	};


	template<class T1, class T2, class ... Args>
	class Both<void(T1, std::tuple<T2, Args...>)>
	{
	public:
		typedef std::tuple<T1, T2, Args...> combined_t;

		auto future(Future<T1> f1, Future<std::tuple<T2, Args...>> f2)
		{
			auto p = promise<combined_t>();

			auto values = std::make_shared<combined_t>();

			auto outstanding = std::make_shared<int>(2);

			f1.then([p, values, outstanding](T1 result)
			{
				resolve(p, values, outstanding, result);
			});
			f1.otherwise([p](const std::exception& ex) {p.reject(ex); });

			f2.then([p, values, outstanding](std::tuple<T2, Args...> result)
			{
				resolve(p, values, outstanding, result);
			});
			f2.otherwise([p](const std::exception& ex) {p.reject(ex); });

			return p.future();
		}

	private:

		template<class P>
		static void resolve(P p, std::shared_ptr<combined_t> values,
			std::shared_ptr<int> outstanding, T1 t)
		{
			std::get<0>(*values) = t;
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}

		static void resolve(Promise<combined_t> p, std::shared_ptr<combined_t> values,
			std::shared_ptr<int> outstanding, std::tuple<T2, Args...> result)
		{
			Assignor<combined_t>::assign(values, result);
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}

	};


	inline auto both(Future<> f1, Future<> f2)
	{
		Both<void()> b;
		return b.future(f1, f2);
	}


	template<class T>
	auto both(Future<> f1, Future<T> f2)
	{
		Both<void(T)> b;
		return b.future(f1, f2);
	}


	template<class T>
	auto both(Future<T> f1, Future<> f2)
	{
		Both<void(T)> b;
		return b.future(f1, f2);
	}


	template<class T1, class T2>
	auto both(Future<T1> f1, Future<T2> f2)
	{
		Both<void(T1, T2)> b;
		return b.future(f1, f2);
	}


	template<class T1, class T2, class ... Args>
	auto both(Future<T1> f1, Future<std::tuple<T2, Args...>> f2)
	{
		Both<void(T1, std::tuple<T2, Args...>)> b;
		return b.future(f1, f2);
	}

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


