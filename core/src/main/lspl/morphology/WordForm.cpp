/*
 * WordForm.cpp
 *
 *  Created on: Dec 24, 2008
 *      Author: alno
 */

#include "../base/BaseInternal.h"

#include "WordForm.h"
#include "Morphology.h"

using namespace lspl::text::attributes;

template<> lspl::uint lspl::base::IdentifiedObject<lspl::morphology::WordForm>::count = 0;

#ifdef MSVC
template lspl::base::RefCountPtr<lspl::morphology::WordForm>;
#endif

namespace lspl { namespace morphology {

AttributeValue WordForm::getAttribute( uint setIndex, AttributeKey key ) const {
	if ( key == AttributeKey::BASE ) // Если запрашиваемый аттрибут - основа слова
		return AttributeValue( base ); // Возвращаем основу, как строковый аттрибут

	return Morphology::extractAttributeValue( attributeSets[ setIndex ], key );
}

} } // namespace lspl::morphology
