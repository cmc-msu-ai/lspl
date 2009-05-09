#ifndef _LSPL_TEXT_READERS_PLAINTEXTREADER_H_
#define _LSPL_TEXT_READERS_PLAINTEXTREADER_H_

#include "../../base/Base.h"

#include "../Forward.h"
#include "../TextConfig.h"

#include "TextReader.h"

#include <string>

class CGraLine;

namespace lspl { namespace morphology {

class WordForm;

LSPL_REFCOUNT_FORWARD(WordForm);

} }

namespace lspl { namespace text { namespace readers {

class LSPL_EXPORT PlainTextReader : public lspl::text::readers::TextReader
{
public:
	PlainTextReader();
	virtual ~PlainTextReader();

	TextRef readFromStream( std::istream & is );
	TextRef readFromString( const std::string & str );
public:
	TextConfig config;
private:
	void addTransitions( TextBuilder & builder, Node & start, Node & end, const CGraLine & unit, morphology::WordFormList & forms );
};

} } }

#endif /*_LSPL_TEXT_READERS_PLAINTEXTREADER_H_*/
