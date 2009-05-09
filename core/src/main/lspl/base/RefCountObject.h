#ifndef _LSPL_BASE_REFCOUNTOBJECT_H_
#define _LSPL_BASE_REFCOUNTOBJECT_H_

#include "Base.h"
#include "IdentifiedObject.h"

namespace lspl { namespace base {

/**
 * Базовый класс для объектов, использующих подсчет ссылок в библиотеке
 */
template< class Type >
class LSPL_EXPORT RefCountObject : public IdentifiedObject<Type> {
public:
	typedef RefCountObject<Type> RefCountBase;
public:
	RefCountObject() : refCount( 0 ) {}
	virtual ~RefCountObject() {}

	/**
	 * Увеличить счетчик ссылок объектов
	 */
	void addRef() const {
		refCount ++;
	}

	/**
	 * Уменьшить счетчик ссылок объектов. Если счетчик ссылок становится меньше 1, то объект удаляется
	 */
	void release() const {
		refCount --;

		if ( refCount <= 0 ) {
			delete this;
		}
	}

private:
	/*
	 * Счетчик ссылок объекта
	 */
	mutable int refCount;
};

} }

#endif /*_LSPL_BASE_REFCOUNTOBJECT_H_*/
