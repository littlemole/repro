#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_EX_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_EX_DEF_GUARD_

#include <exception>
#include <typeindex>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

/*
 * mainly provides promise base exception.
 * additionally there is Wrapped Exception base
 * to wrap an exception for asynchronous dispatch.
 * to test any exception derived from Ex, including
 * Wrapped Exceptions against a given type, use
 * the isa() helper template below.
 *
 */

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

class WrappedEx : public Ex
{
public:
	WrappedEx(const std::exception& ex)
	:Ex(ex.what()),type_(typeid(ex))
	{
		const WrappedEx* we = dynamic_cast<const WrappedEx*>(&ex);
		if (we)
		{
			type_ = we->type();
		}
	}

	const std::type_index& type() const
	{
		return type_;
	}

protected:
	std::type_index type_;
};

namespace ex {

inline bool isWrapped(const std::exception& ex)
{
	return dynamic_cast<const WrappedEx*>(&ex) != 0;
};


template<class T>
bool isa(const std::exception& ex)
{
	if( dynamic_cast<const T*>(&ex) ) {
		return true;
	}

	if(isWrapped(ex))
	{
		const WrappedEx* wrapped = dynamic_cast<const WrappedEx*>(&ex);
		if (wrapped->type() == std::type_index(typeid(T)) )
		{
			return true;
		}
	}

	return false;
}



}}

#endif



