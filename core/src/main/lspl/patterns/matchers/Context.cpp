#include "../../base/BaseInternal.h"

#include "Context.h"

#include "../../text/Transition.h"
#include "../../text/attributes/AttributeValue.h"

using lspl::text::attributes::AttributeKey;
using lspl::text::attributes::AttributeValue;

namespace lspl { namespace patterns { namespace matchers {

Context::Context() {
}

Context::~Context() {
}

AttributeValue Context::getAttribute( Variable variable, AttributeKey attribute ) const {
	Map::const_iterator i = map.find( variable );

	if ( i == map.end() )
		return AttributeValue::UNDEFINED;

	return i->second->getAttribute( attribute );
}

AttributeValue Context::getVariable( Variable variable ) const {
	Map::const_iterator i = map.find( variable );

	if ( i == map.end() )
		return AttributeValue::UNDEFINED;

	return AttributeValue( *i->second );
}

} } } // namespace lspl::patterns::matchers
