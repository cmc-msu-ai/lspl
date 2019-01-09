#ifndef _LSPL_BASE_REFCOUNTOBJECT_H_
#define _LSPL_BASE_REFCOUNTOBJECT_H_

#include "Base.h"
#include "IdentifiedObject.h"

#include <stdexcept>

namespace lspl { namespace base {

class WeakRefInfo;

/**
 * Базовый класс для объектов, использующих подсчет ссылок
 */
class LSPL_EXPORT RefCountObject {
public:
	RefCountObject();
	virtual ~RefCountObject();

	/**
	 * Увеличить счетчик ссылок объектов
	 */
	void addRef() const {
		++ refCount;
	}

	/**
	 * Уменьшить счетчик ссылок объектов. Если счетчик ссылок становится меньше 1, то объект удаляется
	 */
	void release() const {
		-- refCount;

		if ( refCount < 0 ) {
			throw std::logic_error( "Negative reference count" );
		} else if ( refCount == 0 ) {
			delete this;
		}
	}

	/**
	 * Получить количестве ссылок на объект
	 */
	int getRefCount() const {
		return refCount;
	}

	/**
	 * Получить количестве ссылок на объект
	 */
	int getWeakRefCount() const;

	WeakRefInfo * getWeakRefInfo() const {
		return weakRefInfo;
	}

private:
	/**
	 * Счетчик ссылок объекта
	 */
	mutable int refCount;

	/**
	 * Информация о слабых ссылках на объект
	 */
	WeakRefInfo * weakRefInfo;
};

/**
 * Информация об объекте, которая живет объект или хотя бы одна его слабая ссылка
 */
class WeakRefInfo {
public:
	WeakRefInfo( RefCountObject * obj ) : object( obj ), weakRefCount( 0 ) {}
	~WeakRefInfo() {}

	/**
	 * Увеличить счетчик ссылок объектов
	 */
	void addWeakRef() const {
		++ weakRefCount;
	}

	/**
	 * Уменьшить счетчик ссылок объектов. Если счетчик ссылок становится меньше 1, то объект удаляется
	 */
	void releaseWeak() const {
		-- weakRefCount;

		if ( weakRefCount < 0 ) {
			throw std::logic_error( "Negative weak reference count" );
		} else if ( weakRefCount == 0 && object == 0 ) {
			delete this;
		}
	}

	/**
	 * Вызывается, когда объект
	 */
	void detach() const {
		object = 0;

		if ( weakRefCount == 0 ) {
			delete this;
		}
	}

	RefCountObject * getObject() {
		return object;
	}

	int getWeakRefCount() {
		return weakRefCount;
	}

	int getRefCount() {
		return object ? object->getRefCount() : 0;
	}

private:

	/**
	 * Указатель на сам объект
	 */
	mutable RefCountObject * object;

	/**
	 * Счетчик ссылок объекта
	 */
	mutable int weakRefCount;

};

/**
 * Ссылка на объект, увеличвающая время жизни объекта
 */
template <typename Type>
class Reference {
public:
	Reference() : ptr( 0 ) {
	}

	Reference( Type * ptr ) : ptr( ptr ) {
		if ( ptr ) ptr->addRef();
	}

	template< typename T >
	Reference( const Reference<T> & rcptr ) : ptr( rcptr.get() ) {
		if ( ptr ) ptr->addRef();
	}

	Reference( const Reference & rcptr ) : ptr( rcptr.ptr ) {
		if ( ptr ) ptr->addRef();
	}

	~Reference() {
		if ( ptr ) ptr->release();
		ptr = 0;
	}

	Type * get() const {
		return ptr;
	}

	Type * operator ->() const {
		return ptr;
	}

	Type & operator *() const {
		return *ptr;
	}

	bool operator ! () const { // never throws
		return ptr == 0;
	}

	template< typename T >
	bool operator == ( const Reference<T> & p ) const { // never throws
		return ptr == p.ptr;
	}

	template< typename T >
	bool operator != ( const Reference<T> & p ) const { // never throws
		return ptr != p.ptr;
	}

	/**
	 * Ссылается ли ссылка на объект?
	 */
	operator bool () const { // never throws
		return ptr != 0;
	}

	void swap(Reference & rhs) {
		Type * tmp = ptr;
		ptr = rhs.ptr;
		rhs.ptr = tmp;
	}

	template< typename T >
	Reference<T> cast() const {
		return base::Reference<T>( static_cast<T*>( ptr ) );
	}

	Reference & operator = ( const Reference & rcptr ) {
		Reference( rcptr ).swap( *this );
		return *this;
	}

private:
	Type * ptr;
};

/**
 * Ссылка на объект, не увеличивающая время жизни объекта
 */
template <typename Type>
class WeakReference {
public:
	WeakReference() : info( 0 ) {
	}

	WeakReference( Type * ptr ) {
		if ( ptr ) {
			info = ptr->weakRefInfo;
			if ( info ) info->addWeakRef();
		} else {
			info = 0;
		}
	}

	WeakReference( const Reference<Type> & rcptr ) {
		Type * ptr = rcptr.get();

		if ( ptr ) {
			info = ptr->getWeakRefInfo();
			if ( info ) info->addWeakRef();
		} else {
			info = 0;
		}
	}

	WeakReference( const WeakReference & rcptr ) : info( rcptr.info ) {
		if ( info ) info->addWeakRef();
	}

	template< typename T >
	WeakReference( const Reference<T> & rcptr ) {
		Type * ptr = rcptr.get();

		if ( ptr ) {
			info = ptr->weakRefInfo;
			if ( info ) info->addWeakRef();
		} else {
			info = 0;
		}
	}

	template< typename T >
	WeakReference( const WeakReference<T> & rcptr ) {
		info = rcptr->getInfo();
		if ( info ) info->addWeakRef();
	}

	~WeakReference() {
		if ( info ) info->releaseWeak();
		info = 0;
	}

	WeakRefInfo * getInfo() {
		return info;
	}

	const WeakRefInfo * getInfo() const {
		return info;
	}

	/**
	 * Получить сильный указатель
	 */
	Reference<Type> lock() const {
		if ( info == 0 || info->getObject() == 0 )
			return Reference<Type>();
		else
			return Reference<Type>( dynamic_cast<Type*>( info->getObject() ) );
	}

	bool operator ! () const { // never throws
		return info == 0 || info->getObject() == 0;
	}

	template< typename T >
	bool operator == ( const WeakReference<T> & p ) const { // never throws
		return info == p.info;
	}

	template< typename T >
	bool operator != ( const WeakReference<T> & p ) const { // never throws
		return info != p.info;
	}

	/**
	 * Ссылается ли ссылка на существующий объект?
	 */
	operator bool () const {// never throws
		return info != 0 && info->getObject() != 0;
	}

	void swap(WeakReference & rhs) {
		WeakRefInfo * tmp = info;
		info = rhs.info;
		rhs.info = tmp;
	}

	template< typename T >
	WeakReference<T> cast() const {
		return WeakReference<T>( info );
	}

	WeakReference & operator = ( const WeakReference & rcptr ) {
		WeakReference( rcptr ).swap( *this );
		return *this;
	}

private:
	WeakRefInfo * info;
};

}

template< typename T, typename U>
inline base::Reference<T> cast( const base::Reference<U> & ptr ) {
	return base::Reference<T>( static_cast<T*>( ptr.get() ) );
}

}

#endif /*_LSPL_BASE_REFCOUNTOBJECT_H_*/
