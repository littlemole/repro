#include "main.h"
#include "reprocpp/after.h"


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
	.then([&c](){
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
	.then([&c](int x, int y){
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
	.then([&c](int x, int y){
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
	.then([&c](int x){
		EXPECT_EQ(c,2);
		EXPECT_EQ(x,42);
		c += x;
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
	.then([&c](int x){
		EXPECT_EQ(c,2);
		EXPECT_EQ(x,42);
		c += x;
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
	.then([&c](int x, int y){
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
	.then([&c](int x, int y, int z){
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
	.then([&c](int x, int y, int z){
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
	.then([&c](int x, int y, int z){
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
	.then([&c](int x, int y){
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
	.then([&c](int x, int y){
		EXPECT_EQ(c,3);
		EXPECT_EQ(x,42);
		EXPECT_EQ(y,43);
		c += x + y;
	});

	loop.run();

    EXPECT_EQ(c,88);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TestClassesCompile11) {

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
	.then([&c](int x)
	{
		c+=x;
	});

	loop.run();

    EXPECT_EQ(c,44);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TestClassesCompile12) {

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
	.then([&c](int x)
	{
		c+=x;
	});

	loop.run();

    EXPECT_EQ(c,44);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, TestClassesCompile13) {

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
	.then([&c](int x)
	{
		c+=x;
	});

	loop.run();

    EXPECT_EQ(c,44);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, TestClassesCompile14) {

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
	.then([&c]()
	{
		EXPECT_EQ(c,3);
		c++;
	});

	loop.run();

    EXPECT_EQ(c,4);
    MOL_TEST_ASSERT_CNTS(0,0);
}

TEST_F(AfterTest, TestClassesCompile15) {

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
	.then([&c]()
	{
		EXPECT_EQ(c,4);
		c++;
	});

	loop.run();

    EXPECT_EQ(c,5);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TestClassesCompile16) {

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
	.then([&c](int x)
	{
		EXPECT_EQ(c,3);
		c+=x;
	});

	loop.run();

    EXPECT_EQ(c,45);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TestClassesCompile17) {

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
	.then([&c](int x,int y)
	{
		EXPECT_EQ(c,2);
		c+= x;
		c+= y;
	});

	loop.run();

    EXPECT_EQ(c,87);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TestClassesCompile18) {

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
	.then([&c](int x,int y,int z)
	{
		EXPECT_EQ(c,1);
		c+= x;
		c+= y;
		c+= z;
	});

	loop.run();

    EXPECT_EQ(c,130);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TestClassesCompile19) {

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
	.then([&c](int x,int y,int z)
	{
		EXPECT_EQ(c,1);
		c+= x;
		c+= y;
		c+= z;
	});

	loop.run();

    EXPECT_EQ(c,130);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TestClassesCompile20) {

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
	.then([&c](int x,int y,int z)
	{
		EXPECT_EQ(c,1);
		c+= x;
		c+= y;
		c+= z;
	});

	loop.run();

    EXPECT_EQ(c,130);
    MOL_TEST_ASSERT_CNTS(0,0);
}


TEST_F(AfterTest, TestClassesCompile21) {

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
	.then([&c](int x,int y,int z,int a)
	{
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

TEST_F(AfterTest, TestClassesCompile22) {

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
	.then([&c](int x,int y,int z,int a,int b){
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


