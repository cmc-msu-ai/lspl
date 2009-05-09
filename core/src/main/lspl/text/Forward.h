#ifndef _LSPL_TEXT_FORWARD_H_
#define _LSPL_TEXT_FORWARD_H_

#include "../base/RefCountPtr.h"

#include <vector>

namespace lspl { namespace text {

class TextConfig;
class TextBuilder;

LSPL_REFCOUNT_FORWARD( Text );
LSPL_REFCOUNT_FORWARD( Node );
LSPL_REFCOUNT_FORWARD( Transition );
LSPL_REFCOUNT_FORWARD( Match );

namespace markup {

LSPL_REFCOUNT_FORWARD( Token );
LSPL_REFCOUNT_FORWARD( Word );

}

} }

#endif /*_LSPL_TEXT_FORWARD_H_*/
