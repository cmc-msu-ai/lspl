#ifndef _LSPL_PATTERNS_MATCHERS_CONTEXT_H_
#define _LSPL_PATTERNS_MATCHERS_CONTEXT_H_

#include "Forward.h"
#include "Variable.h"

#include "../Alternative.h"
#include "../../base/RefCountPtr.h"
#include "../../text/Forward.h"
#include "../../text/attributes/AttributeKey.h"
#include "../../text/attributes/AttributeValue.h"

#include <vector>
#include <map>

namespace lspl { namespace patterns { namespace matchers {

class LSPL_EXPORT Context {
public:
	Context();
	~Context();

	void setVariable( Variable variable, const text::TransitionConstRef & value ){
		map.insert( std::make_pair( variable, value ) );
	}

	text::attributes::AttributeValue getAttribute( Variable variable, text::attributes::AttributeKey attribute ) const;
	text::attributes::AttributeValue getVariable( Variable variable ) const;

	void addAttributes( std::map<text::attributes::AttributeKey,text::attributes::AttributeValue> & atts, const Alternative::BindingMap & bindings ) const;

	bool operator == ( const Context & ctx ) const {
		return map == ctx.map;
	}

private:
	typedef std::map< Variable, text::TransitionConstRef > Map;
private:
	Map map;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_PATTERNS_MATCHERS_CONTEXT_H_*/
