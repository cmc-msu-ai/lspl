
#ifndef _LSPL_TRANSFORMS_PATTERNTRANSFORM_H_
#define _LSPL_TRANSFORMS_PATTERNTRANSFORM_H_

#include "Transform.h"

#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include "../patterns/matchers/Forward.h"

#include <map>

#include "../Namespace.h"

#include "../patterns/Alternative.h"
#include "../text/Forward.h"
#include "../text/attributes/AttributeKey.h"
#include "../text/attributes/AttributeValue.h"
#include "../patterns/matchers/Variable.h"

namespace lspl { namespace transforms {

class LSPL_EXPORT PatternTransform : public TypedTransform<lspl::patterns::PatternRef> {
  public:
	typedef std::multimap< patterns::matchers::Variable, text::TransitionConstRef > Map;
	typedef Map::const_iterator ConstIterator;
	typedef std::pair<ConstIterator,ConstIterator> ConstRange;

	PatternTransform();
	virtual ~PatternTransform();

    PatternTransform(
    	NamespaceRef space,
		boost::ptr_vector<lspl::patterns::matchers::Matcher>* matchers
    ): 
    	ns(space),
    	matchers( matchers )
    {}

	const boost::ptr_vector<lspl::patterns::matchers::Matcher> & getMatchers() const {
		return *matchers;
	}
    
    virtual lspl::patterns::PatternRef apply( const lspl::text::MatchVariant & v ) const;  
    virtual lspl::patterns::PatternRef apply( const lspl::text::MatchVariant & v, unsigned int globalattributes ) const {};  
	void buildStr( std::string & str, const lspl::text::MatchVariant & v, const boost::ptr_vector<lspl::patterns::matchers::Matcher> & matchers ) const;

  private :
	boost::ptr_vector<lspl::patterns::matchers::Matcher> *matchers;
	NamespaceRef ns;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_PATTERNTRANSFORM_H_ */
