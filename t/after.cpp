#include "main.h"

#include "reprocpp/after.h"

namespace repro 
{



}

using namespace repro;

class AfterTest : public ::testing::Test {
 protected:


	  virtual void SetUp() {
		  MOL_TEST_PRINT_CNTS();
	  }

	  virtual void TearDown() {
		  MOL_TEST_PRINT_CNTS();
	  }

}; // end test setup




TEST_F(AfterTest, twoVoidThenables) {

	Loop loop;
	int c = 0;

	both(
		loop.task([&c](){
			c++;
		}),
		loop.task([&c](){
			c++;
		})
	)
	.then([&c](std::tuple<>){
		 EXPECT_EQ(c,2);
		 c++;
	});

	loop.run();

    EXPECT_EQ(c,3);
    MOL_TEST_ASSERT_CNTS(0,0);
}




TEST_F(AfterTest, TwoThenablesWithValue) {

	Loop loop;
	int c = 0;

	both(
		loop.task([](){
			return 42;
		}),
		loop.task([](){
			return 43;
		})
	)
	.then([&c](std::tuple<int,int> r)
	{
		int x = std::get<0>(r);
		int y = std::get<1>(r);
		EXPECT_EQ(x,42);
		EXPECT_EQ(y,43);
		c += x + y;
	});

	loop.run();

    EXPECT_EQ(c,85);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TwoThenablesWithValueSwitched) {

	Loop loop;
	int c = 0;

	both(
		loop.task2([](){
			return 42;
		}),
		loop.task([](){
			return 43;
		})
	)
	.then([&c](std::tuple<int, int> r)
	{
		int x, y;
		std::tie(x, y) = r;
		EXPECT_EQ(x,42);
		EXPECT_EQ(y,43);
		c += x + y;
	});

	loop.run();

    EXPECT_EQ(c,85);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, ValueThenableAndVoidThenable) {

	Loop loop;
	int c = 0;

	both(
		loop.task([&c](){
			c++;
			return 42;
		}),
		loop.task([&c](){
			c++;
		})
	)
	.then([&c](std::tuple<int> x){
		EXPECT_EQ(c,2);
		EXPECT_EQ(std::get<0>(x),42);
		c += std::get<0>(x);
	});

	loop.run();

    EXPECT_EQ(c,44);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, VoidThenableAndValueThenable) {

	Loop loop;
	int c = 0;

	both(
		loop.task([&c](){
			c++;
		}),
		loop.task([&c](){
			c++;
			return 42;
		})
	)
	.then([&c](std::tuple<int> x){
		EXPECT_EQ(c,2);
		EXPECT_EQ(std::get<0>(x),42);
		c += std::get<0>(x);
	});

	loop.run();

    EXPECT_EQ(c,44);
    MOL_TEST_ASSERT_CNTS(0,0);
}



TEST_F(AfterTest, OneVoidThenableAndTwoValueThenables) {

	Loop loop;
	int c = 0;

	after(
		loop.task([&c](){
			c++;
		}),
		loop.task([&c](){
			c++;
			return 42;
		}),
		loop.task([&c](){
			c++;
			return 43;
		})
	)
	.then([&c](std::tuple<int, int> r)
	{
		int x, y;
		std::tie(x, y) = r;
		EXPECT_EQ(c,3);
		EXPECT_EQ(x,42);
		EXPECT_EQ(y,43);
		c += x + y;
	});

	loop.run();

    EXPECT_EQ(c,88);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, ThreeValueThenables) {

	Loop loop;
	int c = 0;

	after(
		loop.task([&c](){
			c++;
			return 41;
		}),
		loop.task([&c](){
			c++;
			return 42;
		}),
		loop.task([&c](){
			c++;
			return 43;
		})
	)
	.then([&c](std::tuple<int, int,int> r)
	{
		int x, y, z;
		std::tie(x, y, z) = r;
		EXPECT_EQ(c,3);
		EXPECT_EQ(x,41);
		EXPECT_EQ(y,42);
		EXPECT_EQ(z,43);
		c += x + y + z;
	});

	loop.run();

    EXPECT_EQ(c,129);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, ThreeValueThenablesSwitched) {

	Loop loop;
	int c = 0;

	after(
		loop.task2([&c](){
			c++;
			return 41;
		}),
		loop.task([&c](){
			c++;
			return 42;
		}),
		loop.task([&c](){
			c++;
			return 43;
		})
	)
	.then([&c](std::tuple<int, int, int> r)
	{
		int x, y, z;
		std::tie(x, y, z) = r;

		EXPECT_EQ(c,3);
		EXPECT_EQ(x,41);
		EXPECT_EQ(y,42);
		EXPECT_EQ(z,43);
		c += x + y + z;
	});

	loop.run();

    EXPECT_EQ(c,129);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, ThreeValueThenablesSwitched2) {

	Loop loop;
	int c = 0;

	after(
		loop.task([&c](){
			c++;
			return 41;
		}),
		loop.task2([&c](){
			c++;
			return 42;
		}),
		loop.task([&c](){
			c++;
			return 43;
		})
	)
	.then([&c](std::tuple<int, int, int> r)
	{
		int x, y, z;
		std::tie(x, y, z) = r;
		EXPECT_EQ(c,3);
		EXPECT_EQ(x,41);
		EXPECT_EQ(y,42);
		EXPECT_EQ(z,43);
		c += x + y + z;
	});

	loop.run();

    EXPECT_EQ(c,129);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, ValueTheableAndVoidThenableAndValueThenable) {

	Loop loop;
	int c = 0;

	after(
		loop.task([&c](){
			c++;
			return 42;
		}),
		loop.task([&c](){
			c++;
		}),
		loop.task([&c](){
			c++;
			return 43;
		})
	)
	.then([&c](std::tuple<int, int> r)
	{
		int x, y;
		std::tie(x, y) = r;
		EXPECT_EQ(c,3);
		EXPECT_EQ(x,42);
		EXPECT_EQ(y,43);
		c += x + y;
	});

	loop.run();

    EXPECT_EQ(c,88);
    MOL_TEST_ASSERT_CNTS(0,0);
}



TEST_F(AfterTest, TwoValueThenablesAndVoidThenable) {

	Loop loop;
	int c = 0;

	after(
		loop.task([&c](){
			c++;
			return 42;
		}),
		loop.task([&c](){
			c++;
			return 43;
		}),
		loop.task([&c](){
			c++;
		})
	)
	.then([&c](std::tuple<int, int> r)
	{
		int x, y;
		std::tie(x, y) = r;
		EXPECT_EQ(c,3);
		EXPECT_EQ(x,42);
		EXPECT_EQ(y,43);
		c += x + y;
	});

	loop.run();

    EXPECT_EQ(c,88);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, OneValueAndTwoVoids) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([]()
		{
			return 42;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		})
	)
	.then([&c](std::tuple<int> x)
	{
		c+=std::get<0>(x);
	});

	loop.run();

    EXPECT_EQ(c,44);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, OneVoidOneValueOneVoid) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 42;
		}),
		loop.task([&c]()
		{
			c++;
		})
	)
	.then([&c](std::tuple<int> x)
	{
		c+=std::get<0>(x);
	});

	loop.run();

    EXPECT_EQ(c,44);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, VoidVoidValue) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 42;
		})
	)
	.then([&c](std::tuple<int> x)
	{
		c+=std::get<0>(x);
	});

	loop.run();

    EXPECT_EQ(c,44);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, ThreeVoids) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		})
	)
	.then([&c](std::tuple<>)
	{
		EXPECT_EQ(c,3);
		c++;
	});

	loop.run();

    EXPECT_EQ(c,4);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, FourVoids) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		})
	)
	.then([&c](std::tuple<>)
	{
		EXPECT_EQ(c,4);
		c++;
	});

	loop.run();

    EXPECT_EQ(c,5);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, ValueAndThreeVoids) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([]()
		{
			return 42;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		})
	)
	.then([&c](std::tuple<int> x)
	{
		EXPECT_EQ(c,3);
		c+=std::get<0>(x);
	});

	loop.run();

    EXPECT_EQ(c,45);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, ValueTwoVoidsAndValue) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([]()
		{
			return 42;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 43;
		})
	)
	.then([&c](std::tuple<int,int> r)
	{
		int x, y;
		std::tie(x, y) = r;

		EXPECT_EQ(c,2);
		c+= x;
		c+= y;
	});

	loop.run();

    EXPECT_EQ(c,87);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, twoIntsAndVoid) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([]()
		{
			return 42;
		}),
		loop.task([]()
		{
			return 43;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 44;
		})
	)
	.then([&c](std::tuple<int, int,int> r)
	{
		int x, y, z;
		std::tie(x, y, z) = r;

		EXPECT_EQ(c,1);
		c+= x;
		c+= y;
		c+= z;
	});

	loop.run();

    EXPECT_EQ(c,130);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, IntVoidAndTwoInts) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([]()
		{
			return 42;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 43;
		}),
		loop.task([]()
		{
			return 44;
		})
	)
	.then([&c](std::tuple<int, int, int> r)
	{
		int x, y, z;
		std::tie(x, y, z) = r;

		EXPECT_EQ(c,1);
		c+= x;
		c+= y;
		c+= z;
	});

	loop.run();

    EXPECT_EQ(c,130);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, VoidAndThreeInts) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 42;
		}),
		loop.task([]()
		{
			return 43;
		}),
		loop.task([]()
		{
			return 44;
		})
	)
	.then([&c](std::tuple<int, int, int> r)
	{
		int x, y, z;
		std::tie(x, y, z) = r;

		EXPECT_EQ(c,1);
		c+= x;
		c+= y;
		c+= z;
	});

	loop.run();

    EXPECT_EQ(c,130);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, VoidAndFourInts) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 42;
		}),
		loop.task([]()
		{
			return 43;
		}),
		loop.task([]()
		{
			return 44;
		}),
		loop.task([]()
		{
			return 45;
		})
	)
	.then([&c](std::tuple<int, int, int,int> r)
	{
		int x, y, z, a;
		std::tie(x, y, z, a) = r;

		EXPECT_EQ(c,1);
		c+= x;
		c+= y;
		c+= z;
		c+= a;
	});

	loop.run();

    EXPECT_EQ(c,175);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, TwoVoidsAnd5Ints) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 42;
		}),
		loop.task([]()
		{
			return 43;
		}),
		loop.task([]()
		{
			return 44;
		}),
		loop.task([]()
		{
			return 45;
		}),
		loop.task([]()
		{
			return 46;
		})
	)
	.then([&c](std::tuple<int, int, int, int,int> r)
	{
		int x, y, z, a ,b;
		std::tie(x, y, z, a ,b) = r;

		EXPECT_EQ(c,2);
		c+= x;
		c+= y;
		c+= z;
		c+= a;
		c+= b;
	});

	MOL_TEST_PRINT_CNTS();
	loop.run();

    EXPECT_EQ(c,222);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, IntDoubleAndString) {

	Loop loop;
	std::atomic<int> c(0);

	after(
		loop.task([&c]()
		{
			c++;
			return 42;
		}),
			loop.task([&c]()
		{
			c++;
			return 42.0;
		}),
			loop.task([]()
		{
			return std::string("hello world");
		})
	)
	.then([&c](std::tuple<int, double, std::string> r)
	{
		int x;
		double d;
		std::string s;

		std::tie(x, d,s) = r;

		EXPECT_EQ(c, 2);
		EXPECT_EQ(x, 42);
		EXPECT_EQ(d, 42.0);
		EXPECT_EQ(s, "hello world");
		c += x;
		c++;
	});

	MOL_TEST_PRINT_CNTS();
	loop.run();

	EXPECT_EQ(c, 45);
	MOL_TEST_ASSERT_CNTS(0, 0);
}


Future<> coro_trampoline(Loop& loop, std::atomic<int>& c)
{
	auto r = co_await after(

		loop.task([]()
		{
			return 42;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([&c]()
		{
			c++;
		}),
		loop.task([]()
		{
			return 43;
		})
	);

	int x, y;
	std::tie(x, y) = r;

	EXPECT_EQ(c, 2);
	c += x;
	c += y;

	//co_return;
}



TEST_F(AfterTest, CoroAfter) {

	Loop loop;
	std::atomic<int> c(0);

	coro_trampoline(loop, c);

	loop.run();

	EXPECT_EQ(c, 87);

	MOL_TEST_ASSERT_CNTS(0, 0);
}


