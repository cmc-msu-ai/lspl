
#ifndef _LSPL_TRANFORMS_PATTERNTRANSFORMBUILDER_H_
#define _LSPL_TRANFORMS_PATTERNTRANSFORMBUILDER_H_

#include "../base/Base.h"
#include "../base/RefCountObject.h"

#include <vector>

#include "../Namespace.h"

#include "Transform.h"
#include "TransformBuilder.h"
#include "PatternTransform.h"

#include "../patterns/Forward.h"
#include "../patterns/matchers/Forward.h"

namespace lspl { namespace transforms {

class LSPL_EXPORT PatternTransformBuilder : public lspl::transforms::TransformBuilder {
  public:
    PatternTransformBuilder(NamespaceRef space);
    virtual ~PatternTransformBuilder() {};

    PatternTransform * build( const lspl::patterns::Alternative & alt, const std::string & source );
  public:
	NamespaceRef space;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANFORMS_PATTERNTRANSFORMBUILDER_H_ */
