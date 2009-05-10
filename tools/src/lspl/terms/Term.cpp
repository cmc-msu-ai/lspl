/*
 * Term.cpp
 *
 *  Created on: Dec 24, 2008
 *      Author: alno
 */

#include "Term.h"

#include <sstream>
#include <iostream>

#include <lspl/morphology/Morphology.h>

using namespace lspl::morphology;

using lspl::text::attributes::SpeechPart;

namespace lspl { namespace terms {

uint Term::aliveCount = 0;

Term::Term() {
	++aliveCount;
}

Term::~Term() {
	--aliveCount;
}

std::string Term::buildPattern() const {
	std::ostringstream out;

	appendPatternTo( out );
	return out.str();
}

void Term::appendPatternTo( std::ostream & out ) const {
	std::vector<std::pair<SpeechPart,uint> > ctx;

	appendPatternTo( out, ctx );
}

void Term::appendRestrictionsTo( std::ostream & out, std::vector<std::pair<SpeechPart,uint> > & ctx, uint begin, uint end ) const {
	int firstA = -1;

	for ( uint i = begin; i < end; ++ i ) {
		const std::pair<lspl::text::attributes::SpeechPart,uint> & w = ctx[i];

		if ( w.first == SpeechPart::NOUN && firstA >= 0 ) {
			for ( uint j = firstA; j < i; ++ j ) {
				const std::pair<lspl::text::attributes::SpeechPart,uint> & a = ctx[j];

				out << " <" << a.first.getAbbrevation() << a.second << "=" << w.first.getAbbrevation() << w.second << ">";
			}
		}

		if ( w.first == SpeechPart::ADJECTIVE || w.first == SpeechPart::PARTICIPLE ) {
			if ( firstA < 0 )
				firstA = i;
		} else {
			firstA = -1;
		}
	}
}

Token::Token(const std::string & token) :
	token( token ) {

}

void Token::appendPatternTo( std::ostream & out, std::vector<std::pair<SpeechPart,uint> > & ctx ) const {
	out << "\"" << token << "\"";
}

bool Token::needChildren() const {
	return false;
}

Word::Word(const std::string & token) {
	Morphology & morphology = Morphology::instance();
	boost::ptr_vector<WordForm> wordForms;

	morphology.appendWordForms( token, wordForms );

	speechPart = wordForms.empty() ? SpeechPart::NOUN : wordForms[0].getSpeechPart();
	base = morphology.lowcase( wordForms[0].getBase() );
}

void Word::appendPatternTo( std::ostream & out, std::vector<std::pair<SpeechPart,uint> > & ctx ) const {
	uint num = ctx.size() + 1;

	ctx.push_back( std::make_pair( speechPart, num ) );

	out << speechPart.getAbbrevation() << num << "<" << base << ">";
}

bool Word::needChildren() const {
	return speechPart != SpeechPart::NOUN;
}

Group::Group( TermPtr first, bool optionalChildren ) :
	optionalChildren( optionalChildren ), first( first ) {

}

void Group::appendPatternTo( std::ostream & out, std::vector<std::pair<SpeechPart,uint> > & ctx ) const {
	first->appendPatternTo( out, ctx );

	if ( children.empty() )
		return;

	out << " ";

	if ( optionalChildren && !first->needChildren() ) {
		out << "[ ";

		appendChildrenTo( out, ctx );

		out << " ]";
	} else if ( children.size() > 1 ) {
		out << "{ ";

		appendChildrenTo( out, ctx );

		out << " }<1,1>";
	} else {
		appendChildTo( out, ctx, children[0] );
	}
}

void Group::appendChildrenTo( std::ostream & out, std::vector<std::pair<SpeechPart,uint> > & ctx ) const {

	appendChildTo( out, ctx, children[0] );

	for ( uint i = 1, sz = children.size(); i < sz; ++ i ) {
		out << " | ";

		appendChildTo( out, ctx, children[i] );;
	}
}

void Group::appendChildTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx, const TermPtr & child ) const {
	uint ctxResetSize = ctx.size();

	child->appendPatternTo( out, ctx );

	uint ctxSize = ctx.size();

	if ( ctxSize > ctxResetSize ) {
		appendRestrictionsTo( out, ctx, ctxResetSize > 0 ? ctxResetSize - 1 : ctxResetSize, ctxSize );

		ctx.erase( ctx.begin() + ctxResetSize, ctx.end() );
	}
}

bool Group::needChildren() const {
	return false;
}

Sequence::Sequence() {
}

void Sequence::appendPatternTo( std::ostream & out, std::vector<std::pair<SpeechPart,uint> > & ctx ) const {
	if ( children.empty() )
		return;

	children[0]->appendPatternTo( out, ctx );

	for ( uint i = 1, sz = children.size(); i < sz; ++ i ) {
		out << " ";
		children[i]->appendPatternTo( out, ctx );
	}
}

bool Sequence::needChildren() const {
	return false;
}

} }
