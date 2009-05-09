#ifndef _LSPL_TEXT_WRITERS_JSONTEXTWRITER_H_
#define _LSPL_TEXT_WRITERS_JSONTEXTWRITER_H_

#include "../../base/Base.h"
#include "TextWriter.h"
#include "../attributes/AttributeValue.h" 

namespace lspl { namespace text { namespace writers {

class LSPL_EXPORT JsonTextWriter : public TextWriter
{
public:
	JsonTextWriter();
	virtual ~JsonTextWriter();
	
	void writeToStream( const Text & text, std::ostream & os );
private:
	void writeConfig( const TextConfig & config, std::ostream & os );
	void writeNode( const Node & node, std::ostream & os );
	void writeAnnotation( const Transition & ann, std::ostream & os );
	void writeAttributeValue( const attributes::AttributeValue & value, std::ostream & os );
};

} } } // namespace lspl::text::writers

#endif//_LSPL_TEXT_WRITERS_JSONTEXTWRITER_H_
