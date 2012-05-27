
#ifndef _LSPL_TRANSFORMS_TEXTTRANSFORM_H_
#define _LSPL_TRANSFORMS_TEXTTRANSFORM_H_

#include "Transform.h"

#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include "../patterns/matchers/Forward.h"

namespace lspl { namespace transforms {

class LSPL_EXPORT TextTransform : public TypedTransform<std::string> {
  public:
	TextTransform();
	virtual ~TextTransform();

    TextTransform(
		boost::ptr_vector<lspl::patterns::matchers::Matcher>* matchers
    ): 
    	matchers( matchers )
    {}
    
    virtual std::string apply( const lspl::text::MatchVariant & v ) const;  
	void buildStr( std::string & str, const lspl::text::MatchVariant & v, const boost::ptr_vector<lspl::patterns::matchers::Matcher> & matchers ) const;
 	void buildStr( std::string & str, const lspl::text::MatchVariant & v, const lspl::patterns::matchers::Matcher & matchers ) const;
//	void buildStr( std::string & str, const lspl::text::LoopIterationList & iterations, const lspl::patterns::matchers::Matcher & matchers ) const;
  
  private :
	boost::ptr_vector<lspl::patterns::matchers::Matcher> *matchers;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_TEXTTRANSFORM_H_ */
