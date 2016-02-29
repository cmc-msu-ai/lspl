#ifndef _LSPL_PATTERNS_PATTERNBUILDER_H_
#define _LSPL_PATTERNS_PATTERNBUILDER_H_

#include "../base/Base.h"
#include "../base/RefCountObject.h"
#include "../base/Exception.h"

#include "../Namespace.h"
#include "../transforms/TransformBuilder.h"

#include <string>
#include <map>

#include <boost/scoped_ptr.hpp>

namespace lspl { namespace patterns {

class LSPL_EXPORT PatternBuildingException : public base::Exception {
public:
	PatternBuildingException(const std::string & description) :
		Exception( description ) {
	}

	~PatternBuildingException() throw() {}
};

class LSPL_EXPORT PatternBuilder : public base::RefCountObject, public base::IdentifiedObject<PatternBuilder> {
public:

	/**
	 * Класс, представляющий информацию о результате построения шаблонов из исходника
	 */
	class BuildInfo {
	public:
		/**
		 * Длина успешно разобранной строки символов
		 */
		uint parseLength;

		/**
		 * Неразобранная строка
		 */
		std::string parseTail;
	};

	/**
	 * Базовый класс парсера выражений
	 */
	class Parser {
	public:
		Parser( NamespaceRef space, const std::map<std::string, transforms::TransformBuilderRef>& transformBuilders ) : space( space ), transformBuilders( transformBuilders ) {}
		virtual ~Parser() {}

		virtual BuildInfo build( const char * str ) throw (PatternBuildingException) = 0;
	public:
		NamespaceRef space;
		const std::map<std::string, transforms::TransformBuilderRef>& transformBuilders;
	};

public:
	PatternBuilder( const NamespaceRef & ns = new Namespace() );
	virtual ~PatternBuilder();

	/**
	 * Определить новые шаблоны из исходника
	 */
	BuildInfo build( const std::string & str ) throw (PatternBuildingException);

public:

	NamespaceRef space;
	std::map<std::string, transforms::TransformBuilderRef> transformBuilders;

private:

	/**
	 * Текущий парсер билдера
	 */
	boost::scoped_ptr<Parser> parser;
};

} } // namespace lspl::patterns

#endif /*_LSPL_PATTERNS_PATTERNBUILDER_H_*/
