/*
 * AgreementRestriction.cpp
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */
#include "../../base/BaseInternal.h"
#include "../../text/Transition.h"
#include "../../text/attributes/AttributeKey.h"
#include "../matchers/Context.h"
#include "../matchers/Matcher.h"
#include "AgreementRestriction.h"

#include <stdexcept>
#include <iostream>

using namespace lspl::text::attributes;

using lspl::patterns::expressions::Expression;

namespace lspl { namespace patterns { namespace restrictions {

AgreementRestriction::AgreementRestriction(bool weak): weak(weak) {
}

AgreementRestriction::~AgreementRestriction() {
}

bool AgreementRestriction::matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const {
	if ( args.size() <= 1 )
		throw std::logic_error( "Too less arguments" );

	Expression::ValueList val1, val2;

	Expression::ValueList * v1 = &val1;
	Expression::ValueList * v2 = &val2;

	args[0].evaluateTo( currentAnnotation, currentVar, ctx, *v2 );

	for ( uint i = 1; i < args.size(); ++i ) {
		Expression::ValueList * v = v2;
		v2 = v1;
		v1 = v;

		v2->clear();
		args[i].evaluateTo( currentAnnotation, currentVar, ctx, *v2 );

		if ( !checkAgreement( *v1, *v2 ) )
			return false;
	}

	return true;
}

void AgreementRestriction::dump( std::ostream & out, const std::string & tabs ) const {
	if ( args.size() <= 0 )
		throw std::logic_error( "Too less arguments" );

	args[0].dump( out );

	for ( uint i = 1; i < args.size(); ++i ) {
		out << (weak ? " = " : " == ");
		args[i].dump( out );
	}
}

bool AgreementRestriction::checkAgreement( const std::vector<AttributeValue> & val1, const std::vector<AttributeValue> & val2 ) const {
	for( AttributeValue v1 : val1 )
		for( AttributeValue v2 : val2 )
			if ( !checkAgreement( v1, v2 ) )
				return false;

	return true;
}

bool AgreementRestriction::checkAgreement( AttributeValue val1, AttributeValue val2 ) const {
	if ( val1 == AttributeValue::UNDEFINED || val2 == AttributeValue::UNDEFINED )
		return weak || (val1 == val2);

	if ( val1.type != val2.type )
		return false;

	switch ( val1.type.id ) {
		case AttributeType::INDEXED_ID:
			return val1.value == val2.value;
		case AttributeType::STRING_ID:
			return val1.getString() == val2.getString();
		case AttributeType::COMPOUND_ID: {
			const AttributeContainer & cont1 = val1.getContainer();
			const AttributeContainer & cont2 = val2.getContainer();

			for ( uint i = 0; i < AttributeKey::count(); ++ i ) {
				AttributeValue sv1 = cont1.getAttribute( AttributeKey(i) );

				if ( weak && sv1 == AttributeValue::UNDEFINED )
					continue;

				AttributeValue sv2 = cont2.getAttribute( AttributeKey(i) );

				if ( weak && sv2 == AttributeValue::UNDEFINED )
					continue;

				if ( sv1.type == AttributeType::STRING && sv2.type == AttributeType::STRING )
					continue;

				if ( !checkAgreement( sv1, sv2 ) )
					return false;
			}

			return true;
		}
		default:
			throw std::logic_error( "Unknown attribute type" );
	}
}

bool AgreementRestriction::equals( const Restriction & r ) const {
	if ( const AgreementRestriction * ar = dynamic_cast<const AgreementRestriction *>( &r ) ) {
		if ( ar->args.size() != args.size() ) return false;

		for ( uint i = 0, l = args.size(); i < l; ++ i )
			if ( !ar->args[i].equals( args[i] ) )
				return false;

		return true;
	} else {
		return false;
	}
}

bool AgreementRestriction::containsVariable( matchers::Variable var ) const {
	for( boost::ptr_vector<Expression>::const_iterator it = args.begin(); it != args.end(); ++ it )
		if ( it->containsVariable( var ) )
			return true;

	return false;
}

bool AgreementRestriction::containsVariables() const {
	for( boost::ptr_vector<Expression>::const_iterator it = args.begin(); it != args.end(); ++ it )
		if ( it->containsVariables() )
			return true;

	return false;
}

bool AgreementRestriction::containsCurrentAnnotation() const {
	for( boost::ptr_vector<Expression>::const_iterator it = args.begin(); it != args.end(); ++ it )
		if ( it->containsCurrentAnnotation() )
			return true;

	return false;
}

} } }
