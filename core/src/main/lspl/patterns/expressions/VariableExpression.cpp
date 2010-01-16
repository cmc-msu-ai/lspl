/*
 * VariableExpression.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */
#include <cstdio>
#include <cstdlib>

#include "../../base/BaseInternal.h"
#include "VariableExpression.h"
#include "../../text/Transition.h"
#include "../matchers/Context.h"

using namespace lspl::text::attributes;
using namespace lspl::patterns::matchers;

namespace lspl { namespace patterns { namespace expressions {

VariableExpression::VariableExpression( const matchers::Variable & variable ) :
	variable( variable ) {
}

VariableExpression::VariableExpression( text::attributes::SpeechPart sp, uint index ) :
	variable( Variable( sp, index ) ) {
}

VariableExpression::VariableExpression( const Pattern & pt, uint index ) :
	variable( Variable( pt, index ) ) {
}

VariableExpression::VariableExpression( const std::string &base ) {
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
	int number =
			atoi(base.substr(speechSize, base.size() - speechSize).c_str());
	if (!number) {
		number = 1;
	}
	variable = Variable(speechPart, number);
}

VariableExpression::~VariableExpression() {
}

void VariableExpression::evaluateTo( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const Context & ctx, ValueList & results ) const {
	Context::ConstRange range = ctx.getValues( variable );

	for ( Context::ConstIterator it = range.first; it != range.second; ++ it )
		results.push_back( AttributeValue( *it->second ) );

	if ( currentVar == variable )
		results.push_back( AttributeValue( *currentAnnotation ) );
}

void VariableExpression::dump( std::ostream & out, const std::string & tabs ) const {
	out << variable;
}

bool VariableExpression::equals( const Expression & e ) const {
	if ( const VariableExpression * exp = dynamic_cast<const VariableExpression *>( &e ) ) {
		return exp->variable == variable;
	} else {
		return false;
	}
}

bool VariableExpression::containsVariable( matchers::Variable var ) const {
	return var == variable;
}

bool VariableExpression::containsVariables() const {
	return true;
}

bool VariableExpression::containsCurrentAnnotation() const {
	return false;
}

} } }
