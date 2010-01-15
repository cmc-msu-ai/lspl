/*
 * RefCountObject.cpp
 *
 *  Created on: Jan 15, 2010
 *      Author: alno
 */

#include "BaseInternal.h"
#include "RefCountObject.h"

namespace lspl { namespace base {

RefCountObject::RefCountObject() :
	refCount( 0 ), weakRefInfo( new WeakRefInfo( this ) )
{}

RefCountObject::~RefCountObject() {
	weakRefInfo->detach();
}

int RefCountObject::getWeakRefCount() const {
	return weakRefInfo->getWeakRefCount();
}

} } //namespace lspl::base
