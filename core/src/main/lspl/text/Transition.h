#ifndef _LSPL_TRANSITIONS_TRANSITION_H_
#define _LSPL_TRANSITIONS_TRANSITION_H_

#include "../base/Base.h"
#include "../base/RefCountObject.h"
#include "../base/RefCountPtr.h"

#include "Forward.h"

#include "attributes/AttributeContainer.h"

#include <ostream>
#include <vector>

namespace lspl { namespace text {

class Node;

} }

namespace lspl { namespace text {

/**
 * Переход между узлами текста
 */
class LSPL_EXPORT Transition : public base::RefCountObject<Transition>, public attributes::AttributeContainer {
public:
	enum Type {
		SPACE,
		PUNCTUATION,
		WORD,
		MATCH,
		LOOP,
		TOKEN
	};
public:
	Transition( Type type, const text::Node & start, const text::Node & end ) :
		type( type ), start( start ), end( end ) {}

	virtual ~Transition();

	/**
	 * Получить фрагмент текста, соответствующий аннотации
	 */
	std::string getText() const;

	/**
	 * Получить позицию начала (в символах) фрагмента текста, соответствующего аннотации
	 */
	uint getStartOffset() const;

	/**
	 * Получить позицию конца (в символах) фрагмента текста, соответствующего аннотации
	 */
	uint getEndOffset() const;

	virtual void dump( std::ostream & out, std::string tabs = "" ) const = 0;
public:

	/**
	 * Тип перехода
	 */
	const Type type;

	/**
	 * Начало перехода в представлении текста
	 */
	const text::Node & start;

	/**
	 * Конец перехода в представлении текста
	 */
	const text::Node & end;
};

} } // namespace lspl::text

#endif /*_LSPL_TRANSITIONS_TRANSITION_H_*/
