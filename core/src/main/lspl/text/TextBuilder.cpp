#include "../base/BaseInternal.h"

#include "TextBuilder.h"
#include "Text.h"
#include "Node.h"
#include "Transition.h"

#include <string>

namespace lspl { namespace text {

TextBuilder::TextBuilder() : text ( 0 ) {
}

TextBuilder::~TextBuilder() {
}

void TextBuilder::start() {
	text = new Text();
}

void TextBuilder::setContent( const std::string & content ) {
	text->content = content;
}

void TextBuilder::setConfig( const TextConfig & config ) {
	text->config = config;
}

Node & TextBuilder::createNode( uint startOffset, uint endOffset ) {
	Node * node = new Node( (uint)text->nodes.size(), startOffset, endOffset, *text ); // Создаем новый узел
	text->nodes.push_back( node ); // Помещаем его в текст
	return *node; // Возвращаем ссылку на узел
}

Node & TextBuilder::getNodeByIndex( uint index ) {
	return *text->nodes.at( index );
}

void TextBuilder::addToken( const markup::TokenRef & token ) {
	text->addTokenToMarkup( token );
}

void TextBuilder::addWord( const markup::WordRef & word ) {
	text->addWordToMarkup( word );
}

void TextBuilder::addMatch( Match * match ) {
	text->addMatchToMarkup( match );
}

Text * TextBuilder::getText() {
	return text;
}

} } // namespace lspl::text
