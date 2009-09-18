/*
 * TransformBuilder.cpp
 *
 *  Created on: Sep 18, 2009
 *      Author: alno
 */
#include "../base/BaseInternal.h"

#include "TransformBuilder.h"

LSPL_REFCOUNT_CLASS( lspl::transforms::TransformBuilder )

namespace lspl { namespace transforms {

TransformBuilder::TransformBuilder() {
	// TODO Auto-generated constructor stub

}

TransformBuilder::~TransformBuilder() {
	// TODO Auto-generated destructor stub
}

DummyTransform * DummyTransformBuilder::build( const patterns::Alternative & alternative, const std::string & source ) {
	return new DummyTransform();
}

} } // namespace lspl::transforms
