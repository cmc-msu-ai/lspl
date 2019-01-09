#include "../base/BaseInternal.h"

#include "Transition.h"
#include "Node.h"
#include "Text.h"
#include "../patterns/matchers/Matcher.h"

#include <iostream>

using lspl::text::attributes::AttributeKey;
using lspl::text::attributes::AttributeValue;

LSPL_REFCOUNT_CLASS( lspl::text::Transition );

namespace lspl { namespace text {

uint Transition::aliveObjectsCount = 0;

Transition::~Transition() {
	--aliveObjectsCount;
}

uint Transition::getRangeStart() const {
	return start.endOffset;
}

uint Transition::getRangeEnd() const {
	return end.startOffset;
}

std::string Transition::getRangeString() const {
	if ( end.startOffset <= start.endOffset )
		return "";

	return start.text.getContent().substr( start.endOffset, end.startOffset - start.endOffset );
}

AttributeValue Transition::getAttribute( AttributeKey key ) const {
	if ( key == AttributeKey::TEXT ) // Если запрашиваемый аттрибут - текст
		return AttributeValue( getRangeString() ); // Возвращаем текст, как строковый аттрибут

	return AttributeValue::UNDEFINED;
}

} } // namespace lspl
