#include "../../base/BaseInternal.h"

#include "../../text/attributes/SpeechPart.h"

#include "../Pattern.h"
#include "../matchers/Matcher.h"
#include "Variable.h"

namespace lspl { namespace patterns { namespace matchers {

Variable::Variable( const Pattern & pt, uint index ) :
	type( pt.id + text::attributes::SpeechPart::COUNT ), index( index ) {
}

Variable::Variable( const std::string & base ) {
	uint speechPart = 0;
	std::string::size_type speechSize = 0;
	for(uint i = 0; i < text::attributes::SpeechPart::COUNT; ++i) {
		const auto & abbr = text::attributes::SpeechPart::ABBREVATIONS[i];
		const auto size = abbr.size();
		if (speechSize < size && base.compare(0, size, abbr) == 0) {
			speechPart = i;
			speechSize = size;
		}
	}
	type = speechPart;
	const auto num = atoi( base.c_str() + speechSize );
	index = num > 0 ? static_cast<uint>( num ) : 1;
}

} } } // namespace lspl::patterns::matchers
