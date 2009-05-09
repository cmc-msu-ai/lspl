#ifndef _LSPL_READERS_JSONTEXTREADER_H_
#define _LSPL_READERS_JSONTEXTREADER_H_

#include "TextReader.h"
#include "../../Namespace.h"

namespace lspl { namespace text { namespace readers {

class LSPL_EXPORT JsonTextReader : public TextReader
{
public:
	JsonTextReader( const NamespaceRef & ns );
	virtual ~JsonTextReader();

	virtual TextRef readFromStream( std::istream & is );
private:
	class Parser;
private:
	NamespaceRef ns;
};

} } } // namespace lspl::text::readers

#endif//_LSPL_READERS_JSONTEXTREADER_H_
