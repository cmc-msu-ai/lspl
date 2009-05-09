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
#include "AgreementRestriction.h"

#include <stdexcept>
#include <iostream>

using namespace lspl::text::attributes;

namespace lspl { namespace patterns { namespace restrictions {

AgreementRestriction::AgreementRestriction() {
}

AgreementRestriction::~AgreementRestriction() {
}

bool AgreementRestriction::matches( const text::Transition & annotation, const matchers::Context & ctx ) const {
	if ( args.size() <= 1 )
		throw std::logic_error( "Too less arguments" );

	AttributeValue val1;
	AttributeValue val2 = args[0].evaluate( annotation, ctx );

	for ( uint i = 1; i < args.size(); ++i ) {
		val1 = val2;
		val2 = args[i].evaluate( annotation, ctx );

		if ( !checkAgreement( val1, val2 ) )
			return false;
	}

	return true;
}

void AgreementRestriction::dump( std::ostream & out, const std::string & tabs ) const {
	if ( args.size() <= 1 )
		throw std::logic_error( "Too less arguments" );

	args[0].dump( out );

	for ( uint i = 1; i < args.size(); ++i ) {
		out << " ~ ";
		args[i].dump( out );
	}
}

bool AgreementRestriction::checkAgreement( AttributeValue val1, AttributeValue val2 ) const {
	if ( val1 == AttributeValue::UNDEFINED || val2 == AttributeValue::UNDEFINED )
		return true;

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

				if ( sv1 == AttributeValue::UNDEFINED )
					continue;

				AttributeValue sv2 = cont2.getAttribute( AttributeKey(i) );

				if ( sv2 == AttributeValue::UNDEFINED )
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

} } }
