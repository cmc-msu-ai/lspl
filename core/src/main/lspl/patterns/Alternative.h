#ifndef _LSPL_PATTERNS_ALTERNATIVE_H_
#define _LSPL_PATTERNS_ALTERNATIVE_H_

#include "Forward.h"

#include "../base/Base.h"

#include "../text/attributes/SpeechPart.h"
#include "../text/attributes/AttributeKey.h"

#include "../transforms/Forward.h"

#include "matchers/Forward.h"
#include "matchers/MatcherContainer.h"

#include "expressions/Expression.h"

#include <memory>
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

	Alternative( const std::string & source, const std::string & transformSource = std::string() );
	~Alternative();

	/**
	 * Сравнить на равенство с заданной альтернативой.
	 *
	 * @param alt альтернатива, с которой сравнивать
	 * @return true, если альтернативы равны
	 */
	bool equals( const Alternative & alt ) const;

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
	 * Получить исходный код альтернативы
	 */
	const std::string & getTransformSource() const {
		return transformSource;
	}

	/**
	 * Получить набор связываний альтернативы
	 */
	const BindingMap & getBindings() const {
		return bindings;
	}

	/**
	 * Получить преобразование определяемое альтернативой
	 */
	const transforms::Transform & getTransform() const {
		return *transform;
	}

	bool hasTransform() const {
		return transform.get() != 0;
	}

	void dump( std::ostream & out, const std::string & tabs = "" ) const;

public:

	bool operator == ( const Alternative & alt ) const {
		return equals( alt );
	}

	bool operator != ( const Alternative & alt ) const {
		return !equals( alt );
	}

public:

	/**
	 * Добавить связывания аттрибутов
	 */
	template <class PtrMap>
	void addBindings( PtrMap & r ) {
		if ( r.begin() != r.end() )
			bindings.transfer( r.begin(), r.end(), r );
	}

	/**
	 * Добавить связывание аттрибута к альтернативе
	 *
	 * @param bindedAttribute связываемый аттрибут
	 * @param exp выражение, формирующее значение аттрибута
	 */
	void addBinding( text::attributes::AttributeKey bindedAttribute, expressions::Expression * exp ) {
		bindings.insert( bindedAttribute, exp );
	}

	/**
	 * Установить преобразование, осуществляемое альтернативой
	 *
	 * @param t преобразование
	 */
	void setTransform( std::auto_ptr<transforms::Transform> t );

	/**
	 * Обновить список зависимостей альтернативы
	 */
	void updateDependencies();

	const Pattern & getPattern() const { return *pattern; }

private:
	void appendDependencies( const matchers::Matcher & matcher );
	void appendIndexInfo( const boost::ptr_vector<matchers::Matcher> & matchers ) const;
private:

	const Pattern * pattern;

	/**
	 * Преобразование, осуществляемое альтернативой
	 */
	std::auto_ptr<transforms::Transform> transform;

	/**
	 * Исходный текст альтернативы
	 */
	std::string source;

	std::string transformSource;

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

	friend class Pattern;
};

} }

#endif /*_LSPL_PATTERNS_ALTERNATIVE_H_*/
