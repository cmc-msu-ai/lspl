
#ifndef _LSPL_TRANFORMS_TEXTTRANSFORMBUILDER_H_
#define _LSPL_TRANFORMS_TEXTTRANSFORMBUILDER_H_

#include "../base/Base.h"
#include "../base/RefCountObject.h"

#include <vector>

#include "../Namespace.h"

#include "Transform.h"
#include "TransformBuilder.h"
#include "TextTransform.h"

#include "../patterns/Forward.h"
#include "../patterns/matchers/Forward.h"

namespace lspl { namespace transforms {

class TextTransformBuilder : public lspl::transforms::TransformBuilder {  
  public:
    TextTransformBuilder(NamespaceRef space) : space( space ) {};
    ~TextTransformBuilder() {};
    
    TextTransform * build( const lspl::patterns::Alternative & alt, const std::string & source );
  public:
	NamespaceRef space;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANFORMS_TRANSFORMBUILDER_H_ */
