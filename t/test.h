#ifndef MOL_PROMISE_DEBUG_TEST_HELPER_DEF_GUARD_DEFINE_
#define MOL_PROMISE_DEBUG_TEST_HELPER_DEF_GUARD_DEFINE_

#include "reprocpp/debug.h"

/*
 * Helper to setup tests and examples and define the debug mode global counters.
 */


namespace mol {



LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(promises)
/*
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(events)
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(timeouts)
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(sockets)
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(server_connections)
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(client_connections)
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(ws_connections)
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(ws_writers)
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(tcp_connections)
LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(ssl_connections)
*/

}

#ifdef MOL_PROMISE_DEBUG
#define MOL_TEST_PRINT_CNTS()  \
	    std::cerr << "promises:" << mol::g_promises_count << std::endl; \

/*		
	    std::cerr << "events:" << mol::g_events_count << std::endl; \
	    std::cerr << "timeouts:" << mol::g_timeouts_count << std::endl; \
	    std::cerr << "sockets:" << mol::g_sockets_count << std::endl; \
		std::cerr << "processors:" << mol::g_server_connections_count << std::endl; \
		std::cerr << "clients:" << mol::g_client_connections_count << std::endl; \
		std::cerr << "websockets:" << mol::g_ws_connections_count << std::endl; \
		std::cerr << "clients:" << mol::g_ws_writers_count << std::endl; \
		std::cerr << "tcp connections:" << mol::g_tcp_connections_count << std::endl; \
		std::cerr << "ssl connections:" << mol::g_ssl_connections_count << std::endl;
*/		
#else
#define MOL_TEST_PRINT_CNTS()
#endif

#ifdef MOL_PROMISE_DEBUG
#define MOL_TEST_ASSERT_CNTS(p,e) \
	    EXPECT_EQ(p,mol::g_promises_count); \

/*		
	    EXPECT_EQ(e,mol::g_events_count); \
	    EXPECT_EQ(e,mol::g_timeouts_count); \
	    EXPECT_EQ(0,mol::g_sockets_count); \
	    EXPECT_EQ(0,mol::g_server_connections_count); \
	    EXPECT_EQ(0,mol::g_client_connections_count); \
		EXPECT_EQ(0,mol::g_ws_connections_count); \
		EXPECT_EQ(0,mol::g_ws_writers_count); \
		EXPECT_EQ(0,mol::g_tcp_connections_count); \
		EXPECT_EQ(0,mol::g_ssl_connections_count);
*/		
#else
#define MOL_TEST_ASSERT_CNTS(p,e)
#endif





#endif
