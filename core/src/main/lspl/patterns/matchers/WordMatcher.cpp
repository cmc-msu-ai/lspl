#include "../../base/BaseInternal.h"

#include "WordMatcher.h"

#include "../../text/Node.h"
#include "../../text/markup/Word.h"

#include "../../morphology/Morphology.h"

using lspl::text::Transition;
using lspl::text::TransitionRef;
using lspl::text::markup::Word;

using lspl::text::attributes::SpeechPart;

using lspl::morphology::Morphology;

namespace lspl { namespace patterns { namespace matchers {

WordMatcher::WordMatcher( const std::string & base, SpeechPart speechPart )  :
	AnnotationMatcher( WORD ), base( Morphology::instance().upcase( base ) ), speechPart( speechPart ) {

}

bool WordMatcher::matchTransition( const Transition & transition, const Context & context ) const {
	if ( transition.type != Transition::WORD )
		return false;

	Word & word = *(Word *)&transition;

	if ( speechPart != SpeechPart::WORD && speechPart != word.getSpeechPart() ) // Проверяем соответствие частей речи
		return false;

	if ( base != "" && base != word.getBase() ) // Проверяем соответствие начальной формы
		return false;

	return matchRestrictions( transition, context );
}

void WordMatcher::dump( std::ostream & out, const std::string & tabs ) const {
	out << "WordMatcher{ base = " << base << ", speechPart = " << speechPart.getAbbrevation() << ", variable = " << variable << ", restrictions = ";
	dumpRestrictions( out );
	out << " }";
}

} } } // namespace lspl::patterns::matchers
