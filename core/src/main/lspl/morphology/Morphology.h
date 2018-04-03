#ifndef _LSPL_MORPHOLOGY_MORPHOLOGY_H_
#define _LSPL_MORPHOLOGY_MORPHOLOGY_H_

#include "../base/Base.h"
#include "../base/Exception.h"

#include "WordForm.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace morphology {

class MorphologyInitException : public base::Exception{
public:
	MorphologyInitException(const std::string & description, const std::string & sourceFileName, int sourceLineNumber) :
		Exception( description, sourceFileName, sourceLineNumber) {}
};

class LSPL_EXPORT Morphology {
public:
	static text::attributes::AttributeValue extractAttributeValue( uint64 attributeSet, text::attributes::AttributeKey key );

	/**
	 * Получить экземпляр морфологии
	 */
	static Morphology & instance();

	/**
	 * Завершить работу модуля морфологии
	 */
	static void shutdown();

private:
	static Morphology * singletonInstance;

public:
	Morphology();
	virtual ~Morphology();

	/**
	 * Провести морфологический анализ и получить различные интерпретации слова.
	 *
	 * @param token слово для анализа
	 * @param forms ptr_vector для интерпретаций слова
	 */
	virtual void appendWordForms( const std::string & token, boost::ptr_vector<WordForm> & forms ) = 0;

	/**
	 * Провести морфологический анализ и получить различные интерпретации слова.
	 *
	 * @param token слово для анализа
	 * @return ptr_vector интерпретаций слова
	 */
	std::auto_ptr< boost::ptr_vector<WordForm> > getWordForms( const std::string & token ) {
		boost::ptr_vector<WordForm> result;
		appendWordForms( token, result );
		return result.release();
	}

	virtual std::string getAttributesString( uint64 attValues ) = 0;

	std::string upcase( const std::string & str ) { return upcase( str.c_str() ); }
	virtual std::string upcase( const char * str ) = 0;
	virtual std::string upcase( const char * start, const char * end ) = 0;

	std::string lowcase( const std::string & str ) { return lowcase( str.c_str() ); }
	virtual std::string lowcase( const char * str ) = 0;
	virtual std::string lowcase( const char * start, const char * end ) = 0;

};

} } // namespace lspl::morphology

#endif /*_LSPL_MORPHOLOGY_MORPHOLOGY_H_*/
