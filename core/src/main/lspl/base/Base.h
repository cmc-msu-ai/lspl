#ifndef _LSPL_BASE_BASE_H_
#define _LSPL_BASE_BASE_H_

#ifndef LSPL_EXPORT
#	ifdef WIN32
#		define LSPL_EXPORT __declspec(dllimport)
#	else
#		define LSPL_EXPORT
#	endif
#endif

/**
 * Макрос для вставки forward-объявления класса с подсчетом ссылок и связанных типов
 */
#define LSPL_REFCOUNT_FORWARD(typeName) \
	class typeName; \
	typedef base::Reference<typeName> typeName##Ref; \
	typedef base::Reference<const typeName> typeName##ConstRef; \
	typedef base::WeakReference<const typeName> typeName##WeakRef; \
	typedef std::vector<base::Reference<typeName> > typeName##List;

namespace lspl {

typedef unsigned int uint;
typedef unsigned long long uint64;

}

#endif//_LSPL_BASE_BASE_H_
