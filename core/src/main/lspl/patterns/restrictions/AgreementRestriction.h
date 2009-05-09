/*
 * AgreementRestriction.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_RESTRICTIONS_AGREEMENTRESTRICTION_H_
#define _LSPL_PATTERNS_RESTRICTIONS_AGREEMENTRESTRICTION_H_

#include "../../base/Base.h"
#include "Restriction.h"

#include "../matchers/Variable.h"
#include "../expressions/Expression.h"
#include "../../text/attributes/AttributeKey.h"
#include "../../text/attributes/AttributeValue.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns { namespace restrictions {

class LSPL_EXPORT AgreementRestriction : public Restriction {
public:
	AgreementRestriction();
	virtual ~AgreementRestriction();

	void addArgument( expressions::Expression * arg ) {
		args.push_back( arg );
	}

	template <class PtrContainer>
	void addArguments( PtrContainer & r ) {
		if ( r.begin() != r.end() )
			args.transfer( args.end(), r.begin(), r.end(), r );
	}

	bool matches( const text::Transition & annotation, const matchers::Context & ctx ) const;
	void dump( std::ostream & out, const std::string & tabs = "" ) const;
private:
	bool checkAgreement( text::attributes::AttributeValue val1, text::attributes::AttributeValue val2 ) const;
private:
	boost::ptr_vector<expressions::Expression> args;
};

} } }

#endif//_LSPL_PATTERNS_RESTRICTIONS_AGREEMENTRESTRICTION_H_
