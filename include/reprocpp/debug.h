#ifndef MOL_PROMISE_DEBUG_HELPER_DEF_GUARD_DEFINE_
#define MOL_PROMISE_DEBUG_HELPER_DEF_GUARD_DEFINE_

/*
 * helper to define Debug mode only macros.
 * in release mode they generate nothing.
 *
 */

#ifdef MOL_PROMISE_DEBUG
#include <iostream>
#include <atomic>
#include <map>
#include <string>

namespace repro {

inline std::map<std::string,std::atomic<unsigned int>>& monitorama()
{
    static std::map<std::string,std::atomic<unsigned int>> monitor;
    return monitor;
}

} // end namespace repro

#define REPRO_MONITOR_INCR(n)      \
repro::monitorama()[#n]++;

#define REPRO_MONITOR_DECR(n)      \
repro::monitorama()[#n]--;

#else
#define REPRO_MONITOR_INCR(n)
#define REPRO_MONITOR_DECR(n)
#endif


#endif
