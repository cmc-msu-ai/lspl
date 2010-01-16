#include "../../base/BaseInternal.h"

#include "../../text/attributes/SpeechPart.h"

#include "../Pattern.h"
#include "Variable.h"

namespace lspl { namespace patterns { namespace matchers {

Variable::Variable( const Pattern & pt, uint index ) :
	type( pt.id + text::attributes::SpeechPart::COUNT ), index( index ) {
}

Variable::Variable( const std::string &base ) {
	int speechPart = -1;
	int speechSize = 0;
	for(int i = 0; i < text::attributes::SpeechPart::COUNT; ++i) {
		size_t size = text::attributes::SpeechPart::ABBREVATIONS[i].size();
		if (base.size() <= size) {
			continue;
		}
		if (base.substr(0, size) !=
				text::attributes::SpeechPart::ABBREVATIONS[i]) {
			continue;
		}
		if (speechPart == -1 || speechSize < size) {
			speechPart = i;
			speechSize = size;
		}
	}
	if (speechPart = -1) {
		speechPart = 0;
	}
	type = speechPart;
	int index =
			atoi(base.substr(speechSize, base.size() - speechSize).c_str());
	if (!index) {
		index = 1;
	}
}

} } } // namespace lspl::patterns::matchers
