/*
 * Normalization.cpp
 *
 *  Created on: Sep 28, 2009
 *      Author: alno
 */

#include "Normalization.h"

#include "../text/Loop.h"
#include "../text/Match.h"
#include "../text/markup/Token.h"
#include "../text/markup/Word.h"
#include "../morphology/Morphology.h"
#include "../morphology/WordForm.h"

using lspl::text::markup::Word;
using lspl::text::markup::Token;
using lspl::text::Loop;
using lspl::text::Match;
using lspl::text::MatchVariant;
using lspl::text::Transition;
using lspl::text::TransitionList;

namespace lspl { namespace transforms {

Normalization::Normalization() {
}

Normalization::~Normalization() {
}

std::string Normalization::apply( const MatchVariant & matchVariant ) const {
	std::string result;
	appendToString( result, matchVariant );
	return result;
}

std::string Normalization::normalize( const MatchVariant & matchVariant ) const {
	std::string result;
	normalizeToString( result, matchVariant );
	return result;
}

void Normalization::appendToString( std::string & str, const Transition & transition ) const {
	if ( const Token * token = dynamic_cast<const Token*>( &transition ) ) {
		if ( str.length() > 0 )
			str += " ";
		str += token->getToken();
	} else if ( const Word * word = dynamic_cast<const Word*>( &transition ) ) {
		if ( str.length() > 0 )
			str += " ";
		str += word->getBase();
	} else if ( const Loop * loop = dynamic_cast<const Loop*>( &transition ) ) {
		appendToString( str, loop->getIterations() );
	} else if ( const Match * match = dynamic_cast<const Match*>( &transition ) ) {
		appendToString( str, match->getVariants().at( 0 ) );
	}
}

void Normalization::appendToString( std::string & str, const text::LoopIterationList & iterations ) const {
	for ( uint i = 0; i < iterations.size(); ++ i )
		appendToString( str, iterations[ i ]->getVariant( 0 ) );
}

void Normalization::appendToString( std::string & str, const TransitionList & transitions ) const {
	for ( uint i = 0; i < transitions.size(); ++ i )
		appendToString( str, *transitions[ i ] );
}

void Normalization::normalizeToString( std::string & str, const Transition & transition ) const {
	if ( const Token * token = dynamic_cast<const Token*>( &transition ) ) {
		boost::ptr_vector<morphology::WordForm> forms;
		morphology::Morphology::instance().appendWordForms(token->getToken(), forms);
		if ( str.length() > 0 )
			str += " ";
		if (forms.size()) {
			str += forms[0].getBase();
		} else {
			str += token->getToken();
		}
	} else if ( const Word * word = dynamic_cast<const Word*>( &transition ) ) {
		boost::ptr_vector<morphology::WordForm> forms;
		morphology::Morphology::instance().appendWordForms(word->getBase(), forms);
		if ( str.length() > 0 )
			str += " ";
		str += forms[0].getBase();
	} else if ( const Loop * loop = dynamic_cast<const Loop*>( &transition ) ) {
		normalizeToString( str, loop->getIterations() );
	} else if ( const Match * match = dynamic_cast<const Match*>( &transition ) ) {
		normalizeToString( str, match->getVariants().at( 0 ) );
	}
}

void Normalization::normalizeToString( std::string & str, const text::LoopIterationList & iterations ) const {
	for ( uint i = 0; i < iterations.size(); ++ i )
		normalizeToString( str, iterations[ i ]->getVariant( 0 ) );
}

void Normalization::normalizeToString( std::string & str, const TransitionList & transitions ) const {
	for ( uint i = 0; i < transitions.size(); ++ i )
		normalizeToString( str, *transitions[ i ] );
}

} } // namespace lspl::transforms
