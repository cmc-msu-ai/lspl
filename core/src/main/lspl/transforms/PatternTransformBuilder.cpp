
#include "../base/BaseInternal.h"

#include "TransformBuilder.h"
#include "PatternTransformBuilder.h"
#include "PatternTransform.h"
#include "../patterns/matchers/Matcher.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include "../patterns/PatternTransformParser.h"
#include <boost/spirit/include/phoenix1.hpp>

#include "../Namespace.h"

#include <iostream>

LSPL_REFCOUNT_CLASS( lspl::transforms::PatternTransformBuilder )

using namespace phoenix;

namespace lspl { namespace transforms {

PatternTransformBuilder::PatternTransformBuilder(NamespaceRef space) : space( space ) {
}

PatternTransform * PatternTransformBuilder::build(
  const lspl::patterns::Alternative & alt,
  const std::string & source )
{
	lspl::patterns::PatternTransformParser pars(space);
	boost::ptr_vector<lspl::patterns::matchers::Matcher>* matchers=NULL;
	if(!boost::spirit::classic::parse( source.c_str(), pars[ var(matchers) = arg1 ], space_p ).full) {
		throw lspl::patterns::PatternBuildingException( source.c_str() );
	}

	return new PatternTransform( space, matchers );
}


} } // namespace lspl::transforms
