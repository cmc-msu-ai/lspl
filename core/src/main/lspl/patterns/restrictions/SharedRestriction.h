#ifndef _LSPL_PATTERNS_RESTRICTIONS_SHAREDRESTRICTION_H_
#define _LSPL_PATTERNS_RESTRICTIONS_SHAREDRESTRICTION_H_

#include <memory>
#include "Restriction.h"

namespace lspl { namespace patterns { namespace restrictions {

class LSPL_EXPORT SharedRestriction : public Restriction {

public:
	SharedRestriction( Restriction * arg );
	SharedRestriction( const SharedRestriction &r );
	virtual ~SharedRestriction();

	virtual bool matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const;
	virtual void dump( std::ostream & out, const std::string & tabs = "" ) const;
	virtual bool equals( const Restriction & r ) const;

	virtual bool containsVariable( matchers::Variable var ) const;
	virtual bool containsVariables() const;
	virtual bool containsCurrentAnnotation() const;

private:
    std::shared_ptr<Restriction> ptr;
};

} } } // namespace lspl::patterns::restrictions

#endif /* _LSPL_PATTERNS_RESTRICTIONS_SHAREDRESTRICTION_H_ */
