#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_EX_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_EX_DEF_GUARD_

#include <exception>
#include <typeindex>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <functional>


namespace repro {


class Ex : public std::exception
{
public:

    Ex() {}
    Ex(const std::string& m) : msg(m) {}
    
    template<class ... Args>
    Ex(const std::string& m, Args ... args ) 
    {
        std::vector<std::string> v {args ...};
        std::ostringstream oss;
        oss << m << " ";
        for ( std::string s : v )
        {
            oss << s << " ";
        }
        msg = oss.str();
    }
    
    const char* what() const noexcept 
    { 
        return msg.c_str();
    }
        
    std::string msg;
};

class WrappedException : public Ex
{
public:

	template<class E>
	WrappedException(const E& e)
		: e_(std::make_exception_ptr(e))
	{}

	WrappedException(const std::exception_ptr& e)
		: e_(e)
	{}

	virtual std::exception_ptr ptr() const
	{
		return e_;
	}

	virtual void raise(const std::function<void(const std::exception&)> fun) const
	{
		if (e_)
		{
			try
			{
				std::rethrow_exception(e_);
			}
			catch (const std::exception& ex)
			{
				fun(ex);
			}
		}		
	}

private:
	std::exception_ptr e_;
};

template<class E>
WrappedException wrap_exception(const E& e)
{
	const WrappedException* p = dynamic_cast<const WrappedException*>(&e);
	if(p)
	{
		return *p;
	}
	return WrappedException(e);
}

inline WrappedException wrap_exception(std::exception_ptr e)
{
	return WrappedException(e);
}

namespace ex {

template<class T>
bool isa(const std::exception& ex)
{
	if( dynamic_cast<const T*>(&ex) ) 
	{
		return true;
	}
	return false;
}

}}

#endif



