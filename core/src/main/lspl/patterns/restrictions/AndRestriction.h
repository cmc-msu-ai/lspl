/*
 * AndRestriction.h
 *
 *  Created on: Dec 6, 2009
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_RESTRICTIONS_ANDRESTRICTION_H_
#define _LSPL_PATTERNS_RESTRICTIONS_ANDRESTRICTION_H_

#include "Restriction.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace patterns { namespace restrictions {

class LSPL_EXPORT AndRestriction : public Restriction {
public:
	AndRestriction();
	virtual ~AndRestriction();

	void addArgument( Restriction * arg ) {
		args.push_back( arg );
	}

	template <class PtrContainer>
	void addArguments( PtrContainer & r ) {
		if ( r.begin() != r.end() )
			args.transfer( args.end(), r.begin(), r.end(), r );
	}

	virtual bool matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Restriction & r ) const;
	virtual bool containsVariable( matchers::Variable var ) const;
	virtual bool containsCurrentAnnotation() const;

private:

	boost::ptr_vector<Restriction> args;

};

} } } // namespace lspl::patterns::matchers

#endif /* _LSPL_PATTERNS_RESTRICTIONS_ANDRESTRICTION_H_ */
