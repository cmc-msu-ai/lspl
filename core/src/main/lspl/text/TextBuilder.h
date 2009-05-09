#ifndef _LSPL_TEXT_TEXTBUILDER_H_
#define _LSPL_TEXT_TEXTBUILDER_H_

#include "../base/Base.h"
#include "Forward.h"

#include <string>

namespace lspl { namespace text {

class LSPL_EXPORT TextBuilder
{
public:
	TextBuilder();
	virtual ~TextBuilder();

	/**
	 * Начать построение нового текста
	 */
	void start();

	/**
	 * Установить содержимое текста
	 */
	void setContent( const std::string & content );
	void setConfig( const TextConfig & config );

	Node & createNode( uint startOffset, uint endOffset );

	Node & getNodeByIndex( uint index );

	void addToken( const markup::TokenRef & token );
	void addWord( const markup::WordRef & word );
	void addMatch( Match * match );

	/**
	 * Получить текст, построенный билдером
	 */
	Text * getText();
private:
	Text * text;
};

} } // namespace lspl::text::TextBuilder

#endif /*_LSPL_TEXT_TEXTBUILDER_H_*/
