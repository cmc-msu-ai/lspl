#include "../../base/BaseInternal.h"

#include "../../morphology/Morphology.h"

#include "Word.h"
#include "../Node.h"

#include <iostream>

using lspl::morphology::WordForm;
using lspl::morphology::Morphology;
using lspl::text::attributes::SpeechPart;
using lspl::text::attributes::AttributeKey;
using lspl::text::attributes::AttributeValue;

namespace lspl { namespace text { namespace markup {

Word::Word( const text::Node & start, const text::Node & end, const TokenRef & token, const std::string & base, const std::string & stem, SpeechPart speechPart, uint64 attributes ) :
	Transition( WORD, start, end ), token( token ), base( Morphology::instance().upcase( base ) ), stem( Morphology::instance().upcase( stem ) ), speechPart( speechPart ), attributes( attributes ) {
}

Word::~Word() {
}

void Word::dump( std::ostream & out, std::string tabs ) const {
	out << "Word{ token = " << token->getToken()
		<< ", start = " << start.index
		<< ", end = " << end.index
		<< ", base = " << base
		<< ", stem = " << stem
		<< ", speechPart = " << speechPart.getAbbrevation()
		<< ", attributes = " << getAttributesString()
		<< ", case = " << getCase().getName() << " }";
}

std::string Word::getAttributesString() const {
	return Morphology::instance().getAttributesString( attributes );
}

attributes::AttributeValue Word::getAttribute( attributes::AttributeKey key ) const {
	if ( key == AttributeKey::BASE ) // Если запрашиваемый аттрибут - основа слова
		return AttributeValue( base ); // Возвращаем основу, как строковый аттрибут

	if ( key == AttributeKey::TEXT ) // Если запрашиваемый аттрибут - текст
		return AttributeValue( getRangeString() ); // Возвращаем текст, как строковый аттрибут

	return Morphology::extractAttributeValue( attributes, key );
}

} } } // namespace lspl::text::markup
