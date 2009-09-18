/*
 * Transform.h
 *
 *  Created on: Sep 18, 2009
 *      Author: alno
 */

#ifndef _LSPL_TRANSFORMS_TRANSFORM_H_
#define _LSPL_TRANSFORMS_TRANSFORM_H_

#include "../base/Base.h"

#include "Forward.h"
#include "TransformResult.h"

#include "../text/Forward.h"

namespace lspl { namespace transforms {

/**
 * Преобразование осуществляемое правой частью шаблона.
 *
 * Преобразование является неразделяемым объектом.
 */
class LSPL_EXPORT Transform {
public:
	virtual ~Transform() {}

	/**
	 * Применить преобразование к варианту сопоставления.
	 *
	 * @param matchVariant вариант сопоставления
	 * @return результат применения
	 */
	virtual TransformResult * apply( const text::MatchVariant & matchVariant ) const = 0;
};

template< typename Result >
class LSPL_EXPORT TypedTransform : public Transform {
public:

	/**
	 * Применить преобразование к варианту сопоставления.
	 *
	 * @param matchVariant вариант сопоставления
	 * @return результат применения
	 */
	virtual TypedTransformResult<Result> * apply( const text::MatchVariant & matchVariant ) const = 0;

};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_TRANSFORM_H_ */
