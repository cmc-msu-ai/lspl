
#include "../base/BaseInternal.h"

#include "TransformBuilder.h"
#include "TextTransformBuilder.h"
#include "TextTransform.h"
#include "../patterns/matchers/Matcher.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include "../patterns/TextTransformParser.h"
#include <boost/spirit/include/phoenix1.hpp>

#include "../Namespace.h"

#include <iostream>

LSPL_REFCOUNT_CLASS( lspl::transforms::TextTransformBuilder )

using namespace phoenix;

namespace lspl { namespace transforms {

TextTransformBuilder::TextTransformBuilder(NamespaceRef space) : space( space ) {
}

TextTransform * TextTransformBuilder::build(
  const lspl::patterns::Alternative & alt,
  const std::string & source )
{
	lspl::patterns::TextTransformParser pars(space);
	boost::ptr_vector<lspl::patterns::matchers::Matcher>* matchers=NULL;

	parse_info<const char *> pi = boost::spirit::classic::parse( source.c_str(), pars[ var(matchers) = arg1 ], space_p );

	if(!pi.full) {
		throw lspl::patterns::PatternBuildingException( source.c_str() );
	}

    return new TextTransform( matchers, alt.getBindings() );
}


} } // namespace lspl::transforms
