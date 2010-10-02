#ifndef _LSPL_TEXT_TEXTNODE_H_
#define _LSPL_TEXT_TEXTNODE_H_

#include "../base/Base.h"
#include "../base/RefCountObject.h"

#include "Forward.h"

#include <ostream>
#include <vector>

namespace lspl { namespace text {

/**
 * Узел текста
 */
class LSPL_EXPORT Node : public base::RefCountObject, public base::IdentifiedObject<Node> {
public:
	Node( uint index, uint startOffset, uint endOffset, const Text & text );
	~Node();

	bool operator < ( const Node & node ) const {
		return startOffset < node.startOffset;
	}

	void dump( std::ostream & out, std::string tabs = "" ) const;

	/**
	 * Получить текст незначимого отрезка, соответствующего вершине
	 */
	std::string getRangeString() const;

	/**
	 * Получить позицию начала (в символах) отрезка текста, соответствующего вершине
	 */
	uint getRangeStart() const {
		return startOffset;
	}

	/**
	 * Получить позицию конца (в символах) отрезка текста, соответствующего вершине
	 */
	uint getRangeEnd() const {
		return endOffset;
	}

	/**
	 * Получить список ребер, выходящих из узла
	 */
	const TransitionList & getTransitions() const {
		return transitions;
	}

	const TransitionRef & getTransition( uint index ) const {
		return transitions[ index ];
	}

	uint getTransitionCount() const {
		return transitions.size();
	}

	void addTokenTransition( const markup::TokenRef & word );
	void addWordTransition( const markup::WordRef & word );
	void addMatchTransition( const MatchRef & word );

	uint getTokenCount() const { return tokenCount; }
	uint getWordCount() const { return wordCount; }
	uint getMatchesCount() const { return matchCount; }

public:
    
    /**
     * Количество объектов в памяти
     */
    static uint aliveObjectsCount;

	/**
	 * Индекс узла в тексте
	 */
	const uint index;

	/**
	 * Смещение начала узла от начала текста (в символах)
	 */
	const uint startOffset;

	/**
	 * Смещение конца узла от начала текста (в символах)
	 */
	const uint endOffset;

	/**
	 * Текст, содержащий узел
	 */
	const Text & text;

private:

	/**
	 * Переходы, начинающиеся в узле
	 */
	TransitionList transitions;

	uint tokenCount;

	uint wordCount;

	uint matchCount;
};

} } // namespace lspl::text

#endif /*_LSPL_TEXT_TEXTNODE_H_*/

