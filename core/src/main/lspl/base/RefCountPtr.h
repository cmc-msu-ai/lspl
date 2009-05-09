#ifndef _LSPL_BASE_REFCOUNTPTR_H_
#define _LSPL_BASE_REFCOUNTPTR_H_

#include "Base.h"
#include "RefCountObject.h"

namespace lspl { namespace base {

template <typename Type>
class LSPL_EXPORT Reference {
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

	bool operator ! () const {// never throws
		return ptr == 0;
	}

	template< typename T >
	bool operator == ( const Reference<T> & p ) const {// never throws
		return ptr == p.ptr;
	}

	template< typename T >
	bool operator != ( const Reference<T> & p ) const {// never throws
		return ptr != p.ptr;
	}

	operator bool () const {// never throws
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

template <typename Type>
class LSPL_EXPORT WeakReference {

};

}

template< typename T, typename U>
inline base::Reference<T> cast( const base::Reference<U> & ptr ) {
	return base::Reference<T>( static_cast<T*>( ptr.get() ) );
}

}

#endif /*_LSPL_BASE_REFCOUNTPTR_H_*/
