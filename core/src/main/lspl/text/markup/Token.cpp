#include "../../base/BaseInternal.h"

#include "../../morphology/Morphology.h"

#include "Token.h"

#include "../attributes/AttributeValue.h"

using namespace lspl::text::attributes;

using lspl::morphology::Morphology;

namespace lspl { namespace text { namespace markup {

Token::Token( const text::Node & start, const text::Node & end, const std::string & token ) :
	Transition( TOKEN, start, end ), token( Morphology::instance().upcase( token ) ) {
}

Token::~Token() {
}

void Token::dump( std::ostream & out, std::string tabs ) const {
	out << "Token{ token = \"" << token << "\" }";
}

} } } // namespace lspl::text::markup
