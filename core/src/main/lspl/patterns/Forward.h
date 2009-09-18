#ifndef _LSPL_PATTERNS_FORWARD_H_
#define _LSPL_PATTERNS_FORWARD_H_

#include "../base/Base.h"
#include "../base/RefCountPtr.h"

#include <vector>

namespace lspl { namespace patterns {

class Alternative;

LSPL_REFCOUNT_FORWARD( Pattern );
LSPL_REFCOUNT_FORWARD( PatternBuilder );

} } // namespace lspl::patterns

#endif /*_LSPL_PATTERNS_FORWARD_H_*/
