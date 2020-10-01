#ifndef _LSPL_PATTERNS_RESTRICTIONS_NOTRESTRICTION_H_
#define _LSPL_PATTERNS_RESTRICTIONS_NOTRESTRICTION_H_

#include "Restriction.h"

namespace lspl { namespace patterns { namespace restrictions {

class LSPL_EXPORT NotRestriction : public Restriction {
public:
	NotRestriction( Restriction * arg );
	virtual ~NotRestriction();

	virtual bool matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Restriction & r ) const;

	virtual bool containsVariable( matchers::Variable var ) const;
	virtual bool containsVariables() const;
	virtual bool containsCurrentAnnotation() const;

private:

	Restriction* arg;

};

} } } // namespace lspl::patterns::restrictions

#endif /* _LSPL_PATTERNS_RESTRICTIONS_NOTRESTRICTION_H_ */
