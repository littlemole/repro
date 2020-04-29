#ifndef MOL_PROMISE_DEBUG_TEST_HELPER_DEF_GUARD_DEFINE_
#define MOL_PROMISE_DEBUG_TEST_HELPER_DEF_GUARD_DEFINE_

#include "reprocpp/debug.h"

/*
 * Helper to setup tests and examples and define the debug mode global counters.
 */



#ifdef MOL_PROMISE_DEBUG
#define MOL_TEST_PRINT_CNTS()  											\
for ( auto it = repro::monitorama().begin(); it != repro::monitorama().end(); it++)	\
{																		\
	std::cerr << (*it).first << ": " << (*it).second << std::endl;		\
} 	
#else
#define MOL_TEST_PRINT_CNTS()
#endif

#ifdef MOL_PROMISE_DEBUG
#define MOL_TEST_ASSERT_CNTS(p,e) \
for ( auto it = repro::monitorama().begin(); it != repro::monitorama().end(); it++)	\
{																		\
	std::cerr << (*it).first << ": " << (*it).second << std::endl;		\
	EXPECT_EQ(0,(*it).second);											\
} 	

#else
#define MOL_TEST_ASSERT_CNTS(p,e)
#endif





#endif