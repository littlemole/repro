#include <memory>
#include <list>
#include <utility>
#include <iostream>
#include <string>
#include <exception>
#include <functional>
#include <atomic>

#include "reprocpp/promise.h" 
#include "gtest/gtest.h"
#include "reprocpp/test.h"

using namespace repro;


class Task
{
public:

	typedef std::shared_ptr<Task> Ptr;

	virtual ~Task() {};

	virtual void execute() = 0;
};


class TaskImpl : public Task
{
public:
	TaskImpl(std::function<void()> f): f_(f) {}

	virtual void execute() {
		f_();
	}

private:
	std::function<void()> f_;
};

template<class P,class F>
Task* taskimpl_void(P p,F f)
{
	return new TaskImpl( [p,f]()
	{
		try
		{
			f();
		}
		catch (...)
		{
			p.reject(std::current_exception());
			return;
		}
		p.resolve();
	});
}

template<class P,class F>
Task* taskimpl_r(P p,F f)
{
	return new TaskImpl( [p,f]()
	{
		try
		{
			p.resolve(f());
		}
		catch (...)
		{
			p.reject(std::current_exception());
		}
	});
}

class Loop
{
public:

	void run()
	{

		while(!tasks_.empty()) {
			Task::Ptr t = *(tasks_.begin());
			tasks_.pop_front();
			t->execute();
		}
	}

	template<class F,typename std::enable_if<ReturnsVoid<F>::value>::type* = nullptr>
	Future<> task(F f)
	{
		auto p = promise<>();
		Task::Ptr ptr(taskimpl_void(p,f));
		tasks_.push_back(ptr);
		return p.future();
	}


	template<class F,typename std::enable_if<!ReturnsVoid<F>::value>::type* = nullptr>
	auto task(F f) -> Future<decltype(f())>
	{
		auto p = promise<decltype(f())>();
		Task::Ptr ptr(taskimpl_r(p,f));
		tasks_.push_back(ptr);
		return p.future();
	}


	template<class F,typename std::enable_if<!ReturnsVoid<F>::value>::type* = nullptr>
	auto task2(F f) -> Future<decltype(f())>
	{
		auto p = promise<decltype(f())>();
		Task::Ptr ptr(taskimpl_r(p,f));
		tasks_.push_front(ptr);
		return p.future();
	}

	Future<> nextTick()
	{
		auto p = promise<>();

		task([p]() 
		{
			p.resolve();
		});
		
		return p.future();
	}

private:
	std::list<Task::Ptr> tasks_;
};



namespace mol {


void nextTick( std::function<void()> f) noexcept
{
}




void timeout( std::function<void()> f,int secs, int ms) noexcept
{
	
}

}

int main(int argc, char **argv) {


	::testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();

	return r;
}


