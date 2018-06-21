#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_EX_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_EX_DEF_GUARD_

#include <exception>
#include <typeindex>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <functional>

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
/*
class WrappedException : public Ex
{
public:
	WrappedException()
	{}

	virtual std::exception_ptr ptr() const = 0;
	virtual void raise(const std::function<void(const std::exception&)> fun) const
	 = 0;

};
*/
class WrappedException : public Ex
{
public:

	template<class E>
	WrappedException(const E& e)
	: e_(std::make_exception_ptr(e))
	{
	}

	WrappedException(const std::exception_ptr& e)
	: e_(e)
	{
	}

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
	return WrappedException(e);
}
/*
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

*/

}

#endif



