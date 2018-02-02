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
#define LITTLE_MOLE_DECLARE_DEBUG_REF_CNT(name) extern std::atomic<int> name##_g_count;
#define LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(name) std::atomic<int> name##_g_count;
#define LITTLE_MOLE_ADDREF_DEBUG_REF_CNT(name) name##_g_count++;
#define LITTLE_MOLE_RELEASE_DEBUG_REF_CNT(name) name##_g_count--;

#else
#define LITTLE_MOLE_DECLARE_DEBUG_REF_CNT(name)
#define LITTLE_MOLE_DEFINE_DEBUG_REF_CNT(name)
#define LITTLE_MOLE_ADDREF_DEBUG_REF_CNT(name)
#define LITTLE_MOLE_RELEASE_DEBUG_REF_CNT(name)
#endif


#endif
