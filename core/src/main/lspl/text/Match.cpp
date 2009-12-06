#include "../base/BaseInternal.h"

#include "Match.h"

#include "../text/Text.h"
#include "../text/Node.h"
#include "../text/attributes/AttributeValue.h"

#include "../patterns/Pattern.h"
#include "../patterns/Alternative.h"

#include "../transforms/Transform.h"

#include <iostream>

using namespace lspl::text::attributes;
using namespace lspl::patterns;

LSPL_REFCOUNT_CLASS( lspl::text::MatchVariant );

namespace lspl { namespace text {

std::string Fragment::getText() const {
	return match->start.text.getContent().substr( start, end - start );
}

std::string Fragment::getNormalizedText() const {
	return match->start.text.getContent().substr( start, end - start );
}

std::string Fragment::getPatternedText( uint opts ) const {
	return match->start.text.getContent().substr( start, end - start );
}

MatchVariant::MatchVariant( const patterns::Alternative & alternative ) :
	TransitionList(), alternative( alternative ), transformResult( 0 ) {
}

MatchVariant::MatchVariant( const MatchVariant & variant ) :
	TransitionList( variant ), alternative( variant.alternative ), transformResult( 0 ) {
}

MatchVariant::~MatchVariant() {
	if ( transformResult )
		delete transformResult;
}

transforms::TransformResult * MatchVariant::calculateTransformResult() const {
	return alternative.hasTransform() ? alternative.getTransform().applyAndBox( *this ) : new transforms::TypedTransformResult<int>( 0 );
}

MatchVariantContainer::MatchVariantContainer( Type type, const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, const AttributesMap & attributes ) :
	Transition( type, start, end ), pattern( pattern ), attributes( attributes ) {
}

MatchVariantContainer::~MatchVariantContainer() {
}

RestrictedMatch::RestrictedMatch( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, MatchVariant * variant, const AttributesMap & attributes ) :
	MatchVariantContainer( RESTRICTED_MATCH, start, end, pattern, attributes ) {
	addVariant( variant );
}

RestrictedMatch::~RestrictedMatch() {
}

AttributeValue MatchVariantContainer::getAttribute( AttributeKey key ) const {
	AttributesMap::const_iterator it = attributes.find( key ); // Находим аттрибут в карте

	if ( it != attributes.end() )
		return it->second;

	return Transition::getAttribute( key );
}

void RestrictedMatch::dump( std::ostream & out, std::string tabs ) const {
	out << "RestrictedMatch{ name = " << pattern.name << ", start = " << start.index
			<< ", end = " << end.index << ", attributes = [";

	for ( AttributesMap::const_iterator it = attributes.begin(); it != attributes.end(); ++ it ) {
		if ( it != attributes.begin() )
			out << ",";

		out << "\n\t" << tabs << it->first.getAbbrevation() << " / " << it->second;
	}

	out << "\n" << tabs << "] }";
}

Match::Match( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, MatchVariant * variant, const AttributesMap & attributes ) :
	MatchVariantContainer( MATCH, start, end, pattern, attributes ), fragments( 0 ) {
	addVariant( variant );
}

Match::~Match() {
	if ( fragments )
		delete []fragments;
}

const Fragment & Match::getFragment(uint num) const {
	if (!fragments) {
		fragments = new Fragment[1];
		fragments[0].match = this;
		fragments[0].start = start.endOffset;
		fragments[0].end = end.startOffset;
	}

	return fragments[0];
}

void Match::dump( std::ostream & out, std::string tabs ) const {
	out << "Match{ name = " << pattern.name << ", start = " << start.index
			<< ", end = " << end.index << ", attributes = [";

	for ( AttributesMap::const_iterator it = attributes.begin(); it != attributes.end(); ++ it ) {
		if ( it != attributes.begin() )
			out << ",";

		out << "\n\t" << tabs << it->first.getAbbrevation() << " / " << it->second;
	}

	out << "\n" << tabs << "] }";
}

bool Match::equals( const patterns::Pattern & p, const Node & s, const Node & e, const AttributesMap & atts ) const {
	if ( &p != &pattern || &s != &start || &e != &end )
			return false; // Если не совпадают шаблоны или узлы сопоставлений, то сопоставления не равны

	AttributesMap::const_iterator self_it = attributes.begin(); // Итератор аттрибутов карты текущего сопоставления
	AttributesMap::const_iterator match_it = atts.begin(); // Итератор аттрибутов карты сравниваемого сопоставления

	while ( ( self_it != attributes.end() ) && ( match_it != atts.end() ) ) {
		if ( self_it->first != match_it->first || self_it->second != match_it->second )
			return false; // Если значения карты не совпадают, то сопоставления не равны

		// Сдвигаем итераторы
		++ self_it;
		++ match_it;
	}

	return ( self_it == attributes.end() ) && ( match_it == atts.end() );
}

} }
