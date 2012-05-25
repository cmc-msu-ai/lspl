
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

void TextTransform::buildStr( std::string & str, const MatchVariant & v, const boost::ptr_vector<Matcher> & matchers ) const {
	for ( uint i = 0; i < matchers.size(); ++ i )
		buildStr( str, v, matchers.at(i) );
}

/*void TextTransform::buildStr( std::string & str, const text::LoopIterationList & iterations, const Matcher & matchers ) const {
	for ( uint i = 0; i < iterations.size(); ++ i ) {
		if ( const Word * word = dynamic_cast<const Word*>( &iterations[ i ]->getVariant( 0 ) ) ) {
			if(matchers.variable==word->alternative.getMatcher( i ).variable) {
			}
			if ( str.length() > 0 )
				str += " ";
			str += word->getBase();
		} else if ( const Loop * loop = dynamic_cast<const Loop*>( &iterations[ i ]->getVariant( 0 ) ) ) {
			buildStr( str, loop->getIterations() );
		} else if ( const Match * match = dynamic_cast<const Match*>( &iterations[ i ]->getVariant( 0 ) ) ) {
			buildStr( str, *match->getVariants().at( 0 ) );
		}
//		buildStr( str, iterations[ i ]->getVariant( 0 ), matchers );
	}
}
*/
void TextTransform::buildStr( std::string & str, const lspl::text::MatchVariant & v, const Matcher & matchers ) const {
/*	std::vector<AttributeKey> attrkeys;
	std::vector<AttributeValue> attrvals;
	std::vector<AttributeValue> formvals;

	attrkeys.push_back(AttributeKey::CASE);
	attrkeys.push_back(AttributeKey::NUMBER);
	attrkeys.push_back(AttributeKey::GENDER);
	attrkeys.push_back(AttributeKey::DOC);
	attrkeys.push_back(AttributeKey::TENSE);
	attrkeys.push_back(AttributeKey::ANIMATE);
	attrkeys.push_back(AttributeKey::FORM);
	attrkeys.push_back(AttributeKey::MODE);
	attrkeys.push_back(AttributeKey::PERSON);
	attrkeys.push_back(AttributeKey::REFLEXIVE);
*/
//	std::cout<<v.alternative.getMatcherCount()<<std::endl;

	if ( matchers.type == lspl::patterns::matchers::Matcher::TOKEN ) {
		const TokenMatcher * token = dynamic_cast<const TokenMatcher*>( &matchers );
		if ( str.length() > 0 )
			str += " ";
		str += token->token;
	} else if ( matchers.type == lspl::patterns::matchers::Matcher::WORD ) {
		const WordMatcher * wordvar = dynamic_cast<const WordMatcher*>( &matchers );
		const Word * word=NULL;
		for(int j=0; j<v.alternative.getMatcherCount(); j++) {
/*			if(const Loop * loop = dynamic_cast<const Loop*>(&*v[j])) {
				buildStr( str, loop->getIterations(), matchers );
			}
			else
*/				if(wordvar->variable==v.alternative.getMatcher( j ).variable) {
					word = dynamic_cast<const Word*>( &*v[j] );
					break;
				}
		}
		if(!word) {
			std::cout<<"'Unknown variable "<<wordvar->variable<<" '"<<std::endl;
			return;
		}
			//получить коллекцию форм слова
/*			boost::ptr_vector<morphology::WordForm> forms;
		
		std::cout<<word->getBase()<<std::endl;
		morphology::Morphology::instance().appendWordForms(word->getBase(), forms);
		lspl::morphology::AotMorphology *aot=dynamic_cast<lspl::morphology::AotMorphology*>(&morphology::Morphology::instance());
		char str[1000];
		string stri=word->getBase();
		aot->lemmatizer->GetAllAncodesQuick((BYTE*)stri.c_str(),true,(BYTE*)str,false);
		std::cout<<str<<std::endl;
		std::cout<<forms.size()<<std::endl;
		for(uint k=0; k<forms.size(); k++) 
			std::cout<<forms[k].getBase()<<std::endl;
*/				
		if(!wordvar->restrictions.size()) {
			if ( str.length() > 0 )
				str += " ";
			str += word->getToken();
			return;
		}

		AgreementRestriction* restrictions=const_cast<AgreementRestriction*>(dynamic_cast<const AgreementRestriction*>(&wordvar->restrictions[0]));
		AttributeExpression* attr;
		if(attr=const_cast<AttributeExpression*>(dynamic_cast<const AttributeExpression*>(&restrictions->args[0])));
		else {
			if ( str.length() > 0 )
				str += " ";
			str += word->getToken();
			return;
		}
		ConstantExpression* val;

		//Нормализовать слово
		if(attr->attribute==AttributeKey::BASE) {
			if ( str.length() > 0 )
				str += " ";
			str += word->getBase();
			return;
		}
			//найти нужную форму слова
/*			for(uint j=0; j<attrkeys.size(); j++) {
			for(uint k=0; k<wordvar->restrictions.size(); k++) {
				restrictions=const_cast<AgreementRestriction*>(dynamic_cast<const AgreementRestriction*>(&wordvar->restrictions[k]));
				attr=const_cast<AttributeExpression*>(dynamic_cast<const AttributeExpression*>(&restrictions->args[0]));
				val=const_cast<ConstantExpression*>(dynamic_cast<const ConstantExpression*>(&restrictions->args[1]));
				if(attrkeys[j] == attr->attribute)
					attrvals.push_back(val->value);
				else
					attrvals.push_back(word->getAttribute(attrkeys[j]));
			}
		}
			uint n=0;
		bool flag=true;
		for(n=0; n<forms.size(); n++) {
			for(uint k=0; k<attrkeys.size(); k++) {
				formvals.push_back(forms[n].getAttribute(0,attrkeys[k]));
			}
			for(uint k=0; k<attrvals.size(); k++) {
				if(attrvals[k]!=formvals[k])
					flag=false;
			}
			formvals.clear();
			if(flag)
				break;	
		}
		n--;
			if(!flag) {
			std::cout<<"'No requested word form found for var "<<wordvar->variable<<" '"<<std::endl;
			continue;
		}
*/			
		
		if ( str.length() > 0 )
			str += " ";
		str += word->getToken();
	} else if ( matchers.type == lspl::patterns::matchers::Matcher::PATTERN ) {
		const PatternMatcher * patternvar = dynamic_cast<const PatternMatcher*>( &matchers );
		const Match * match = NULL;
		for(int j=0; j<v.alternative.getMatcherCount(); j++) 
			if(patternvar->variable==v.alternative.getMatcher( j ).variable) {
				match = dynamic_cast<const Match*>( &*v[j] );
				break;
			}
		if(!match) {
			std::cout<<"'Unknown variable "<<patternvar->variable<<" '"<<std::endl;
			return;
		}

		buildStr( str, *match->getVariants().at( 0 ), patternvar->pattern.alternatives.at(0).getMatchers() );
	}

}

} } // namespace lspl::transforms
