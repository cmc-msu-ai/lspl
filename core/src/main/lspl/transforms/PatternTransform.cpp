
#include "../base/BaseInternal.h"

#include "PatternTransform.h"

#include <iostream>
#include <sstream>

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
#include "../patterns/matchers/StringMatcher.h"
#include "../patterns/restrictions/AgreementRestriction.h"
#include "../patterns/expressions/AttributeExpression.h"
#include "../patterns/expressions/ConstantExpression.h"
#include "../patterns/Pattern.h"
#include "../patterns/PatternBuilder.h"

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

PatternTransform::~PatternTransform() {
	matchers->clear();
	delete matchers;
}

lspl::patterns::PatternRef PatternTransform::apply( const MatchVariant & matchVariant  ) const {
	std::string result;
	lspl::patterns::PatternBuilderRef builder = new lspl::patterns::PatternBuilder(ns);
	int pattnum=ns->getPatternCount();
	std::string pattern_name = std::string("GP-");
	std::stringstream ss;

	//сгенерировать имя шаблона
	ss << pattnum;
	for(int i=0; i<ss.str().length(); i++) {
		pattern_name += (ss.str().c_str()[i]-'0'+'A');
	}
	result += pattern_name;
	result += " = ";
	//сгенерировать строку шаблона
	buildStr( result, matchVariant, *matchers );

	//попытаться построить шаблон
	try {
		builder->build(result.c_str());
	} catch(lspl::patterns::PatternBuildingException &pbe) {
		std::cerr<<"Exception while building generated pattern:"<<std::endl<<result.c_str()<<std::endl;
		throw pbe;
	}
	
	return ns->getPatternByName(pattern_name);
}

void PatternTransform::buildStr( std::string & result, const MatchVariant & matchVariant, const boost::ptr_vector<Matcher> & matchers ) const {
	ContextRetriever cr;
	patterns::matchers::Context c = cr.apply(matchVariant);
	for ( uint i = 0; i < matchers.size(); ++ i ) {
		//часть строки шаблона
		if ( matchers.at(i).type == Matcher::TOKEN ) {
			const StringMatcher * token = dynamic_cast<const StringMatcher*>( &matchers.at(i) );
			result += token->token;
		} else 
		
		//ссылка на переменную
		if ( matchers.at(i).type == Matcher::WORD ) {
			const WordMatcher * wordvar = dynamic_cast<const WordMatcher*>( &matchers.at(i) );
			ConstRange val=c.getValues(wordvar->variable);

			//переменная не найдена. либо пустой цикл, либо неизвестное имя
			if(val.first==val.second)
				continue;
				
			for(ConstIterator it=val.first; it!=val.second; it++) {
				const Word * word = dynamic_cast<const Word*>( &*it->second );

				//нет атрибутов, вернуть значение как в тексте
				if(!wordvar->restrictions.size()) {
					result += word->getToken();
					continue;
				}

				//вернуть значение конкретного атрибута слова
				AgreementRestriction* restrictions=const_cast<AgreementRestriction*>(dynamic_cast<const AgreementRestriction*>(&wordvar->restrictions[0]));
				AttributeExpression* attr;
				if(attr=const_cast<AttributeExpression*>(dynamic_cast<const AttributeExpression*>(&restrictions->getArgs()[0]))) {
					//лексема
					if(attr->attribute==AttributeKey::BASE) {
						result += word->getBase();
					}
					//другие атрибуты
					else {
						result += word->getAttribute(attr->attribute).getAbbrevation();
					}
					continue;
				}

				//неизвестный параметр. не должно случаться
				result += "UndefAttr";
			}
		} else

		//ссылка на шаблон
		if ( matchers.at(i).type == Matcher::PATTERN ) {
			const PatternMatcher * patternvar = dynamic_cast<const PatternMatcher*>( &matchers.at(i) );
			ConstRange val=c.getValues(patternvar->variable);

			//шаблон не найден, неизвестное имя
			if(val.first==val.second)
				continue;

			for(ConstIterator it=val.first; it!=val.second; it++) {
				const Match * match = dynamic_cast<const Match*>( &*it->second );

				AgreementRestriction* restrictions=const_cast<AgreementRestriction*>(dynamic_cast<const AgreementRestriction*>(&patternvar->restrictions[0]));
				AttributeExpression* attr;
				if(attr=const_cast<AttributeExpression*>(dynamic_cast<const AttributeExpression*>(&restrictions->getArgs()[0]))) {
					result += match->getAttribute(attr->attribute).getAbbrevation();
					continue;
				}
			}
		}

	}
	
}

} } // namespace lspl::transforms
