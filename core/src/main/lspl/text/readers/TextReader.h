#ifndef _LSPL_TEXT_READERS_TEXTREADER_H_
#define _LSPL_TEXT_READERS_TEXTREADER_H_

#include "../../base/Base.h"

#include "../Text.h"

#include <iostream>

namespace lspl { namespace text { namespace readers {

class LSPL_EXPORT TextReader {
public:
	TextReader();
	virtual ~TextReader();

	virtual TextRef readFromStream( std::istream & is ) = 0;
	virtual TextRef readFromStream( std::istream & is, const std::string & enc ) = 0;
};

} } }

#endif /*_LSPL_TEXT_READERS_TEXTREADER_H_*/
