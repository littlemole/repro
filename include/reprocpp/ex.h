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

/// exception base
/// derive all exception from Ex
/// to avoid slice-by-value
class Ex : public std::exception
{
public:

	//! default constructor
    Ex() {}
	//! constructor
	//! \param std::string message for exception
    Ex(const std::string& m) : msg(m) {}

	//! get exception message text
    const char* what() const noexcept 
    { 
        return msg.c_str();
    }

	//! clone this exception and make std::exception_ptr
	virtual std::exception_ptr make_exception_ptr() const
	{
		return std::make_exception_ptr(*this);
	}
        
	//! the exception message
    std::string msg;
};

/// recursive exception base class helper to derive an exception E from
/// will implement make_exception_ptr() 
template<class E>
class ReproEx : public Ex
{
public:

    ReproEx() {}
    ReproEx(const std::string& m) : Ex(m) {}

	virtual std::exception_ptr make_exception_ptr() const
	{
		E* e = (E*) this;
		return std::make_exception_ptr(*e);
	}
};


namespace ex {

//! check whether an exception is of type T
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

//! make an Ex derived exception with default behaviour

#define MAKE_REPRO_EX(ex) 					\
class ex : public repro::ReproEx<ex>		\
{											\
public:										\
	ex() {}									\
	ex(const std::string& s) 				\
	: repro::ReproEx<ex>(s) {}				\
};

#endif



