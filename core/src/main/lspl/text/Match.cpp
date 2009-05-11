#include "../base/BaseInternal.h"

#include "Match.h"

#include "../text/Text.h"
#include "../text/Node.h"
#include "../text/attributes/AttributeValue.h"

#include "../patterns/Pattern.h"
#include "../patterns/Alternative.h"
#include "../patterns/matchers/Context.h"

#include <iostream>

using namespace lspl::text::attributes;
using namespace lspl::patterns;

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

Match::Match( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, const Alternative & alternative, const matchers::Context & context ) :
	Transition( MATCH, start, end ), pattern( pattern ), fragments( 0 ) {

	/*
	 * Инициализируем аттрибуты
	 */
	for ( Alternative::BindingMap::const_iterator it = alternative.getBindings().begin(), e = alternative.getBindings().end(); it != e; ++ it ) {
		AttributeKey key = it->first;
		AttributeValue value = it->second->evaluate( *this, context );

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

Match::Match( const text::Node & start, const text::Node & end, const patterns::Pattern & pattern, const AttributesMap & attributes ) :
	Transition( MATCH, start, end ), pattern( pattern ), attributes( attributes ), fragments( 0 ) {
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

AttributeValue Match::getAttribute( AttributeKey key ) const {
	AttributesMap::const_iterator it = attributes.find( key ); // Находим аттрибут в карте

	if ( it != attributes.end() )
		return it->second;

	return Transition::getAttribute( key );
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

bool Match::equals( const Match & match ) const {
	if ( &match.pattern != &pattern || &match.start != &start || &match.end != &end )
		return false; // Если не совпадают шаблоны или узлы сопоставлений, то сопоставления не равны

	AttributesMap::const_iterator self_it = attributes.begin(); // Итератор аттрибутов карты текущего сопоставления
	AttributesMap::const_iterator match_it = match.attributes.begin(); // Итератор аттрибутов карты сравниваемого сопоставления

	while ( ( self_it != attributes.end() ) && ( match_it != match.attributes.end() ) ) {
		if ( self_it->first != match_it->first || self_it->second != match_it->second )
			return false; // Если значения карты не совпадают, то сопоставления не равны

		// Сдвигаем итераторы
		++ self_it;
		++ match_it;
	}

	return ( self_it == attributes.end() ) && ( match_it == match.attributes.end() );
}

} }
