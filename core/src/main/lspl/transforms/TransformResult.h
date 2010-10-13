/*
 * TransformResult.h
 *
 *  Created on: Sep 18, 2009
 *      Author: alno
 */

#ifndef _LSPL_TRANSFORMS_TRANSFORMRESULT_H_
#define _LSPL_TRANSFORMS_TRANSFORMRESULT_H_

#include "../base/Base.h"
#include "../base/Exception.h"

#include "Forward.h"

#include "../text/Forward.h"

#include <boost/any.hpp>

namespace lspl { namespace transforms {

template< typename Result >
class TypedTransformResult;

class LSPL_EXPORT TransformResult {
public:
	virtual ~TransformResult() {}

	template< typename Result> Result getValue() {
		if ( TypedTransformResult<Result> * res = dynamic_cast<TypedTransformResult<Result> *>( this ) )
			return res->value;
		else
			throw base::Exception( "Wrong result type requested" );
	}
};

template< typename Result >
class TypedTransformResult : public TransformResult {
public:
	TypedTransformResult( Result value ) : value( value ) {}
	virtual ~TypedTransformResult() {}

public:
	const Result value;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_TRANSFORMRESULT_H_ */
