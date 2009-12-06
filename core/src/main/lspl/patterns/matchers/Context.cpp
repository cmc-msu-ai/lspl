#include "../../base/BaseInternal.h"

#include "Context.h"

#include "../../text/Transition.h"
#include "../../text/attributes/AttributeValue.h"

using lspl::text::attributes::AttributeKey;
using lspl::text::attributes::AttributeValue;
using lspl::patterns::expressions::Expression;

namespace lspl { namespace patterns { namespace matchers {

Context::Context() {
}

Context::~Context() {
}

void Context::addValue( Variable var, const text::TransitionConstRef & value ) {
	map.insert( std::make_pair( var, value ) );
}

Context::ConstRange Context::getValues( Variable var ) const {
	return map.equal_range( var );
}

void Context::addAttributes( std::map<text::attributes::AttributeKey,text::attributes::AttributeValue> & attributes, const Alternative::BindingMap & bindings ) const {
	for ( Alternative::BindingMap::const_iterator it = bindings.begin(), e = bindings.end(); it != e; ++ it ) {
		Expression::ValueListPtr v = it->second->evaluate( 0, Variable(), *this );
		AttributeKey key = it->first;
		AttributeValue value = v->empty() ? AttributeValue::UNDEFINED : v->back();

		if ( key == AttributeKey::UNDEFINED ) {
			for ( uint attr = 0; attr < AttributeKey::count(); ++ attr ) {
				AttributeKey k = AttributeKey( attr );
				AttributeValue v = value.getContainer().getAttribute( k );

				if ( v != AttributeValue::UNDEFINED )
					attributes.insert( std::make_pair( k, v ) );
			}
		} else {
			attributes.insert( std::make_pair( key, value ) );
		}
	}
}

} } } // namespace lspl::patterns::matchers
