
#include "../base/BaseInternal.h"

#include "TextTransform.h"

#include <iostream>

#include "../text/Loop.h"
#include "../text/Match.h"
#include "../text/markup/Token.h"
#include "../text/markup/Word.h"
#include "../text/attributes/AttributeKey.h"
#include "../text/attributes/AttributeValue.h"
#include "../morphology/Morphology.h"
#include "../morphology/WordForm.h"
#include "../patterns/matchers/Matcher.h"
#include "../patterns/matchers/TokenMatcher.h"
#include "../patterns/matchers/WordMatcher.h"
#include "../patterns/matchers/PatternMatcher.h"
#include "../patterns/restrictions/AgreementRestriction.h"
#include "../patterns/expressions/AttributeExpression.h"
#include "../patterns/expressions/ConstantExpression.h"
#include "../patterns/Pattern.h"

#include "ContextRetriever.h"
#include "../patterns/matchers/Context.h"

#include "aot/Source/AgramtabLib/RusGramTab.h"
#include "aot/Source/LemmatizerLib/Lemmatizers.h"
#include "aot/Source/LemmatizerLib/Paradigm.h"

#include "aot/Source/common/utilit.h"

#include "../morphology/Morphology.h"
#include "../morphology/AotMorphology.h"

using lspl::text::markup::Word;
using lspl::text::markup::Token;
using lspl::text::Loop;
using lspl::text::Match;
using lspl::text::MatchVariant;
using lspl::text::Transition;
using lspl::text::TransitionList;
using namespace lspl::text::attributes;
using namespace lspl::patterns::restrictions;
using namespace lspl::patterns::expressions;
using namespace lspl::patterns::matchers;

namespace lspl { namespace transforms {

TextTransform::TextTransform() {
}

TextTransform::~TextTransform() {
}

std::string TextTransform::apply( const MatchVariant & matchVariant  ) const {
	std::string result;
	buildStr( result, matchVariant, *matchers );
	return result;
}

void TextTransform::buildStr( std::string & result, const MatchVariant & matchVariant, const boost::ptr_vector<Matcher> & matchers ) const {
	ContextRetriever cr;
	patterns::matchers::Context c = cr.apply(matchVariant);
	for ( uint i = 0; i < matchers.size(); ++ i ) {
		//лексема
		if ( matchers.at(i).type == Matcher::TOKEN ) {
			const TokenMatcher * token = dynamic_cast<const TokenMatcher*>( &matchers.at(i) );
			if ( result.length() > 0 )
				result += " ";
			result += token->token;
		} else 
		//переменная
		if ( matchers.at(i).type == Matcher::WORD ) {
			const WordMatcher * wordvar = dynamic_cast<const WordMatcher*>( &matchers.at(i) );
			ConstRange val=c.getValues(wordvar->variable);

			//переменная не найдена. либо пустой цикл, либо неизвестное имя
			if(val.first==val.second)
				continue;
				
			for(ConstIterator it=val.first; it!=val.second; it++) {
				const Word * word = dynamic_cast<const Word*>( &*it->second );

				//нет правил преобразования слова
				if(!wordvar->restrictions.size()) {
					if ( result.length() > 0 )
						result += " ";
					result += word->getToken();
					continue;
				}

				AgreementRestriction* restrictions=const_cast<AgreementRestriction*>(dynamic_cast<const AgreementRestriction*>(&wordvar->restrictions[0]));
				AttributeExpression* attr;
				if(attr=const_cast<AttributeExpression*>(dynamic_cast<const AttributeExpression*>(&restrictions->getArgs()[0])));
				else {
					//есть правила согласования слова. пока не обрабатываются
					if ( result.length() > 0 )
						result += " ";
					result += word->getToken();
					continue;
				}
				
				//Нормализовать слово. если есть, то всегда идет первым атрибутом
				if(attr->attribute==AttributeKey::BASE) {
					if ( result.length() > 0 )
						result += " ";
					result += word->getBase();
					continue;
				}

				/*
				 * здесь будет преобразование слова по заданным аттрибутам
				 */

				//если ничего выше не сработало, то вернуть слово как в тексте
				if ( result.length() > 0 )
					result += " ";
				result += word->getToken();
			}
		} else
		//шаблон
		if ( matchers.at(i).type == Matcher::PATTERN ) {
			const PatternMatcher * patternvar = dynamic_cast<const PatternMatcher*>( &matchers.at(i) );
			ConstRange val=c.getValues(patternvar->variable);

			//переменная не найдена. либо пустой цикл, либо неизвестное имя
			if(val.first==val.second)
				continue;

			for(ConstIterator it=val.first; it!=val.second; it++) {
				const Match * match = dynamic_cast<const Match*>( &*it->second );
				//рекурсивно вывести шаблон
				const TextTransform *tt=dynamic_cast<const TextTransform*>(&patternvar->pattern.alternatives.at(0).getTransform());
				buildStr(result, *match->getVariants().at( 0 ), tt->getMatchers());
			}
		}

	}
}

} } // namespace lspl::transforms
