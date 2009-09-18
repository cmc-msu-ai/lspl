/*
 * TransformBuilder.h
 *
 *  Created on: Sep 18, 2009
 *      Author: alno
 */

#ifndef _LSPL_TRANFORMS_TRANSFORMBUILDER_H_
#define _LSPL_TRANFORMS_TRANSFORMBUILDER_H_

#include "../base/Base.h"
#include "../base/RefCountObject.h"
#include "../base/RefCountPtr.h"

#include <vector>

#include "Transform.h"

#include "../patterns/Forward.h"

namespace lspl { namespace transforms {

class LSPL_EXPORT TransformBuilder : public base::RefCountObject<TransformBuilder> {
public:
	TransformBuilder();
	virtual ~TransformBuilder();

	/**
	 * Построить новое преобразование для заданной альтернативы и исходного текста.
	 *
	 * @param alternative альтернатива, для которой задано преобразование
	 * @param source исходный код
	 * @return новый экземпляр преобразования. При этом жизненный цикл экземпляра не управляется никем, за удаление экземпляра отвественнен получатель.
	 */
	virtual Transform * build( const patterns::Alternative & alternative, const std::string & source ) = 0;
};

template< typename Result>
class LSPL_EXPORT TypedTransformBuilder : public TransformBuilder {
public:

	virtual TypedTransform<Result> * build( const patterns::Alternative & alternative, const std::string & source ) = 0;
};

class LSPL_EXPORT DummyTransformBuilder : public TypedTransformBuilder<int> {
public:

	virtual DummyTransform * build( const patterns::Alternative & alternative, const std::string & source );

};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANFORMS_TRANSFORMBUILDER_H_ */
