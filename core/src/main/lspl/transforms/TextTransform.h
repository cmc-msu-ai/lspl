
#ifndef _LSPL_TRANSFORMS_TEXTTRANSFORM_H_
#define _LSPL_TRANSFORMS_TEXTTRANSFORM_H_

#include "Transform.h"

#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include "../patterns/matchers/Forward.h"

#include <map>

#include "../patterns/Alternative.h"
#include "../text/Forward.h"
#include "../text/attributes/AttributeKey.h"
#include "../text/attributes/AttributeValue.h"
#include "../patterns/matchers/Variable.h"

namespace lspl { namespace transforms {

class LSPL_EXPORT TextTransform : public TypedTransform<std::string> {
  public:
	typedef std::multimap< patterns::matchers::Variable, text::TransitionConstRef > Map;
	typedef Map::const_iterator ConstIterator;
	typedef std::pair<ConstIterator,ConstIterator> ConstRange;

	TextTransform();
	virtual ~TextTransform();

    TextTransform(
		boost::ptr_vector<lspl::patterns::matchers::Matcher>* matchers
    ): 
    	matchers( matchers )
    {}

	const boost::ptr_vector<lspl::patterns::matchers::Matcher> & getMatchers() const {
		return *matchers;
	}
    
    virtual std::string apply( const lspl::text::MatchVariant & v ) const;  
	void buildStr( std::string & str, const lspl::text::MatchVariant & v, const boost::ptr_vector<lspl::patterns::matchers::Matcher> & matchers ) const;

  private :
	boost::ptr_vector<lspl::patterns::matchers::Matcher> *matchers;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_TEXTTRANSFORM_H_ */
