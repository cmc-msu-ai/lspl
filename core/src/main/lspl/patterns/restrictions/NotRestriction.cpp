#include "../../base/BaseInternal.h"

#include "NotRestriction.h"

namespace lspl { namespace patterns { namespace restrictions {

NotRestriction::NotRestriction(Restriction * arg): arg(arg) {
}

NotRestriction::~NotRestriction() {
}

bool NotRestriction::matches( const text::Transition * currentAnnotation, const matchers::Variable currentVar, const matchers::Context & ctx ) const {
	return !arg->matches(currentAnnotation, currentVar, ctx);
}

void NotRestriction::dump( std::ostream & out, const std::string & tabs ) const {
	out << tabs << "!(";
	arg->dump(out);
	out << ")" << std::endl;
}

bool NotRestriction::equals( const Restriction & r ) const {
	try {
		const NotRestriction &nr = dynamic_cast<const NotRestriction &>(r);
		return arg->equals(*nr.arg);
	} catch (...) {
		return false;
	}
}

bool NotRestriction::containsVariable( matchers::Variable var ) const {
	return arg->containsVariable(var);
}

bool NotRestriction::containsVariables() const {
	return arg->containsVariables();
}

bool NotRestriction::containsCurrentAnnotation() const {
	return arg->containsCurrentAnnotation();
}

} } } // namespace lspl::patterns::matchers
