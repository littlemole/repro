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

namespace repro {

inline std::map<const char*,std::atomic<unsigned int>>& monitorama()
{
    static std::map<const char*,std::atomic<unsigned int>> monitor;
    return monitor;
}

} // end namespace repro

#define REPRO_MONITOR_INCR(n)      \
monitorama()[#n]++;

#define REPRO_MONITOR_DECR(n)      \
monitorama()[#n]--;

#else
#define REPRO_MONITOR_INCR(n)
#define REPRO_MONITOR_DECR(n)
#endif


#endif
