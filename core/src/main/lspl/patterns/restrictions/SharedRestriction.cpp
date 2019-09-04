#include "../../base/BaseInternal.h"

#include "SharedRestriction.h"

namespace lspl { namespace patterns { namespace restrictions {

SharedRestriction::SharedRestriction(Restriction *r) : ptr(r) {
}

SharedRestriction::SharedRestriction(const SharedRestriction &r) : SharedRestriction(nullptr) {
	this->ptr = r.ptr;
}

SharedRestriction::~SharedRestriction() {
}

bool SharedRestriction::matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const {
	return ptr->matches(currentAnnotation, currentVar, ctx);
}

void SharedRestriction::dump( std::ostream & out, const std::string & tabs ) const {
	ptr->dump(out, tabs);
}

bool SharedRestriction::equals( const Restriction & r ) const {
	return ptr->equals(r);
}

bool SharedRestriction::containsVariable( matchers::Variable var ) const {
	return ptr->containsVariable(var);
}

bool SharedRestriction::containsVariables() const {
	return ptr->containsVariables();
}

bool SharedRestriction::containsCurrentAnnotation() const {
	return ptr->containsCurrentAnnotation();
}

} } } // namespace lspl::patterns::restrictions
