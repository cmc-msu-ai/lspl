#ifndef _LSPL_MORPHOLOGY_MORPHOLOGY_H_
#define _LSPL_MORPHOLOGY_MORPHOLOGY_H_

#include "../base/Base.h"
#include "../base/Exception.h"

#include "WordForm.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace morphology {

class LSPL_EXPORT MorphologyInitException : public base::Exception{
public:
	MorphologyInitException(const std::string & description, const std::string & sourceFileName, int sourceLineNumber) :
		Exception( description, sourceFileName, sourceLineNumber) {}
};

class LSPL_EXPORT Morphology
{

public:
	static text::attributes::AttributeValue extractAttributeValue( uint64 attributeSet, text::attributes::AttributeKey key );

	static Morphology & instance();
	static void shutdown();

private:
	static Morphology * singletonInstance;

public:
	Morphology();
	virtual ~Morphology();

	virtual void appendWordForms( std::string token, boost::ptr_vector<WordForm> & forms ) = 0;

	virtual std::string getAttributesString( uint64 attValues ) = 0;

	std::string upcase( const std::string & str ) { return upcase( str.c_str() ); }
	virtual std::string upcase( const char * str ) = 0;

	std::string lowcase( const std::string & str ) { return lowcase( str.c_str() ); }
	virtual std::string lowcase( const char * str ) = 0;

};

} } // namespace lspl::morphology

#endif /*_LSPL_MORPHOLOGY_MORPHOLOGY_H_*/
