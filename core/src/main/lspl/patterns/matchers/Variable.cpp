#include "../../base/BaseInternal.h"

#include "../../text/attributes/SpeechPart.h"

#include "../Pattern.h"
#include "Variable.h"

namespace lspl { namespace patterns { namespace matchers {

Variable::Variable( const Pattern & pt, uint index ) :
	type( pt.id + text::attributes::SpeechPart::COUNT ), index( index ) {
}

} } } // namespace lspl::patterns::matchers
