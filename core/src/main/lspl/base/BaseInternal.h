#ifndef _LSPL_BASE_BASEINTERNAL_H_
#define _LSPL_BASE_BASEINTERNAL_H_

#ifndef LSPL_EXPORT
#	ifdef WIN32
#		define LSPL_EXPORT __declspec(dllexport)
#	else
#		define LSPL_EXPORT
#	endif
#endif

#include <boost/foreach.hpp>

#ifdef MSVC // Версия для Microsoft Visual Studio

#	define LSPL_REFCOUNT_CLASS(className) \
	template<> lspl::uint lspl::base::IdentifiedObject<className>::count = 0; \
	template lspl::base::RefCountPtr<className>;

#else

#	define LSPL_REFCOUNT_CLASS(className) \
	template<> lspl::uint lspl::base::IdentifiedObject<className>::count = 0;

#endif

#endif//_LSPL_BASE_BASEINTERNAL_H_
