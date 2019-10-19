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



inline std::map<const char*,std::atomic<unsigned int>>& monitorama()
{
    static std::map<const char*,std::atomic<unsigned int>> monitor;
    return monitor;
}

class Monitor 
{
public:

    Monitor(const char* name)
        : name_(name)
    {
        monitorama()[name_]++;
    }

    Monitor(const Monitor& rhs)
        :name_(rhs.name_)
    {
        monitorama()[name_]++;
    }

    Monitor( Monitor&& rhs)
        :name_(rhs.name_)
    {
        rhs.name_ = 0;
    }

    ~Monitor()
    {
        if(name_)
        {
            monitorama()[name_]--;
        }
    }

    Monitor& operator=(const Monitor& rhs)
    {
        if( this == &rhs)
        {
            return *this;
        }
        name_ = rhs.name_;
        return *this;
    }

    Monitor& operator=(Monitor&& rhs)
    {
        if( this == &rhs)
        {
            return *this;
        }
        name_ = rhs.name_;
        return *this;
    }

private:
    const char* name_;    
};

#define LITTLE_MOLE_MONITOR(n)      \
class Monitor_##n : public Monitor  \
{                                   \
public:                             \
    Monitor_##n() : Monitor(#n)     \
    {}                              \
};                                  \
Monitor_##n monitor_##n_;

#else
#define LITTLE_MOLE_MONITOR(n)
#endif

#endif
