/*
 * Forward.h
 *
 *  Created on: Dec 24, 2008
 *      Author: alno
 */

#ifndef _LSPL_MORPHOLOGY_FORWARD_H_
#define _LSPL_MORPHOLOGY_FORWARD_H_

#include "../base/RefCountObject.h"

#include <vector>

namespace lspl { namespace morphology {

class WordForm;
class Morphology;

typedef base::RefCountObject<WordForm> WordFormPtr;
typedef std::vector<WordFormPtr> WordFormList;

} }

#endif /* _LSPL_MORPHOLOGY_FORWARD_H_ */
