#include "main.h"
#include "reprocpp/ex.h"

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


class BasicTest : public ::testing::Test {
 protected:


	  virtual void SetUp() {
		  MOL_TEST_PRINT_CNTS();
	  }

	  virtual void TearDown() {
		  MOL_TEST_PRINT_CNTS();
	  }


}; // end test setup

class Aex : public ReproEx<Aex> {};
class Bex : public Aex {};

TEST_F(BasicTest, MultiCatch) 
{

	Loop loop;
	int c = 0;
	std::type_index t(typeid(std::exception));
	std::string e;

	loop.task([&c]() {
		c++;
		MOL_TEST_PRINT_CNTS();
			
	})   
	.then([&c,&loop]() {

		return loop.task([&c]()
		{
			c++;
			MOL_TEST_PRINT_CNTS();
			throw Bex();
		});
	})  
	.then([&c,&loop]() {

		return loop.task([&c]()
		{
			c++; 
			MOL_TEST_PRINT_CNTS();
		});
	})		
	.then([&c]() {

		EXPECT_EQ(2, c);
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.otherwise(
		[&t,&e](const Bex& ex) 
		{
			std::cout << typeid(ex).name() << std::endl;
			t = std::type_index(typeid(ex));
			e = "somex";
		},
		[&t,&e](const Aex& ex) 
		{
			std::cout << typeid(ex).name() << std::endl;
			t = std::type_index(typeid(ex));
			e = "someotherxx";
		},
		[](const std::exception& ex) 
		{
			std::cout << typeid(ex).name() << std::endl;
			std::cout << "std::ex " << typeid(ex).name() << std::endl;
			// not called
		}
	);

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(2, c);
	EXPECT_EQ(std::type_index(typeid(Bex)), t);
	EXPECT_EQ("somex", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

 
TEST_F(BasicTest, ThenableNewThrows) {

	Loop loop;
	int c = 0;
	std::type_index t(typeid(std::exception));
	std::string e;

	loop.task([&c]() {
		throw Ex("HeloEx");
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.then([&c]() {
		EXPECT_EQ(1, c);
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.otherwise([&t,&e](const Ex& ex) 
		{
			t = std::type_index(typeid(ex));
			e = ex.what();
		},
		[](const std::exception& ) 
		{
			// fail
		}
	);

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(0, c);
	EXPECT_EQ(std::type_index(typeid(Ex)), t);
	EXPECT_EQ("HeloEx", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

 
TEST_F(BasicTest, ThenableNewThrowsStdEx) {

	Loop loop;
	int c = 0;
	std::type_index t(typeid(std::exception));
	std::string e;

	loop.task([&c]() {
		throw std::exception();
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.then([&c]() {
		EXPECT_EQ(1, c);
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.otherwise([](const Ex& ex) 
		{
			(void)ex;
			// not called
		},
		[&t,&e](const std::exception& ex) 
		{
			t = std::type_index(typeid(ex));
			e = "std::ex";
		}
	);

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(0, c);
	EXPECT_EQ(std::type_index(typeid(std::exception)), t);
	EXPECT_EQ("std::ex", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

class SomeEx : public std::exception {};

TEST_F(BasicTest, ThenableNewThrowsSomeEx) {

	Loop loop;
	int c = 0;
	std::type_index t(typeid(std::exception));
	std::string e;

	loop.task([&c]() {
		throw SomeEx();
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.then([&c]() {
		EXPECT_EQ(1, c);
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.otherwise([](const Ex& ex) 
		{
			(void)ex;
			// not called
		},
		[&t,&e](const SomeEx& ex) 
		{
			t = std::type_index(typeid(ex));
			e = "somex";
		},
		[](const std::exception& ex) 
		{
			(void)ex;
			// not called
		}
	);

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(0, c);
	EXPECT_EQ(std::type_index(typeid(SomeEx)), t);
	EXPECT_EQ("somex", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}
 
TEST_F(BasicTest, ThenableThrows) {

	Loop loop;
	int c = 0;
	std::type_index t(typeid(std::exception));
	std::string e;

	loop.task([&c]() {
		throw Ex("HeloEx");
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.then([&c]() {
		EXPECT_EQ(1, c);
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.otherwise([&t,&e](const std::exception& ex) 
	{
		t = std::type_index(typeid(ex));
		e = ex.what();
	});

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(0, c);
	EXPECT_EQ(std::type_index(typeid(Ex)), t);
	EXPECT_EQ("HeloEx", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

TEST_F(BasicTest, ThenableChainedThrows) {

	Loop loop;
	int c = 0;
	std::type_index t(typeid(std::exception));
	std::string e;

	loop.task([&c]() {		
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.then([&c,&loop]()  
	{
		EXPECT_EQ(1, c);
		c++;
		MOL_TEST_PRINT_CNTS();
		return loop.task([&c]() {
			c++;
			return c;
		});
	})
	.then([&c](int i) {
		EXPECT_EQ(c, 3);
		EXPECT_EQ(i, 3);
		throw Ex("HeloEx");
	})
	.otherwise([&t, &e](const std::exception& ex)
	{
		t = std::type_index(typeid(ex));
		e = ex.what();
	});

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(3, c);
	EXPECT_EQ(std::type_index(typeid(Ex)), t);
	EXPECT_EQ("HeloEx", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}



class MyEx : public ReproEx<MyEx> {};

Future<> coroReturnNoAsync(Loop&, std::string& e)
{
	e = "test";
	co_return;
}

TEST_F(BasicTest, CoroReturnNoAsync) {

	Loop loop;
	std::string e;

	coroReturnNoAsync(loop, e);

	loop.run();

	EXPECT_EQ("test", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}


Future<> thenableThrowsCoroMyEx(Loop& loop,std::type_index& t, std::string& e)
{
	try
	{
		co_await loop.task([]() 
		{
			throw MyEx();
		});
	}
	catch (const std::exception& ex)
	{
		t = std::type_index(typeid(ex));
		e = "myex";
	};
}

TEST_F(BasicTest, ThenableThrowsCoroMyEx) {

	Loop loop;
	std::type_index t(typeid(std::exception));
	std::string e;

	thenableThrowsCoroMyEx(loop,t, e);

	loop.run();

	EXPECT_EQ(std::type_index(typeid(MyEx)), t);
	EXPECT_EQ("myex", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

Future<> thenableThrowsCoro(Loop& loop,std::type_index& t, std::string& e)
{
	try
	{
		co_await loop.task([]() 
		{
			throw Ex("HeloEx");
		});
	}
	catch (const std::exception& ex)
	{
		t = std::type_index(typeid(ex));
		e = ex.what();
	};
}

TEST_F(BasicTest, ThenableThrowsCoro) {

	Loop loop;
	std::type_index t(typeid(std::exception));
	std::string e;

	thenableThrowsCoro(loop,t, e);

	loop.run();

	EXPECT_EQ(std::type_index(typeid(Ex)), t);
	EXPECT_EQ("HeloEx", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}


Future<> thenableChainedThrowsCoro(Loop& loop, std::type_index& t, std::string& e)
{
	try
	{
		co_await loop.task([]()
		{
		});

		co_await loop.task([]()
		{
			throw Ex("HeloEx");
		});
	}
	catch (const std::exception& ex)
	{
		t = std::type_index(typeid(ex));
		e = ex.what();
	};
}


TEST_F(BasicTest, ThenableChainedThrowsCoro) {

	Loop loop;
	std::type_index t(typeid(std::exception));
	std::string e;

	thenableChainedThrowsCoro(loop, t, e);

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(std::type_index(typeid(Ex)), t);
	EXPECT_EQ("HeloEx", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

class TestEx : public std::exception 
{
public:
	const char* what() const noexcept
	{
		return "TestEx";
	}
};

Future<> thenableChainedThrowsCoroTestEx(Loop& loop, std::type_index& t, std::string& e)
{
	try
	{
		co_await loop.task([]()
		{
		});

		co_await loop.task([]()
		{
			throw TestEx();
		});
	}
	catch (const std::exception& ex)
	{
		t = std::type_index(typeid(ex));
		e = ex.what();
	};
}


TEST_F(BasicTest, ThenableChainedThrowsCoroTestEx) {

	Loop loop;
	std::type_index t(typeid(std::exception));
	std::string e;

	thenableChainedThrowsCoroTestEx(loop, t, e);

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(std::type_index(typeid(TestEx)), t);
	EXPECT_EQ("TestEx", e);
	MOL_TEST_ASSERT_CNTS(0, 0);
}



TEST_F(BasicTest, Thenable) {

	Loop loop;
	int c = 0;

	loop.task([&c](){
		EXPECT_EQ(0,c);
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.then([&c](){
		EXPECT_EQ(1,c);
		c++;
		MOL_TEST_PRINT_CNTS();
	});

	MOL_TEST_PRINT_CNTS();
	loop.run();

    EXPECT_EQ(2,c);
    MOL_TEST_ASSERT_CNTS(0,0);
}



TEST_F(BasicTest, ThenableWithValue) {

	Loop loop;
	int c = 0;

	loop.task([&c](){
		EXPECT_EQ(c,0);
		c++;
		return 42;
	})
	.then([&c](int i){
		EXPECT_EQ(c,1);
		EXPECT_EQ(i,42);
		c += i;
	});

	loop.run();

    EXPECT_EQ(c,43);
    MOL_TEST_ASSERT_CNTS(0,0);
}

/*
TEST_F(BasicTest, ThenableWithValueRef) {

	Loop loop;
	int c = 0;
	int& x = c;

	loop.task([&c,&x]() ->int& {
		EXPECT_EQ(c, 0);
		c++;
		
		return x;
	})
	.then([&c](int& i) {
		EXPECT_EQ(c, 1);
		EXPECT_EQ(i, 1);
		c += i;
	});

	loop.run();

	EXPECT_EQ(c, 2);
	MOL_TEST_ASSERT_CNTS(0, 0);
}
*/
static int testStructCount = 0;
class TestStruct
{
public:
	TestStruct(int a, int b)
		: a_(a), b_(b)
	{
		testStructCount++;
	}

	~TestStruct()
	{
		testStructCount--;
	}

	
	TestStruct(const TestStruct& ts)
		: a_(ts.a_), b_(ts.b_)
	{
		testStructCount++;
	}
	

	TestStruct(TestStruct&& ts)
		: a_(std::move(ts.a_)), b_(std::move(ts.b_))
	{
		testStructCount++;
	}
	

	int a() { return a_; }
	int b() { return b_; }

private:
	//TestStruct(TestStruct&& ts) = delete;

	int a_;
	int b_;
};

TEST_F(BasicTest, ThenableWithValueNotConstructible) {

	Loop loop;
	int c = 0;

	loop.task([&c]() ->TestStruct {
		EXPECT_EQ(c, 0);
		c++;

		return TestStruct(c,42);
	})
	.then([&c](TestStruct ts) {
		EXPECT_EQ(c, 1);
		EXPECT_EQ(ts.a(), c);
		EXPECT_EQ(ts.b(), 42);
		c += 1;
	});

	loop.run();

	EXPECT_EQ(c, 2);
	MOL_TEST_ASSERT_CNTS(0, 0);
	EXPECT_EQ(0, testStructCount);

}


class TestStruct2
{
public:
	TestStruct2(int a, int b)
		: a_(a), b_(b)
	{
		testStructCount++;
	}

	TestStruct2(const TestStruct2& ts)
		: a_(ts.a_), b_(ts.b_)
	{
		testStructCount++;
	}

	~TestStruct2()
	{
		testStructCount--;
	}

	TestStruct2& operator=(const TestStruct2&) = default;

	int a() { return a_; }
	int b() { return b_; }

private:

	//TestStruct2(TestStruct2&& ts) = delete;
	int a_;
	int b_;
};

TestStruct2 testTemp()
{
	return TestStruct2(1, 42);
}

TEST_F(BasicTest, ThenableWithValueNotConstructibleMovableOnly) {

	Loop loop;
	int c = 0;

	static_assert(std::is_move_constructible<TestStruct2>::value, "is not move ctr");
	static_assert(std::is_copy_constructible<TestStruct2>::value, "is not move ctr");

	loop.task([&c]() ->TestStruct2 {
		EXPECT_EQ(c, 0);
		c++;

		return TestStruct2(c, 42);
	})
	.then([&c](TestStruct2 ts) {
		EXPECT_EQ(c, 1);
		EXPECT_EQ(ts.a(), c);
		EXPECT_EQ(ts.b(), 42);
		c += 1;
	});

	loop.run();

	EXPECT_EQ(c, 2);
	MOL_TEST_ASSERT_CNTS(0, 0);
	EXPECT_EQ(0, testStructCount);

}

class TestStruct3
{
public:
	TestStruct3(int a, int b)
		: a_(a), b_(b)
	{
		testStructCount++;
	}

	TestStruct3(TestStruct3&& ts)
		: a_(std::move(ts.a_)), b_(std::move(ts.b_))
	{
		testStructCount++;
	}

	~TestStruct3()
	{
		testStructCount--;
	}

	//TestStruct3(const TestStruct3& ts) = default;

	int a() { return a_; }
	int b() { return b_; }

private:
	TestStruct3(const TestStruct2& ts) = delete;

	int a_;
	int b_;
};

TEST_F(BasicTest, ThenableWithValueNotConstructibleMoveAssignableOnly) {

	Loop loop;
	int c = 0;

	static_assert(std::is_move_constructible<TestStruct3>::value, "is not move ctr");
	static_assert(!std::is_copy_constructible<TestStruct3>::value, "is not move ctr");


	loop.task([&c]() ->TestStruct3 {
		EXPECT_EQ(c, 0);
		c++;

		return TestStruct3(c, 42);
	})
	.then([&c](TestStruct3 ts) {
		EXPECT_EQ(c, 1);
		EXPECT_EQ(ts.a(), c);
		EXPECT_EQ(ts.b(), 42);
		c += 1;
	});

	loop.run();

	EXPECT_EQ(c, 2);
	MOL_TEST_ASSERT_CNTS(0, 0);
	EXPECT_EQ(0,testStructCount);
}


template<class F>
Future<int> forEach(int start, int end, F f, std::shared_ptr<int> result = std::make_shared<int>(0) )
{
	auto p = promise<int>();

	f(start)
	.then( [p,result,start,end,f] (int r)
	{
		*result += r;
		int s = start+1;

		if( s < end )
		{
			forEach(s,end,f,result)
			.then( [p,result] (int r)
			{
				*result += r;
				p.resolve(r);
			});
		}
		else
		{
			p.resolve(*result);
		}
	});


	return p.future();
}


TEST_F(BasicTest, ForEach) {

	Loop loop;
	int r = 0;

	forEach(1,10, [&loop](int i)
	{
		auto p = promise<int>();

		loop.task([i]()
		{
			return i;
		})
		.then([p](int i)
		{
			std::cout << "forEach " << i << std::endl;
			p.resolve(i);
		});

		return p.future();
	})
	.then( [&r](int c)
	{
		std::cout << "forEach Sum " << c << std::endl;
		r = c;
	});

	loop.run();

    EXPECT_EQ(r ,45);
    MOL_TEST_ASSERT_CNTS(0,0);
}



template<class P,class T>
Future<T>
map(int start, int end, P value , std::function<Future<T>(T,P)> f)
{
	auto p = promise<T>();

	f(start,value)
	.then( [p,start,end,f] (int i)
	{
		int s = start+1;

		if( s < end )
		{
			map<P,T>(s,end,i,f)
			.then( [p] (int r)
			{
				p.resolve(r);
			});
		}
		else
		{
			p.resolve(i);
		}
	});


	return p.future();
}


TEST_F(BasicTest, ForEachMap) {

	Loop loop;
	int r = 0;

	map<int,int>(1,10, 0, [&loop](int i, int r)
	{
		auto p = promise<int>();

		loop.task([i]()
		{
			return i;
		})
		.then([p,r](int i)
		{
			std::cout << "forEach " << i << std::endl;
			p.resolve(r+i);
		});

		return p.future();
	})
	.then( [&r](int c)
	{
		std::cout << "forEach sum " << c << std::endl;
		r = c;
	});

	loop.run();

    EXPECT_EQ(r ,45);
    MOL_TEST_ASSERT_CNTS(0,0);
}


template<class R,class ... Args>
bool isSimple( std::function<R(Args...)> fun)
{
	return !IsFuture<typename std::invoke_result<decltype(fun),Args...>::type>::value;
}

TEST_F(BasicTest, IsSimpleReturn) 
{
	std::function<void()> f1 = [](){};
	std::function<int()> f2 = [](){ return 42; };
	std::function< Future<>()> f3 = [](){ auto p = promise<>(); return p.future(); };

	bool f1_isSimple = isSimple(f1);
	bool f2_isSimple = isSimple(f2);
	bool f3_isSimple = isSimple(f3);
	

    EXPECT_EQ(true,f1_isSimple);
    EXPECT_EQ(true,f2_isSimple);
    EXPECT_EQ(false,f3_isSimple);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(BasicTest, SimpleChain) 
{
	Loop loop;
	int c = 0;

	loop.task([&c](){
		EXPECT_EQ(0,c);
		c++;
	})
	.then([&c](){
		EXPECT_EQ(1,c);
		c++;
	});

	loop.run();

    EXPECT_EQ(2,c);
    MOL_TEST_ASSERT_CNTS(0,0);
}



TEST_F(BasicTest, SimpleChain2) 
{
	Loop loop;
	int c = 0;

	loop.task([&c](){
		EXPECT_EQ(0,c);
		c++;
		MOL_TEST_PRINT_CNTS();
	})
	.then([&c,&loop](){
		EXPECT_EQ(1,c);
		c++;
		return loop.task([&c](){
			c++;
			return c;
		});
	})
	.then([&c](int i){
		EXPECT_EQ(3,i);
		c++;
		MOL_TEST_PRINT_CNTS();
	});

	MOL_TEST_PRINT_CNTS();
	loop.run();

    EXPECT_EQ(4,c);
    MOL_TEST_ASSERT_CNTS(0,0);
}


Future<> coro_test(Loop& loop, int& result)
{
	std::cout << "enter coro_test" << std::endl;

	result = co_await loop.task( [result]() 
	{
		std::cout << "executing async coro_test task" << std::endl;

		return result+1;
	});

	std::cout << "leave coro_test" << std::endl;

	co_return;
}

TEST_F(BasicTest, SimpleCoroutine)
{
	Loop loop;
	int c = 0;

	std::cout << "initial call to coro_test" << std::endl;
	coro_test(loop,c)
	.then([](){})
	.otherwise([](const std::exception& ){});
	
	std::cout << "loop run()" << std::endl;
	loop.run();

	std::cout << "loop exited" << std::endl;

	EXPECT_EQ(1, c);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

Future<> coro_test_throw(Loop& loop, int& result)
{
	std::cout << "enter coro_test" << std::endl;

	// throwing early works except for MSVC on 64bit 
	// which fails with internal compiler error :-(
#ifndef _WIN64
	throw Ex("early"); 
#endif

	co_await loop.nextTick();

	throw Ex("early");

	// not necessary when you through while async anyway:
	result = co_await loop.task([result]()
	{
		std::cout << "executing async coro_test task" << std::endl;
		throw Ex("early");

		return result + 1;
	});

	std::cout << "leave coro_test" << std::endl;

	co_return;
}

Future<> coro_test_late_throw(Loop& loop, int& result)
{
	std::cout << "enter coro_test" << std::endl;

	co_await loop.nextTick();

	throw Ex("early");

	// not necessary when you through while async anyway:
	result = co_await loop.task([result]()
	{
		std::cout << "executing async coro_test task" << std::endl;
		throw Ex("early");

		return result + 1;
	});

	std::cout << "leave coro_test" << std::endl;

	co_return;
}

Future<> coro_test_trampoline(Loop& loop, int& result)
{
	std::cout << "enter coro_test_trampoline" << std::endl;
	try
	{
		co_await coro_test_throw(loop, result);
	}
	catch (const std::exception& ex)
	{
		result++;
		std::cout << "ex::" << ex.what() << std::endl;
	}
	std::cout << "leave coro_test_trampoline" << std::endl;

	co_return;
}

TEST_F(BasicTest, SimpleCoroutineWithThrow)
{
	Loop loop;
	int c = 0;

	std::cout << "initial call to coro_test" << std::endl;

	try
	{
		coro_test_trampoline(loop, c);
	}
	catch (const std::exception& ex)
	{
		c++;
		std::cout << "ex!::" << ex.what() << std::endl;
	}

	std::cout << "loop run()" << std::endl;
	loop.run();

	std::cout << "loop exited" << std::endl;

	EXPECT_EQ(1, c);
	MOL_TEST_ASSERT_CNTS(0, 0);
}


Future<> coro_test_plain_throw(Loop& loop, int& /*result*/ )
{
	std::cout << "enter coro_test" << std::endl;
	auto p = promise<>();

	loop
	.nextTick()
	.then([p]() 
	{
		p.reject(Ex("plain"));
	});

	std::cout << "leave coro_test" << std::endl;

	return p.future();
}

Future<> coro_test_plain_trampoline(Loop& loop, int& result)
{
	std::cout << "enter coro_test_trampoline" << std::endl;
	try
	{
		co_await coro_test_plain_throw(loop, result);
	}
	
	catch (const Ex& ex)
	{
		result++;
		std::cout << "ex::" << ex.what() << std::endl;
	}	
	
	catch (const std::exception& )
	{
		//result++;
		//std::cout << "ex::" << ex.what() << std::endl;
	}
	std::cout << "leave coro_test_trampoline" << std::endl;

	co_return;
}

TEST_F(BasicTest, PlainOldCoroutineImpleWithThrow)
{
	Loop loop;
	int c = 0;

	std::cout << "initial call to coro_test" << std::endl;

	coro_test_plain_trampoline(loop, c)
        .then([](){})
        .otherwise([](const std::exception& ){});


	std::cout << "loop run()" << std::endl;
	loop.run();

	std::cout << "loop exited" << std::endl;

	EXPECT_EQ(1, c);
	MOL_TEST_ASSERT_CNTS(0, 0);
}


Future<> coro_test_plain_InsideOut_trampoline(Loop& loop, int& result)
{
	std::cout << "enter coro_test_trampoline" << std::endl;

	auto p = promise<>();

	coro_test_late_throw(loop, result)
	.then([p]() { p.resolve(); })
	.otherwise([p](const std::exception& ex) 
	{
		 std::cout << "inside err handler: " << typeid(ex).name() << " " << ex.what() << std::endl;
		 p.reject(ex);
	});

	std::cout << "leave coro_test_trampoline" << std::endl;

	return p.future();
}

TEST_F(BasicTest, PlainOldCoroutineImpleWithThrowInsideOut)
{
	Loop loop;
	int c = 0;

	std::cout << "initial call to coro_test" << std::endl;

	coro_test_plain_InsideOut_trampoline(loop, c)
	.then([]() { })
	.otherwise([&c](const std::exception& ) { c++; });

	std::cout << "loop run()" << std::endl;
	loop.run();

	std::cout << "loop exited" << std::endl;

	EXPECT_EQ(1, c);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

Future<int> coro_test_value(Loop& loop, int& result)
{
	std::cout << "enter coro_test" << std::endl;

	result = co_await loop.task([result]()
	{
		std::cout << "executing async coro_test task" << std::endl;

		return result + 1;
	});

	std::cout << "leave coro_test" << std::endl;
	co_return result;
}

TEST_F(BasicTest, SimpleCoroutineWithValue)
{
	Loop loop;
	int c = 0;

	std::cout << "initial call to coro_test" << std::endl;
	coro_test_value(loop, c)
        .then([](int ){})
        .otherwise([](const std::exception& ){});


	std::cout << "loop run()" << std::endl;
	loop.run();

	std::cout << "loop exited" << std::endl;

	EXPECT_EQ(1, c);
	MOL_TEST_ASSERT_CNTS(0, 0);
}

Future<TestStruct> coroutineWithValueNotConstructible(Loop& loop)
{
	co_await loop.nextTick();

	co_return TestStruct(1, 42);
}

Future<> CoroutineWithValueNotConstructible_trampoline(Loop& loop, int& c)
{
	TestStruct ts = co_await coroutineWithValueNotConstructible(loop);
	c += ts.a();
	c += ts.b();
}

TEST_F(BasicTest, CoroutineWithValueNotConstructible) {

	Loop loop;
	int c = 0;

	CoroutineWithValueNotConstructible_trampoline(loop, c)
        .then([](){})
        .otherwise([](const std::exception& ){});


	loop.run();

	EXPECT_EQ(c, 43);
	MOL_TEST_ASSERT_CNTS(0, 0);
	EXPECT_EQ(0, testStructCount);

}


Future<TestStruct2> coroutineWithValueNotConstructibleMovableOnly(Loop& loop)
{
	co_await loop.nextTick();

	co_return TestStruct2(1, 42);
}

Future<> CoroutineWithValueNotConstructible_trampolineMovableOnly(Loop& loop, int& c)
{
	TestStruct2 ts = co_await coroutineWithValueNotConstructibleMovableOnly(loop);
	c += ts.a();
	c += ts.b();
}

TEST_F(BasicTest, CoroutineWithValueNotConstructibleMovableOnly) {

	Loop loop;
	int c = 0;

	CoroutineWithValueNotConstructible_trampolineMovableOnly(loop, c)
        .then([](){})
        .otherwise([](const std::exception& ){});

	loop.run();

	EXPECT_EQ(c, 43);
	MOL_TEST_ASSERT_CNTS(0, 0);
	EXPECT_EQ(0, testStructCount);

}


Future<TestStruct3> coroutineWithValueNotConstructibleMoveAssignableOnly(Loop& loop)
{
	co_await loop.nextTick();

	co_return TestStruct3(1, 42);
}


Future<> CoroutineWithValueNotConstructible_trampolineMoveAssignableOnly(Loop& loop, int& c)
{
	TestStruct3 ts = co_await coroutineWithValueNotConstructibleMoveAssignableOnly(loop);
	c += ts.a();
	c += ts.b();
}

TEST_F(BasicTest, CoroutineWithValueNotConstructibleMoveAssignableOnly) {

	Loop loop;
	int c = 0;

	CoroutineWithValueNotConstructible_trampolineMoveAssignableOnly(loop, c)
        .then([](){})
        .otherwise([](const std::exception& ){});

	loop.run();

	EXPECT_EQ(c, 43);
	MOL_TEST_ASSERT_CNTS(0, 0);
	EXPECT_EQ(0, testStructCount);

}

TEST_F(BasicTest, PromiseMonadLaw1LeftIdentity) {

	int value = 1;
	int lr = -1;
	int rr = -1;

	Loop loop;
	{
		// promise is M
		auto p = promise<int>();

		// bind value to M
		loop.nextTick()
		.then([value, p]()
		{
			p.resolve(value);
		});

		// a function f return decltype(value)
		auto f = [](int x)
		{
			return x * 2;
		};

		// bind M to f
		p.future()
		.then([&rr, f](int x)
		{
			rr = f(x);
		});

		// bind f to value
		lr = f(value);

		loop.run();
	}

	EXPECT_EQ(rr, 2);
	EXPECT_EQ(rr, lr);

	MOL_TEST_ASSERT_CNTS(0, 0);

}

TEST_F(BasicTest, PromiseMonadLaw2RightIdentity) {
	
	int value = 1;
	int lr = -1;
	int rr = -1;

	Loop loop;
	{

		// promise is M
		auto p = promise<int>();

		// bind value to M
		loop.nextTick()
		.then([value, p]()
		{
			p.resolve(value);
		});

		// a function f return M'
		auto f = [&loop](int x)
		{
			auto p = promise<int>();
			// bind value to M'
			loop.nextTick()
			.then([x, p]
			{
				p.resolve(x * 2);
			});
			return p.future();
		};

		// bind M to f
		p.future()
		.then(f)
		.then([&rr](int x)
		{
			rr = x;
		});

		// bind f to value
		f(value)
		.then([&lr](int x)
		{
			lr = x;
		});

		loop.run();
	}
	EXPECT_EQ(rr, 2);
	EXPECT_EQ(rr, lr);

	MOL_TEST_ASSERT_CNTS(0, 0);

}


TEST_F(BasicTest, PromiseMonadLaw3Associativity) {


	int value = 1;
	int lr = -1;
	int rr = -1;

	Loop loop;
	{
		// promise is M
		auto p = promise<int>();

		// bind value to M
		loop.nextTick()
		.then([value, p]()
		{
			p.resolve(value);
		});

		// a function f return M'
		auto f = [&loop](int x)
		{
			auto p = promise<int>();
			// bind value to M'
			loop.nextTick()
			.then([x, p]
			{
				p.resolve(x * 2);
			});
			return p.future();
		};

		// another function g return M''
		auto g = [&loop](int x)
		{
			auto p = promise<int>();
			// bind value to M''
			loop.nextTick()
			.then([x, p]
			{
				p.resolve(x * 3);
			});
			return p.future();
		};

		// bind M to f and the result of it to g
		p.future()
		.then(f)
		.then(g)
		.then([&rr](int x)
		{
			rr = x;
		});

		// bind f and g to plain value
		f(value)
		.then([&g](int x)
		{
			return g(x);
		})
		.then([&lr](int x)
		{
			lr = x;
		});

		loop.run();
	}
	EXPECT_EQ(rr, 6);
	EXPECT_EQ(rr, lr);

	MOL_TEST_ASSERT_CNTS(0, 0);

}






