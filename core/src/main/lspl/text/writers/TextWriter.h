#ifndef _LSPL_TEXT_WRITERS_TEXTWRITER_H_
#define _LSPL_TEXT_WRITERS_TEXTWRITER_H_

#include "../../base/Base.h"
#include "../Forward.h"

#include <iostream>

namespace lspl { namespace text { namespace writers {

class LSPL_EXPORT TextWriter {
public:
	TextWriter();
	virtual ~TextWriter();
	
	virtual void writeToStream( const Text & text, std::ostream & os ) = 0;
};

} } }

#endif /*_LSPL_TEXT_WRITERS_TEXTWRITER_H_*/
