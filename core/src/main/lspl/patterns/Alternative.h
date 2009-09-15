#ifndef _LSPL_PATTERNS_ALTERNATIVE_H_
#define _LSPL_PATTERNS_ALTERNATIVE_H_

#include "Forward.h"

#include "../base/Base.h"

#include "../text/attributes/SpeechPart.h"
#include "../text/attributes/AttributeKey.h"

#include "matchers/Forward.h"
#include "matchers/MatcherContainer.h"

#include "expressions/Expression.h"

#include <boost/ptr_container/ptr_map.hpp>

namespace lspl { namespace patterns {

class LSPL_EXPORT IndexInfo {
public:
	enum Type {
		WORD,
		PATTERN
	};
public:
	IndexInfo( Type type ) : type( type ) {}
public:
	Type type;
};

class LSPL_EXPORT WordIndexInfo : public IndexInfo {
public:
	WordIndexInfo( text::attributes::SpeechPart speechPart ) : IndexInfo( WORD ), speechPart( speechPart ) {}
public:
	text::attributes::SpeechPart speechPart;
};

class LSPL_EXPORT PatternIndexInfo : public IndexInfo {
public:
	PatternIndexInfo( const Pattern & pattern ) : IndexInfo( PATTERN ), pattern( pattern ) {}
public:
	const Pattern & pattern;
};

/**
 * Альтернатива шаблона
 */
class LSPL_EXPORT Alternative : public matchers::MatcherContainer {
public:
	typedef boost::ptr_map<text::attributes::AttributeKey,expressions::Expression> BindingMap;
public:

	Alternative( const std::string & source ) :
		source( source ) {
	}

	/**
	 * Сравнить на равенство с заданной альтернативой.
	 *
	 * @param alt альтернатива, с которой сравнивать
	 * @return true, если альтернативы равны
	 */
	bool equals( const Alternative & alt ) const;

	/**
	 * Добавить связывания
	 */
	template <class PtrMap>
	void addBindings( PtrMap & r ) {
		if ( r.begin() != r.end() )
			bindings.transfer( r.begin(), r.end(), r );
	}

	/**
	 * Добавить связывние переменной
	 */
	void addBinding( text::attributes::AttributeKey bindedAttribute, expressions::Expression * exp ) {
		bindings.insert( bindedAttribute, exp );
	}

	/**
	 * Получить список зависимостей альтерантивы
	 */
	const std::vector<const Pattern *> & getDependencies() const {
		return dependencies;
	}

	/**
	 * Получить набор индексов для выбора альтернативы
	 */
	const boost::ptr_vector<IndexInfo> & getStartIndices() const {
		if ( indexInfo.size() == 0 )
			appendIndexInfo( getMatchers() );

		return indexInfo;
	}

	/**
	 * Получить исходный код альтернативы
	 */
	const std::string & getSource() const {
		return source;
	}

	/**
	 * Получить набор связываний альтернативы
	 */
	const BindingMap & getBindings() const {
		return bindings;
	}

	/**
	 * Обновить список зависимостей альтернативы
	 */
	void updateDependencies();

	void dump( std::ostream & out, const std::string & tabs = "" ) const;

private:
	void appendDependencies( const matchers::Matcher & matcher );
	void appendIndexInfo( const boost::ptr_vector<matchers::Matcher> & matchers ) const;
private:

	/**
	 * Исходный текст альтернативы
	 */
	std::string source;

	/**
	 * Список связываний аттрибутов шаблона
	 */
	BindingMap bindings;

	/**
	 * Зависимости альтернативы
	 */
	std::vector<const Pattern *> dependencies;

	/**
	 * Список, содержащий информацию об индексах, применимых при поиске альтернативы
	 */
	mutable boost::ptr_vector<IndexInfo> indexInfo;
};

} }

#endif /*_LSPL_PATTERNS_ALTERNATIVE_H_*/
