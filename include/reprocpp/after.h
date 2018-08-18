#ifndef DEFINE_MOL_HTTP_SERVER_PROMISE_AFTER_DEF_GUARD_DEFINE_
#define DEFINE_MOL_HTTP_SERVER_PROMISE_AFTER_DEF_GUARD_DEFINE_

#include "reprocpp/promise.h"


namespace repro     {


	template<class T, int I = 1, int O = 0, class E = void>
	class Assignor;

	template<class T, int I, int O>
	class Assignor<T, I, O, typename std::enable_if<(I>=0 && I < std::tuple_size<T>::value)>::type>
	{
	public:
		template<class ... Args>
		static void assign(std::shared_ptr<T>& values, std::tuple<Args...>& result)
		{
			std::get<I>(*values) = std::get<I - 1 + O>(result);

			Assignor<T, I + 1, O>::assign(values, result);
		}
	};

	template<class T, int I, int O>
	class Assignor<T, I, O, typename std::enable_if<!(I>=0 && I<std::tuple_size<T>::value)>::type>
	{
	public:

		template<class ... Args>
		static void assign(std::shared_ptr<T> values, std::tuple<Args...> result)
		{
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

			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, outstanding]() { resolve(p, rejected, outstanding); });
			f1.otherwise([p,rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, outstanding]() { resolve(p, rejected, outstanding); });
			f2.otherwise([p,rejected](const std::exception& ex) { reject(p,rejected,ex); });

			return p.future();
		}

		auto future(Future<> f1, Future<std::tuple<>> f2)
		{
			auto p = promise<combined_t>();

			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, outstanding]() { resolve(p, rejected, outstanding); });
			f1.otherwise([p,rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, outstanding](std::tuple<>) { resolve(p, rejected, outstanding); });
			f2.otherwise([p,rejected](const std::exception& ex) { reject(p,rejected,ex); });

			return p.future();
		}

	private:

		template<class P>
		static void resolve(P p, std::shared_ptr<bool> rejected, std::shared_ptr<int> outstanding)
		{
			if(*rejected)
			{
				return;
			}

			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(combined_t());
			}
		}

		template<class P>
		static void reject(P p, std::shared_ptr<bool> rejected, const std::exception& ex)
		{
			if(*rejected)
			{
				return;
			}

			*rejected = true;

			p.reject(ex);
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

			auto values = std::make_shared<combined_t>();
			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, values, outstanding]() { resolve(p, rejected, values, outstanding); });
			f1.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, values, outstanding](F result) { resolve(p, rejected, values, outstanding, result); });
			f2.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			return p.future();
		}

		auto future(Future<> f1, Future<std::tuple<F>> f2)
		{
			auto p = promise<combined_t>();

			auto values = std::make_shared<combined_t>();
			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, values, outstanding]() { resolve(p, rejected, values, outstanding); });
			f1.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, values, outstanding](std::tuple<F> result) { resolve(p, rejected, values, outstanding, result); });
			f2.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			return p.future();
		}

		auto future(Future<F> f1, Future<> f2)
		{
			auto p = promise<combined_t>();

			auto values = std::make_shared<combined_t>();
			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, values, outstanding](F result) { resolve(p, rejected, values, outstanding, result); });
			f1.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, values, outstanding]() { resolve(p, rejected, values, outstanding); });
			f2.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			return p.future();
		}

		auto future(Future<F> f1, Future<std::tuple<>> f2)
		{
			auto p = promise<combined_t>();

			auto values = std::make_shared<combined_t>();
			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, values, outstanding](F result) { resolve(p, rejected, values, outstanding, result); });
			f1.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, values, outstanding](std::tuple<>) { resolve(p, rejected, values, outstanding); });
			f2.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			return p.future();
		}

	private:

		template<class P, class T>
		static void resolve(P p, std::shared_ptr<bool> rejected, std::shared_ptr<combined_t> values, std::shared_ptr<int> outstanding, T t)
		{
			if(*rejected)
			{
				return;
			}

			std::get<0>(*values) = t;
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}

		template<class P, class T>
		static void resolve(P p, std::shared_ptr<bool> rejected, std::shared_ptr<combined_t> values, std::shared_ptr<int> outstanding, std::tuple<T> t)
		{
			if(*rejected)
			{
				return;
			}

			std::get<0>(*values) = std::get<0>(t);
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}

		template<class P>
		static void resolve(P p, std::shared_ptr<bool> rejected, std::shared_ptr<combined_t> values, std::shared_ptr<int> outstanding)
		{
			if(*rejected)
			{
				return;
			}

			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}

		template<class P>
		static void reject(P p, std::shared_ptr<bool> rejected, const std::exception& ex)
		{
			if(*rejected)
			{
				return;
			}

			*rejected = true;
			p.reject(ex);
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

			auto values = std::make_shared<combined_t>();
			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, values, outstanding]()
			{
				resolve(p, rejected, values, outstanding);
			});
			f1.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, values, outstanding](std::tuple<T, Args...> result)
			{
				resolve(p, rejected, values, outstanding, result);
			});
			f2.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			return p.future();
		}

		auto future(Future<T> f1, Future<Args...> f2)
		{
			auto p = promise<combined_t>();

			auto values = std::make_shared<combined_t>();
			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, values, outstanding]( T result)
			{
				resolve<0>(p, rejected, values, outstanding,result);
			});
			f1.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, values, outstanding](Args... result)
			{
				resolve<1>(p, rejected, values, outstanding, result...);
			});
			f2.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			return p.future();
		}

		auto future(Future<T> f1, Future<std::tuple<Args...>> f2)
		{
			auto p = promise<combined_t>();

			auto values = std::make_shared<combined_t>();
			auto outstanding = std::make_shared<int>(2);
			auto rejected = std::make_shared<bool>(false);

			f1.then([p, rejected, values, outstanding]( T result)
			{
				resolve<0>(p, rejected, values, outstanding,result);
			});
			f1.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			f2.then([p, rejected, values, outstanding](std::tuple<Args...> result)
			{
				resolve(p, rejected, values, outstanding, result);
			});
			f2.otherwise([p, rejected](const std::exception& ex) {reject(p,rejected,ex); });

			return p.future();
		}		

	private:

		template<class P>
		static void resolve(P p, std::shared_ptr<bool> rejected, std::shared_ptr<combined_t> values,
			std::shared_ptr<int> outstanding)
		{
			if(*rejected)
			{
				return;
			}
			
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}

		template<int I>
		static void resolve(Promise<combined_t> p, std::shared_ptr<bool> rejected, std::shared_ptr<combined_t> values,
			std::shared_ptr<int> outstanding, T result)
		{
			if(*rejected)
			{
				return;
			}

			std::get<I>(*values) = result;

			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}


		template<int I,class P>
		static void resolve(Promise<combined_t> p, std::shared_ptr<bool> rejected, std::shared_ptr<combined_t> values,
			std::shared_ptr<int> outstanding, P result)
		{
			if(*rejected)
			{
				return;
			}

			std::get<I>(*values) = result;

			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}


		static void resolve(Promise<combined_t> p, std::shared_ptr<bool> rejected, std::shared_ptr<combined_t> values,
			std::shared_ptr<int> outstanding, std::tuple<T, Args...> result)
		{
			if(*rejected)
			{
				return;
			}

			Assignor<combined_t,0,1>::assign(values, result);
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}

		static void resolve(Promise<combined_t> p, std::shared_ptr<bool> rejected, std::shared_ptr<combined_t> values,
			std::shared_ptr<int> outstanding, std::tuple<Args...> result)
		{
			if(*rejected)
			{
				return;
			}

			Assignor<combined_t,1,0>::assign(values, result);
			(*outstanding)--;
			if (*outstanding == 0)
			{
				p.resolve(*values);
			}
		}

		template<class P>
		static void reject(P p, std::shared_ptr<bool> rejected, const std::exception& ex)
		{
			if(*rejected)
			{
				return;
			}

			*rejected = true;
			p.reject(ex);
		}	
	};


	inline auto both(Future<> f1, Future<> f2)
	{
		Both<void()> b;
		return b.future(f1, f2);
	}

	inline auto both(Future<> f1, Future<std::tuple<>> f2)
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



	template<class T>
	auto both(Future<T> f1, Future<std::tuple<>> f2)
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
		Both<void(T1, T2, Args...)> b;
		return b.future(f1, f2);
	}

	template<class ... Args>
	auto both(Future<> f1, Future<std::tuple<Args...>> f2)
	{
		Both<void(Args...)> b;
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


