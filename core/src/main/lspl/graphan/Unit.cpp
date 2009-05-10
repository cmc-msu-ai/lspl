/*
 * Token.cpp
 *
 *  Created on: May 9, 2009
 *      Author: alno
 */
#include "../base/BaseInternal.h"

#include "Unit.h"

namespace lspl { namespace graphan {

Unit::Unit( const std::string & str, uint offset, Type type ) :
	str( str ), offset( offset ), type( type ) {
}

Unit::Unit( const char * begin, uint length, uint offset, Type type ) :
	str( std::string( begin, length ) ), offset( offset ), type( type ) {
}

Unit::~Unit() {
}

} }
