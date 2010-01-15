#ifndef _LSPL_PATTERNS_MATCHERS_CONTEXT_H_
#define _LSPL_PATTERNS_MATCHERS_CONTEXT_H_

#include "Forward.h"
#include "Variable.h"

#include "../Alternative.h"
#include "../../text/Forward.h"
#include "../../text/attributes/AttributeKey.h"
#include "../../text/attributes/AttributeValue.h"

#include <vector>
#include <map>

namespace lspl { namespace patterns { namespace matchers {

class LSPL_EXPORT Context {
public:
	typedef std::multimap< Variable, text::TransitionConstRef > Map;
	typedef Map::const_iterator ConstIterator;
	typedef std::pair<ConstIterator,ConstIterator> ConstRange;
public:
	Context();
	~Context();

	void addValue( Variable var, const text::TransitionConstRef & value );

	ConstRange getValues( Variable var ) const;

	void addAttributes( std::map<text::attributes::AttributeKey,text::attributes::AttributeValue> & atts, const Alternative::BindingMap & bindings ) const;

	bool operator == ( const Context & ctx ) const {
		return map == ctx.map;
	}

private:

private:
	Map map;
};

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_PATTERNS_MATCHERS_CONTEXT_H_*/
