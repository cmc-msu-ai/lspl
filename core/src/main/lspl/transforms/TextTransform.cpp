
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
#include "../patterns/expressions/VariableExpression.h"
#include "../patterns/expressions/CurrentAnnotationExpression.h"
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
using namespace lspl::morphology;

namespace lspl { namespace transforms {


const unsigned int TextTransform::SpeechPartTable[22] = { 1, 2, 3, 6, 6, 6, 12, 12, 7, 7, 9, 0, 8, 11, 10, 0, 10, 2, 4, 5, 4, 3 };
const unsigned int TextTransform::GrammemClear[11] = {  	0xFFFFFFFE,		//un
															0xFFFFFF01,		//падеж
															0xFFFFFCFF,		//число
															0xFFFFE3FF,		//род
															0xFFFF1FFF,		//степень сравнения
															0xFFF8FFFF,		//время
															0xFFE7FFFF,		//одушевленность
															0xFF9FFFFF,		//форма
															0xF87FFFFF,
															0xC7FFFFFF,		//лицо
															0x3FFFFFFF
															};

const unsigned int TextTransform::ClearSP[14] = {			0,
															(GrammemClear[1] & GrammemClear[2]),
															(GrammemClear[1] & GrammemClear[2] & GrammemClear[3] & GrammemClear[4] & GrammemClear[6] & GrammemClear[7]),
															(GrammemClear[2] & GrammemClear[3] & GrammemClear[5] & GrammemClear[9]),
															(GrammemClear[1] & GrammemClear[2] & GrammemClear[3] & GrammemClear[6]),
															0,
															(GrammemClear[1]),
															0,
															0,
															0,
															0,
															0,
															(GrammemClear[1]),
															//при нормализации шаблона еще не известно какая часть речи будет нормализовываться, поэтому передать все признаки
															(GrammemClear[1] & GrammemClear[2] & GrammemClear[3] & GrammemClear[4] & GrammemClear[5] & GrammemClear[6] & GrammemClear[7] & GrammemClear[9])
															};


TextTransform::~TextTransform() {
	matchers->clear();
	delete matchers;
}

std::string TextTransform::apply( const MatchVariant & matchVariant  ) const {
	std::string result = "";
	buildStr( result, matchVariant, *matchers, 0 );
	return result;
}

std::string TextTransform::apply( const MatchVariant & matchVariant, unsigned int attributes ) const {
	std::string result = "";
	buildStr( result, matchVariant, *matchers, attributes );
	return result;
}

void TextTransform::buildStr( std::string & result, const MatchVariant & matchVariant, const boost::ptr_vector<Matcher> & matchers, unsigned int globalAttributes) const {
	ContextRetriever cr;
	patterns::matchers::Context c = cr.apply(matchVariant);

	AttributesMap attributes;


	/*
	 * в attributes содержатся арибуты для вывода каждого элемента (слово или шаблон), у которого меняется форма
	 * атрибуты - бит-маска соответствующая 32 признакам, определенным в библиотеке
	 * в ней накапливаются все правила для конкретного элемента
	 *
	 * общая схема:
	 * 1) первый проход по правой части собирает все правила преобразования
	 * 2) к главным словам шаблона применяются глобальные правила, заданыне в шаблоне-родителе при выводе
	 * 3) второй проход по правой части собирает все правила согласования
	 * 4) третий проход по правой части выводит все элементы, находя в АОТе слова с атрибутами, соответствующими атрибутам вывода
	 */

	//вычислить правила преобразования для всех слов/шаблонов
	//первый проход по правой части собирает все правила
	//так как правила для любого элемента могут находиться в любом месте шаблона (не обязательно рядом с элементом)
	//цикл по всем элементам правой части
	for ( uint i = 0; i < matchers.size(); ++ i ) {

		//нет правил рядом с элементом
		if ( !matchers.at(i).restrictions.size() )
			continue;

		//цикл по всем правилам, стоящим рядом с элемнтом
		for ( uint j = 0; j < matchers.at(i).restrictions.size(); j++) {
			const AgreementRestriction *restriction=dynamic_cast<const AgreementRestriction*>(&matchers.at(i).restrictions[j]);

			//правило преобразования/согласования
			if(const AttributeExpression *attrKey1=dynamic_cast<const AttributeExpression*>(&restriction->getArgs()[0])) {
				//правила преобразования
				if(const CurrentAnnotationExpression *curExpr=dynamic_cast<const CurrentAnnotationExpression*>(attrKey1->base)) {
					//обработка правил преобразования для слов/шаблонов ( N<c=nom> )  (у лексем не может быть правил преобразования)
					if(matchers.at(i).type == Matcher::WORD || matchers.at(i).type == Matcher::PATTERN) {
						ConstRange val=c.getValues(matchers.at(i).variable);

						//переменная не найдена. либо пустой цикл, либо неизвестное имя
						if(val.first==val.second)
							continue;

						//цикл по всем словам (если их несколько на одной переменной, например, переменная в цикле)
						for(ConstIterator it=val.first; it!=val.second; it++) {
							const Transition * trans = dynamic_cast<const Transition*>( &*it->second );
							if(!trans)
								continue;
							unsigned int &outAttributes = attributes.insert(AttributesPair(trans, 0)).first->second;

							//нормализовать слово, если есть, то идет первым атрибутом
							if(attrKey1->attribute==AttributeKey::BASE) {
								//если атрибуты преобразования ранее не задавались, то задать атрибуты этого слова как в тексте (задаются только если надо преобразовывать слово, и только один раз в самом начале перед всеми преобразованиями)
								if(!outAttributes)
									for(uint id=1; id<=10; id++)
										outAttributes |= 1<<(trans->getAttribute(AttributeKey( id )).value);

								//нормализация слова
								if(trans->type == Transition::WORD) {
									//задать нормализацию в зависимости от части речи
									int sp = dynamic_cast<const Word*>(trans)->getSpeechPart().id;
									outAttributes = (outAttributes & ClearSP[sp]) | (Normalized & ~ClearSP[sp]);
									//для глагола чтобы задать инфинитив надо обнулить все признаки
									outAttributes &= ((sp == 3)?1:outAttributes);
								}
								else
									//нормализация шаблона
									if(trans->type == Transition::MATCH) {
										outAttributes = (outAttributes & ClearSP[13]) | (Normalized & ~ClearSP[13]);
									}
								continue;
							}
							else
								//задать атрибуты перед преобразованием как в тексте
								if(!outAttributes)
									for(uint id=1; id<=10; id++)
										outAttributes |= 1<<(trans->getAttribute(AttributeKey( id )).value);

						   	//изменить требуемые атрибуты в исходном слове/шаблоне
							if(const ConstantExpression *attrVal=dynamic_cast<const ConstantExpression*>(&restriction->getArgs()[1]))
								outAttributes = (outAttributes & GrammemClear[attrKey1->attribute.index]) | (1 << attrVal->getValue().value);
						}
					}
				}
			}
		}
	}



	//применить глобальные атрибуты к главному слову/шаблону в шаблоне (слову, указанному в конце левой части в круглых скобках, от которого шаблон наследует признаки)
	if(globalAttributes && !bindings.empty()) {
		//цикл по всем словам, указанным главными
		for(BindingMap::const_iterator it = bindings.begin(); it != bindings.end(); ++it) {
			if(it->first == AttributeKey::UNDEFINED)
				if(const VariableExpression *varExpr = dynamic_cast<const VariableExpression*>(it->second)) {
					ConstRange val=c.getValues(varExpr->getVariable());

					if(val.first==val.second)
						continue;

					for(ConstIterator it=val.first; it!=val.second; ++it) {
						const Transition *trans = dynamic_cast<const Transition*>( &*it->second );
						if(!trans)
							continue;
						unsigned int &outAttributes = attributes.insert(AttributesPair(trans, 0)).first->second;

						if(!outAttributes)
							for(uint id=1; id<=10; id++)
								outAttributes |= 1<<(trans->getAttribute(AttributeKey( id )).value);


						if(trans->type == Transition::WORD) {
							//задать глобальные атрибуты слову
							int sp = dynamic_cast<const Word*>(trans)->getSpeechPart().id;
							outAttributes = (outAttributes & ClearSP[sp]) | (globalAttributes & ~ClearSP[sp]);
							//внешняя нормализация для глагола
							outAttributes &= (sp == 3 && ((globalAttributes & ~ClearSP[sp]) == (Normalized & ~ClearSP[sp])))?1:outAttributes;
						}
						else
							//задать глобальные атрибуты шаблону
							if(trans->type == Transition::MATCH) {
								outAttributes = (outAttributes & ClearSP[13]) | (globalAttributes & ~ClearSP[13]);
							}
					}
				}
		}
	}



	//применить правила согласования между словами/шаблонами
	//цикл по всем элементам правой части, второй проход
	for ( uint i = 0; i < matchers.size(); ++ i ) {

		//нет правил преобразования
		if ( !matchers.at(i).restrictions.size() )
			continue;

		//цикл по всем правилам элемента
		for ( uint j = 0; j < matchers.at(i).restrictions.size(); j++) {
			const AgreementRestriction *restriction=dynamic_cast<const AgreementRestriction*>(&matchers.at(i).restrictions[j]);

			//правила согласования
			if(const AttributeExpression *attrKey1=dynamic_cast<const AttributeExpression*>(&restriction->getArgs()[0])) {

				//правила согласования конкретных атрибутов ( A.c ~> N.c )
				if(const VariableExpression *varExpr1=dynamic_cast<const VariableExpression*>(attrKey1->base)) {
					if(const AttributeExpression *attrKey2=dynamic_cast<const AttributeExpression*>(&restriction->getArgs()[1]))
						if(const VariableExpression *varExpr2=dynamic_cast<const VariableExpression*>(attrKey2->base)) {

							ConstRange val1=c.getValues(varExpr1->getVariable());
							ConstRange val2=c.getValues(varExpr2->getVariable());

							//переменная не найдена. либо пустой цикл, либо неизвестное имя
							if(val1.first==val1.second || val2.first==val2.second)
								continue;

							const Transition *trans2 = dynamic_cast<const Transition*>( &*(val2.first)->second );
							if(!trans2)
								continue;

							unsigned int tmpAttribute=attributes.insert(AttributesPair(trans2, 0)).first->second;
							if(!tmpAttribute)
								for(uint id=1; id<=10; id++)
									tmpAttribute |= 1<<(trans2->getAttribute(AttributeKey( id )).value);

							for(ConstIterator it=val1.first; it!=val1.second; it++) {
								const Transition *trans1 = dynamic_cast<const Transition*>( &*it->second );
								if(!trans1)
									continue;
								unsigned int &outAttributes = attributes.insert(AttributesPair(trans1, 0)).first->second;

								if(!outAttributes)
									for(uint id=1; id<=10; id++)
										outAttributes |= 1<<(trans1->getAttribute(AttributeKey( id )).value);

								outAttributes = (outAttributes & GrammemClear[attrKey1->attribute.index]) | (tmpAttribute & ~GrammemClear[attrKey1->attribute.index]);
							}
							continue;
						}
				}
				continue;
			}

			//правила согласования ( A ~> N )
			if(const VariableExpression *varExpr1=dynamic_cast<const VariableExpression*>(&restriction->getArgs()[0]))
				if(const VariableExpression *varExpr2=dynamic_cast<const VariableExpression*>(&restriction->getArgs()[1])) {

					ConstRange val1=c.getValues(varExpr1->getVariable());
					ConstRange val2=c.getValues(varExpr2->getVariable());

					//переменная не найдена. либо пустой цикл, либо неизвестное имя
					if(val1.first==val1.second || val2.first==val2.second)
						continue;

					const Transition *trans2 = dynamic_cast<const Transition*>( &*(val2.first)->second );
					if(!trans2)
						continue;

					unsigned int tmpAttribute=attributes.insert(AttributesPair(trans2, 0)).first->second;
					//word1 согласовывается с word2 => word2 не меняется, его атрибуты не надо просчитывать, чтобы оно потом вывелось без изменений. поэтому его атрибуты считаются в tmp
					if(!tmpAttribute)
						for(uint id=1; id<=10; id++)
							tmpAttribute |= 1<<(trans2->getAttribute(AttributeKey( id )).value);

					for(ConstIterator it=val1.first; it!=val1.second; it++) {
						const Transition *trans1 = dynamic_cast<const Transition*>( &*it->second );
						if(!trans1)
							continue;
						unsigned int &outAttributes = attributes.insert(AttributesPair(trans1, 0)).first->second;

						if(!outAttributes)
							for(uint id=1; id<=10; id++)
								outAttributes |= 1<<(trans1->getAttribute(AttributeKey( id )).value);

						if(trans1->type == Transition::WORD) {
							//скопировать нужные имеющиеся атрибуты для слова
							int sp = dynamic_cast<const Word*>(trans1)->getSpeechPart().id;
							for(uint id=1; id<=10; id++)
								if( tmpAttribute & ~GrammemClear[id] )
									outAttributes = (outAttributes & (ClearSP[sp] | GrammemClear[id])) | (tmpAttribute & ~ClearSP[sp] & ~GrammemClear[id]);
						}
						else
							//скопировать нужные имеющиеся атрибуты для шаблона
							if(trans1->type == Transition::MATCH) {
								for(uint id=1; id<=10; id++)
									if( tmpAttribute & ~GrammemClear[id] )
										outAttributes = (outAttributes & (ClearSP[13] | GrammemClear[id])) | (tmpAttribute & ~ClearSP[13] & ~GrammemClear[id]);
							}
					}
				}
		}
	}



	//вывести все слова в заданных нужных формах
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

			for(ConstIterator it=val.first; it!=val.second; ++it) {
				const Word * word = dynamic_cast<const Word*>( &*it->second );
				if(!word)
					continue;
				unsigned int &outAttributes=attributes.insert(AttributesPair(word, 0)).first->second;

				//нет правил преобразования слова
				if(!outAttributes) {
					if ( result.length() > 0 )
						result += " ";
					result += word->getToken();
					continue;
				}

				lspl::morphology::AotMorphology *aot=dynamic_cast<lspl::morphology::AotMorphology*>(&morphology::Morphology::instance());
				std::vector<CFormInfo> res;
				if(!aot->getParadigm(word->getToken(), res))
					continue;

				unsigned int sourceGrammem=0, targetGrammem=0, sGrammem=0;
				unsigned long long tmpGrammem=0;
				CFormInfo *f;

				//взять часть речи слова
				int posflag=0;
				for ( uint i = 0; i < res.size(); ++ i) {
					f = &res[i];
					if(SpeechPartTable[aot->getSP(f->GetSrcAncode().c_str())] != word->getSpeechPart().id)
						continue;

					posflag=1;

					//найти в словаре номер слова атирбутами, не противоречащами заданным
					int attrNum;
					outAttributes &= 0xFFFFFFFE;
					//цикл по всем словоформам из полученной парадигмы
					for ( attrNum = 0; attrNum < f->GetCount(); ++ attrNum) {
						int aotsp=aot->getSP(f->GetAncode(attrNum).c_str());
						if( SpeechPartTable[aotsp] != word->getSpeechPart().id)
							continue;
						//получить атрибуты АОТа
						tmpGrammem = aot->getAttributes(f->GetAncode(attrNum).c_str());
						//построить бит-маску для очередного слова из АОТа
						sGrammem = 	(((tmpGrammem >> 2) & 1) << 1) |
									(((tmpGrammem >> 3) & 1) << 2) |
									(((tmpGrammem >> 4) & 1) << 3) |
									(((tmpGrammem >> 5) & 1) << 4) |
									(((tmpGrammem >> 6) & 1) << 5) |
									(((tmpGrammem >> 7) & 1) << 6) |
//									(((tmpGrammem >> 8) & 1) << 7) |

									(((tmpGrammem >> 1) & 1) << 8) |
									(((tmpGrammem >> 0) & 1) << 9) |

									(((tmpGrammem >> 9) & 1) << 10) |
									(((tmpGrammem >> 10) & 1) << 11) |
									(((tmpGrammem >> 11) & 1) << 12) |

									(((tmpGrammem >> 22) & 1) << 13) |
									(((tmpGrammem >> 49) & 1) << 14) |
									//для прилагательных/причастий принудительно отметить, что нет степени сравнения
									((aotsp==1 | aotsp==17 | aotsp==18 | aotsp==20)?(((((tmpGrammem >> 22) & 1) | ((tmpGrammem >> 49) & 1))?0:1) << 15):0) |

									(((tmpGrammem >> 13) & 1) << 16) |
									(((tmpGrammem >> 15) & 1) << 17) |
									(((tmpGrammem >> 14) & 1) << 18) |

									(((tmpGrammem >> 20) & 1) << 19) |
									(((tmpGrammem >> 21) & 1) << 20) |

									//полная/краткая форма прилагательных/причастий различается в части речи
									(((aotsp == 1 || aotsp == 18)?1:0) << 21) |
									(((aotsp == 17 || aotsp == 20)?1:0) << 22) |

//									(((tmpGrammem >> 13) & 1) << 23) |
//									(((tmpGrammem >> 15) & 1) << 24) |
//									(((tmpGrammem >> 14) & 1) << 25) |
//									(((tmpGrammem >> 13) & 1) << 26) |

									(((tmpGrammem >> 16) & 1) << 27) |
									(((tmpGrammem >> 17) & 1) << 28) |
									(((tmpGrammem >> 18) & 1) << 29)

//									(((tmpGrammem >> 15) & 1) << 30) |
//									(((tmpGrammem >> 14) & 1) << 31)
									;

						if(aotsp == 21) {
							if(outAttributes == sGrammem)
								break;
							else
								continue;
						}
						//сравнить атрибуты слов. атрибут сравнивается только если он имеется у обоих словоформ, иначе он игнорируется
						unsigned int tmp=0;
						for(uint id=1; id<=10; id++) {
							unsigned int tmp1=outAttributes & ~GrammemClear[id];
							unsigned int tmp2=sGrammem & ~GrammemClear[id];
							tmp |= ( !tmp1 || !tmp2 )?(0):((tmp1 ^ (tmp2 & tmp1)) );
						}
						if( !tmp )
							break;
					}

					//если нашли слово с требемыми атрибутами, то дописать к результату это слово
					if( attrNum < f->GetCount() ) {
						if ( result.length() > 0 )
							result += " ";
						result += f->GetWordForm( attrNum );
						posflag=2;
						break;
					}

				}

				if(posflag==2) {
					continue;
				}

				//ошибка, нужная часть речи для слова не найдена в АОТе, вернуть слово как в тексте
				if(!posflag) {
					if ( result.length() > 0 )
						result += " ";
					result += /*"POSerror " +*/ word->getToken();
					continue;
				}


				//ошибка, нужная форма слова не найдена в АОТе, вернуть слово как в тексте
				if ( result.length() > 0 )
					result += " ";
				result += /*"WFerror " +*/ word->getToken();

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
				if(!match)
					continue;

				//получить результат преобразования вложенного шаблона с заданными атрибутами
				if ( result.length() > 0 )
					result += " ";
				result += match->getVariants().at( 0 )->getTransformResult<std::string>(attributes.insert(AttributesPair(match, 0)).first->second);

//				TextTransform *tt;
//				if(tt = const_cast<TextTransform*>(dynamic_cast<const TextTransform*>(&match->getVariants().at(0)->alternative.getTransform()/*&patternvar->pattern.alternatives.at(j).getTransform()*/)))
//					buildStr(result, *match->getVariants().at( 0 ), tt->getMatchers(), attributes.insert(AttributesPair(match, 0)).first->second, match->getVariants().at(0)->alternative.getBindings());
			}
		}
	}
}

} } // namespace lspl::transforms
