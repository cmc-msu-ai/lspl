#include "../base/BaseInternal.h"
#include "../Namespace.h"

#include "PatternBuilder.h"
#include "Pattern.h"

#include "parsers/CharacterSets.h"

#include "../morphology/Morphology.h"

#include "matchers/Matcher.h"
#include "matchers/TokenMatcher.h"
#include "matchers/RegexpMatcher.h"
#include "matchers/LoopMatcher.h"
#include "matchers/PatternMatcher.h"
#include "matchers/WordMatcher.h"

#include "../transforms/TransformBuilder.h"
#include "../transforms/TextTransformBuilder.h"
#include "../transforms/PatternTransformBuilder.h"

#include "restrictions/AgreementRestriction.h"
#include "restrictions/NotRestriction.h"
#include "restrictions/OrRestriction.h"
#include "restrictions/SharedRestriction.h"

#include "expressions/CurrentAnnotationExpression.h"
#include "expressions/ConstantExpression.h"
#include "expressions/AttributeExpression.h"
#include "expressions/VariableExpression.h"

using namespace lspl::text::attributes;

using namespace lspl::patterns::restrictions;
using namespace lspl::patterns::expressions;
using namespace lspl::patterns::matchers;
using namespace lspl::patterns::parsers;
using lspl::morphology::Morphology;

LSPL_REFCOUNT_CLASS( lspl::patterns::PatternBuilder );

namespace lspl { namespace patterns {

typedef std::unique_ptr<Matcher> MatcherPtr;
typedef std::unique_ptr<Pattern> PatternPtr;

class ParserImpl: public PatternBuilder::Parser {
private:
	const char *buffer;
	uint pos;

	/*
	 * Пропускает пробельные символы в буфере
	 */
	void skipSpaces() {
		while (isSpace(buffer[pos]))
			++pos;
	}

	/**
	 * Проверка на наличие впереди конца ввода
	 */
	bool seekEndOfInput() {
		skipSpaces();
		return buffer[pos] == '\0';
	}

	/**
	* Проверка на наличие конца строки / ввода
	*/
	bool seekEndOfLine() {
		while (buffer[pos] == ' ')
			++pos;
		return buffer[pos] == '\0' || buffer[pos] == '\n' || buffer[pos] == '\r';
	}

	/**
	 * Создаёт экземпляр исключения с заданным сообщением об ошибке, хранящий
	 * информацию о текущей позиции парсера и входных данных
	 */
	PatternBuildingException produceException(const std::string &description) {
		std::string combinedDesc = description;
		combinedDesc += " at character #" + std::to_string(pos);
		PatternBuildingException e(combinedDesc, buffer, pos);
		return e;
	}

	/*
	 * Вырезать из токена-идентификатора индекс
	 */
	bool cutIndexFromToken(std::string &token, uint &index) {
		int pos = token.length();
		while (pos != 0 && isDigit(token[pos - 1]))
			--pos;

		if (pos == token.length())
			return false;
		if (pos == 0)
			throw produceException("Invalid token: matcher with index expected, but only integer found");

		try {
			index = std::stoul(token.substr(pos));
		} catch (std::out_of_range &e) {
			throw produceException("Integer overflow in matcher index");
		} catch (...) {
			throw produceException("Unknown exception while retrieving matcher index");
		}

		token.resize(pos);
		return true;
	}

	/*
	 *  Убеждается, что текст продолжается строкой pattern
	 */
	bool strFollows(const char *pattern) {
		skipSpaces();
		uint i = 0;
		while (pattern[i] != '\0' && pattern[i] == buffer[pos + i])
			++i;
		return pattern[i] == '\0';
	}

	/*
	 * В точности повторяет действие функции strFollows, но считывает pattern из буфера
	 * и кидает исключение, если не встречает строку
	 */
	void readStrFollows(const char *pattern) {
		if (!strFollows(pattern))
			throw produceException(std::string("Expected \"") + pattern + "\"");
		pos += strlen(pattern);
	}

	/**
	 * Допустим ли символ в токене
	 */
	static bool isAllowedCharacterInToken(char c) {
		if (c == '_' || c == '-')
			return true;
		return !isInvalidChar(c) && !isPunct(c) && !isSpace(c);
	}

	/**
	 * Считывает токен
	 */
	std::string readToken() {
		skipSpaces();
		std::string token;
		while (isAllowedCharacterInToken(buffer[pos]))
			token += buffer[pos++];
		return token;
	}

	/*
	 * Считывает беззнаковое целое
	 */
	uint readUInt() {
		std::string token = readToken();
		if (token.length() == 0) {
			throw produceException(std::string("Integer expected, but ") +
					(seekEndOfInput()
							? std::string("end of file")
							: std::string() + buffer[pos])
					+ " found");
		}
		uint index = 0;
		try {
			index = std::stoul(token);
		} catch (std::out_of_range &e) {
			throw produceException("Integer overflow");
		} catch (std::invalid_argument &e) {
			throw produceException("Invalid integer \"" + token + "\"");
		}
		return index;
	}

	/**
	 * Обработка имени шаблона
	 */
	std::string readPatternName() {
		skipSpaces();
		if (getCharacterSets(buffer[pos]) != (LSPL_CHARACTERSETS_LATIN | LSPL_CHARACTERSETS_UPPERCASE))
			throw produceException("Pattern name should start with an uppercase latin letter");
		std::string patternName = readToken();
		if (!isLatin(patternName.back()))
			throw produceException("Pattern name should end with a latin letter");
		return patternName;
	}

	/**
	 * Обработка параметров шаблона
	 */
	std::vector<Expression*> readPatternArguments() {
		std::vector<Expression*> result;
		readStrFollows("(");
		result.push_back(readAttributeExpression());
		while (strFollows(",")) {
			readStrFollows(",");
			result.push_back(readAttributeExpression());
		}
		readStrFollows(")");
		return result;
	}

	/**
	 * Является ли строка регулярным выражением?
	 */
	static bool isRegexp( const std::string & str ) {
		static std::string regexSymbols(".[{()\\*+?|^$'");

		for (uint i = 0; i < regexSymbols.length(); ++i)
			if (str.find(regexSymbols.at(i) ) != std::string::npos)
				return true;

		return false;
	}

	/*
	 * Разделить строку на токены
	 */
	static std::vector<std::string> split(const std::string &contents) {
		std::vector<std::string> words;
		std::string word;
		for (uint i = 0; i < contents.length(); ++i) {
			if (!isSpace(contents[i]))
				word += contents[i];
			else if (word.length() != 0) {
				words.push_back(word);
				word.clear();
			}
		}
		if (word.length() != 0)
			words.push_back(word);
		return words;
	}

	/**
	 * Считать строковую константу
	 */
	std::string readStringConstant() {
		readStrFollows("\"");
		std::string contents;
		while (buffer[pos] != '\0' && buffer[pos] != '\"')
			contents += buffer[pos++];
		if (buffer[pos] == '\0')
			throw produceException("Unexpected end of input (closing \" excepted)");
		readStrFollows("\"");
		return contents;
	}

	/**
	 * элемент_строка := "регулярное выражение"
	 */
	MatcherPtr readStringMatcher() {
		std::string contents = readStringConstant();
		if (isRegexp(contents))
			return MatcherPtr(new RegexpMatcher(contents));

		// Разделяем на отдельные слова, если строка не является
		// регулярным выражением
		std::vector<std::string> words = split(contents);
		if (words.size() == 0)
			throw produceException("Empty string cannot be matched");
		if (words.size() == 1)
			return MatcherPtr(new TokenMatcher(words.front()));

		// Слов больше, чем одно. Создаём отдельные сопоставители для каждого слова
		LoopMatcher *wordMatcher = new LoopMatcher(1, 1);
		MatcherContainer &container = wordMatcher->newAlternative();
		for (std::string &word : words) {
			container.addMatcher(new TokenMatcher(word));
		}
		return MatcherPtr(wordMatcher);
	}

	/**
	 * Обработка элемента шаблона
	 *
	 * элемент_шаблона ::= простой_элемент | опциональный_элемент | повторение_элементов
	 *                                     | (набор_альтернатив)
	 *
	 * простой элемент := элемент_строка | элемент_слово | экземпляр_шаблона
	 */
	MatcherPtr readMatcher() {
		skipSpaces();
		if (strFollows("{"))
			return readNestedMatcher(0, 0, "{", "}", true, false);
		if (strFollows("["))
			return readNestedMatcher(0, 1, "[", "]", false, false);
		if (strFollows("("))
			return readNestedMatcher(1, 1, "(", ")", false, true);
		if (strFollows("\""))
			return readStringMatcher();

		// элемент_слово
		std::string token = readToken();
		if (token == "")
			throw produceException("Matcher expected");
		uint index = 0;
		cutIndexFromToken(token, index);
		for (int i = 0; i < SpeechPart::COUNT; ++i)
			if (token == SpeechPart(i).getAbbrevation())
				return readWordMatcher(SpeechPart(i), index);

		// экземпляр_шаблона
		PatternRef pattern = space->getPatternByName(token);
		if (!pattern)
			throw produceException("No pattern with specified name");
		return readPatternMatcher(pattern, index);
	}

	/*
	 * Считать сопоставитель с вложенным списком альтернатив, т.е.
	 *
	 *   1) опциональный_элемент
	 *   2) (набор_альтернатив)
	 *   3) повторение_элементов
	 *
	 *  В параметрах min и max задаётся минимальное и максимальное количество повторений
	 *  (см. LoopMatcher)
	 *
	 *  lbrace и rbrace -- это открывающая и закрывающая скобка (круглая, квадратная или
	 *  фигурная)
	 *
	 *  Параметр allow задаёт, можно ли переопределять значения min и max в самом коде шаблона
	 *
	 *  Параметр canBeBinding задаёт, может ли вложенный список альтернатив на самом деле
	 *  быть параметрами шаблона
	 *
	 */
	MatcherPtr readNestedMatcher(uint min, uint max, const char* lbrace, const char* rbrace,
								 bool allow, bool canBeBinding) {
		uint before_pos = pos;

		readStrFollows(lbrace);
		std::vector<std::vector<MatcherPtr> > alts;
		try {
			alts = readAlternatives();
		} catch (PatternBuildingException &e) {
			if (!canBeBinding) throw e;
			pos = before_pos;
			return nullptr;
		}
		readStrFollows(rbrace);

		// Потенциально вложенный сопоставитель все ещё может быть параметром шаблона, если
		// параметр всего один. В таком случае нужно проверить, что
		//   1. внутри скобок есть только один параметр;
		//   2. этот параметр не является составным;
		//   3. после параметра ничего нет (кроме, возможно, шаблона извлечения).
		if (canBeBinding && alts.size() == 1 && alts[0].size() == 1
		    && dynamic_cast<LoopMatcher*>(alts[0][0].get()) == nullptr
		    && (seekEndOfLine() || strFollows("="))) {
			pos = before_pos;
			return nullptr;
		}

		if (allow && strFollows("<") && !strFollows("<<")) {
			readStrFollows("<");
			min = readUInt();
			if (strFollows(",")) {
				readStrFollows(",");
				max = readUInt();
			}
			readStrFollows(">");
		}

		LoopMatcher *matcher = new LoopMatcher(min, max);
		for (std::vector<MatcherPtr> &alt : alts)
			matcher->newAlternative().addMatchers(alt);
		return MatcherPtr(matcher);
	}

	/**
	 * Считать основу слова или лемму
	 */
	std::string readMatcherBase() {
		std::string token;
		while (isCyrillic(buffer[pos]) || isDigit(buffer[pos]) || buffer[pos] == '-')
			token += buffer[pos++];
		if (token.empty())
			throw produceException("Empty lemma or stem");
		if (token.front() == '-' || token.back() == '-')
			throw produceException("Lemma or stem can't start/finish with a hyphen");
		return token;
	}

	/**
	 * Считывает последовательность альтернатив лемм/основ
	 */
	AlternativeBaseComparator* readAlternativeBaseComparator(AlternativeBaseComparator *cmp) {
		cmp->addAlternativeBase(readMatcherBase());
		while (strFollows("|")) {
			readStrFollows("|");
			cmp->addAlternativeBase(readMatcherBase());
		}
		return cmp;
	}

	/**
	 * условия_на_лемму ::= [ lemma = ] лемма { | лемма }  |  [ lemma ]  != лемма { | лемма }
	 */
	void readLemmaRestriction(MatcherPtr &matcher) {
		WordMatcher *word_m = dynamic_cast<WordMatcher*>(matcher.get());
		if (word_m == nullptr)
			throw produceException("No lemma restrictions on a non-word matcher");

		bool readAnouncement = false;
		if (strFollows("lemma")) {
			readStrFollows("lemma");
			readAnouncement = true;
		}

		bool negative = false;  // !=
		if (strFollows("="))
			readStrFollows("=");
		else if (strFollows("!=")) {
			negative = true;
			readStrFollows("!=");
		} else if (readAnouncement)
			throw produceException("= or != expected");

		if (strFollows("\""))
			word_m->setBaseComparator(new LemmaRegexpComparator(readStringConstant(), negative));
		else
			word_m->setBaseComparator(readAlternativeBaseComparator(new LemmaComparator(negative)));
	}

	/**
	  * условия_на_основу ::= stem = основа { | основа } | stem  != основа { | основа}
	  */
	void readStemRestriction(MatcherPtr &matcher) {
		readStrFollows("stem");

		WordMatcher *word_m = dynamic_cast<WordMatcher*>(matcher.get());
		if (word_m == nullptr)
			throw produceException("No stem restrictions on a non-word matcher");

		bool negative = false;  // !=
		if (strFollows("="))
			readStrFollows("=");
		else if (strFollows("!=")) {
			negative = true;
			readStrFollows("!=");
		} else
			throw produceException("= or != expected");

		if (strFollows("\""))
			word_m->setBaseComparator(new StemRegexpComparator(readStringConstant(), negative));
		else
			word_m->setBaseComparator(readAlternativeBaseComparator(new StemComparator(negative)));
	}

	/**
	 * Получить имя аттрибута по его имени или сокращению
	 */
	AttributeKey searchForAttributeByName(const std::string &name) {
		AttributeKey key = AttributeKey::findByAbbrevation(name);
		if (key != AttributeKey::UNDEFINED)
			return key;
		key = AttributeKey::findByName(name);
		return key;
	}

	/**
	 * Создаёт для сопоставителя ограничение на характеристику attributeName, которая в качестве
	 * значений может принимать аргументы из набора attributeNames
	 *
	 * negative == true меняет поведения ограничения на обратное (условие не должно выполняться)
	 */
	Restriction* appendMatcherAttributeRestriction(std::string &attributeName, bool negative, std::vector<std::string> attributeNames) {
		std::vector<AttributeValue> values(attributeNames.size());
		std::transform(attributeNames.begin(), attributeNames.end(), values.begin(), AttributeValue::findIndexedByAbbrevation);

		std::vector<AttributeValue>::iterator it;
		if ((it = std::find(values.begin(), values.end(), AttributeValue::UNDEFINED)) != values.end())
			throw produceException("Unknown attribute value \"" + attributeNames[it - values.begin()] + "\"");

		AttributeKey key = attributeName != "" ? searchForAttributeByName(attributeName)
											   : Morphology::instance().getAttributeKeyByValue(attributeNames.front());
		if (key == AttributeKey::UNDEFINED)
			throw produceException("Unable to retrieve attribute type");

		it = std::find_if(values.begin(), values.end(), [&](AttributeValue &v){ return Morphology::instance().getAttributeKeyByValue(v) != key; });
		if (it != values.end())
			throw produceException("Attribute value \"" + attributeNames[it - values.begin()] + "\" doesn't correspond to common attribute type");

		// Аргрументы проверены, можно собирать ограничение
		Restriction* result;
		std::vector<AgreementRestriction*> alternativeRestrictions;
		for (AttributeValue &v: values){
			AgreementRestriction *r = new AgreementRestriction();
			r->addArgument(new AttributeExpression(new CurrentAnnotationExpression(), key));
			r->addArgument(new ConstantExpression(v));
			alternativeRestrictions.push_back(r);
		}

		if (alternativeRestrictions.size() == 1)
			result = alternativeRestrictions.front();
		else {
			OrRestriction *r = new OrRestriction();
			for (Restriction *alt : alternativeRestrictions)
				r->addArgument(alt);
			result = r;
		}

		if (negative)
			result = new NotRestriction(result);
		return result;
	}


	/**
	 * Чтение одного ограничения на аттрибут
	 *
	 * характеристика ::= [ название_ признака = ]  значение_ признака { | значение_ признака } |
	 *                    [ название_ признака ]  != значение_ признака { | значение_ признака }
	 *
	 */
	void readAttributeRestriction(MatcherPtr &matcher) {
		bool negative = false; // !=
		std::string attributeName;
		std::vector<std::string> valueNames;

		skipSpaces();
		if (isLatin(buffer[pos])) {
			attributeName = readToken();
			if (strFollows("|") || strFollows(",") || strFollows(">")) {
				valueNames.emplace_back(std::move(attributeName));
				if (strFollows("|")) readStrFollows("|");
			}
			else if (strFollows("="))
				readStrFollows("=");
			else if (!strFollows("!="))
				throw produceException("Invalid expression in matcher restriction");
		}

		if (strFollows("!=")) {
			readStrFollows("!=");
			negative = true;
		};

		skipSpaces();
		if (!isLatin(buffer[pos]))
			return;

		valueNames.push_back(readToken());
		while (strFollows("|")) {
			readStrFollows("");
			valueNames.push_back(readToken());
			if (valueNames.back().length() == 0)
				throw produceException("Expression value expected");
		}

		matcher->addRestriction(appendMatcherAttributeRestriction(attributeName, negative, valueNames));
	}

	/**
	 * Чтение одного ограничения сопоставителя
	 *
	 */
	void readMatcherRestriction(MatcherPtr &matcher) {
		if (strFollows("lemma") || isCyrillic(buffer[pos]) || strFollows("\""))
			readLemmaRestriction(matcher);
		else if (strFollows("stem"))
			readStemRestriction(matcher);
		else if (strFollows("!=")) {
			// По одному != непонятно, что будет дальше, ограничение на лемму или на признак
			// Запоминаем позицию для отката назад, если не угадали
			uint backtrack_pos = pos;
			readStrFollows("!=");
			skipSpaces();
			if (isCyrillic(buffer[pos]) || isDigit(buffer[pos])) {
				pos = backtrack_pos;
				readLemmaRestriction(matcher);
			} else {
				pos = backtrack_pos;
				readAttributeRestriction(matcher);
			}

		} else
			readAttributeRestriction(matcher);
	}

	/*
	 * Чтение списка ограничений сопоставителя
	 */
	void readMatcherRestrictions(MatcherPtr &matcher) {
		readStrFollows("<");
		readMatcherRestriction(matcher);
		while (!strFollows(">")) {
			readStrFollows(",");
			readAttributeRestriction(matcher);
		}
		readStrFollows(">");
	}

	/**
	 * Считать сопоставитель-шаблон
	 *
	 * экземпляр-шаблона ::= имя_шаблона [индекс] | имя_шаблона [индекс]  <характеристика { , характеристика }>
	 */
	MatcherPtr readPatternMatcher(PatternRef pattern, uint index) {
		PatternMatcher *matcher = new PatternMatcher(*pattern);
		MatcherPtr result(matcher);
		matcher->variable = Variable(*pattern, index);
		if (strFollows("<") && !strFollows("<<"))
			readMatcherRestrictions(result);
		return result;
	}

	/*
	 * Считать сопоставитель-слово
	 */
	MatcherPtr readWordMatcher(const SpeechPart &sp, uint index) {
		WordMatcher *matcher = new WordMatcher(sp);
		MatcherPtr result(matcher);
		matcher->variable = Variable(sp, index);
		if (strFollows("<") && !strFollows("<<"))
			readMatcherRestrictions(result);
		return result;
	}

	/*
	 * Сгенерировать сопоставитель, реализующий перестановку из
	 * указанного набора сопоставителей
	 */
	MatcherPtr makePermutationMatcher(std::vector<MatcherPtr> source) {
		if (source.size() == 0)
			throw produceException("Internal error: empty permutation requested");
		if (source.size() == 1)
			return std::move(source.front());

		std::vector<Matcher*> permutation;
		for (uint i = 0; i < source.size(); ++i)
			permutation.push_back(source[i].release());
		LoopMatcher *wordMatcher = new LoopMatcher(1, 1, true);
		sort(permutation.begin(), permutation.end());
		do {
			wordMatcher->newAlternative().addMatchers(permutation.begin(), permutation.end());
		} while (next_permutation(permutation.begin(), permutation.end()));
		return MatcherPtr(wordMatcher);
	}

	/**
	 * Считать одно имя переменной + признак (если есть)
	 *
	 * имя ::= имя_элемента  [. название_признака]   | имя_шаблона [индекс]  [. название_признака]
	 */
	Expression* readAttributeExpression() {
		std::string token = readToken();
		if (token == "")
			throw produceException("Variable name expected");
		uint index = 0;
		cutIndexFromToken(token, index);

		// Элемент-слово
		Expression* result = nullptr;
		for (int i = 0; i < SpeechPart::COUNT; ++i)
			if (token == SpeechPart(i).getAbbrevation()) {
				result = new VariableExpression(SpeechPart(i), index);
				break;
			}

		// Шаблон
		if (!result) {
			PatternRef pattern = space->getPatternByName(token);
			if (!pattern)
				throw produceException("No pattern with specified name");
			result = new VariableExpression(*pattern, index);
		}

		if (strFollows(".")) {
			readStrFollows(".");
			AttributeKey key = AttributeKey::findByAbbrevation(readToken());
			if (key == AttributeKey::UNDEFINED)
				throw produceException("Unknown attribute");
			result = new AttributeExpression(result, key);
		}

		return result;
	}

	/**
	 * Считать = или ==
	 */
	std::string readAgreement() {
		if (strFollows("==")) {
			readStrFollows("==");
			return "==";
		}
		if (strFollows("=")) {
			readStrFollows("=");
			return "=";
		}
		throw produceException("= or == expected");
	}

	template<class MatcherPtrT>
	bool tryToAddAgreementRestriction(MatcherPtrT* matchers, int size, SharedRestriction &r) {
		Variable nullvar;
		for (int i = size - 1; i >= 0; --i)
			if (matchers[i]->variable == nullvar) {
				LoopMatcher *loop = dynamic_cast<LoopMatcher*>(&*matchers[i]);
				if (!loop)
					continue;
				bool restrictionAdded = false;
				if (loop->is_permutation) {
					for (Matcher &matcher : loop->alternatives[0].getMatchers())
						if (matcher.variable != nullvar && r.containsVariable(matcher.variable)) {
							restrictionAdded = true;
							matcher.addRestriction(new SharedRestriction(r));
						}
				} else {
					// В целом, ситуаций, когда LoopMatcher оказывается не-перестановкой быть не должно,
					// но мы все же рассмотрим её
					for (MatcherContainer& alt : loop->alternatives)
						restrictionAdded |= tryToAddAgreementRestriction(alt.getMatchers().c_array(), alt.getMatchers().size(), r);
				}
				return restrictionAdded;
			} else if (r.containsVariable(matchers[i]->variable)) {
				matchers[i]->addRestriction(new SharedRestriction(r));
				return true;
			}
		return false;
	}

	/**
	 * Считать одно ограничение согласования для перестановки
	 *
	 * условие ::= условие_ согласования
	 * условие_ согласования ::= имя = имя { = имя } | имя == имя { == имя }
	 */
	void readPermutationRestriction(std::vector<MatcherPtr> &matchers) {
		std::vector<Expression*> exps(1, readAttributeExpression());
		std::string agreementType = readAgreement();
		exps.push_back(readAttributeExpression());
		while (strFollows("=")) {
			if (readAgreement() != agreementType)
				throw produceException("Weak (=) and strong (==) agreements mixed");
			exps.push_back(readAttributeExpression());
		}

		AgreementRestriction *agreement_r = new AgreementRestriction(agreementType == "=");
		for (Expression *e : exps)
			agreement_r->addArgument(e);

		SharedRestriction shared_r(agreement_r);
		if (tryToAddAgreementRestriction(matchers.data(), matchers.size(), shared_r))
			return;
		for (Expression *e : exps)
			delete e;
	}

	/**
	 * Считать ограничения согласования для перестановки
	 *
	 * условия ::= условие {,  условие }
	 */
	void readPermutationRestrictions(std::vector<MatcherPtr> &matchers) {
		readStrFollows("<<");
		readPermutationRestriction(matchers);
		while (strFollows(",")) {
			readStrFollows(",");
			readPermutationRestriction(matchers);
		}
		readStrFollows(">>");
	}

	/**
	 * Обработка шаблона распознавания (последовательности перестановок)
	 *
	 * шаблон_распознавания ::= последовательность_перестановок
	 *
	 * последовательность_перестановок ::= последовательность_элементов
	 *                                     { ~ последовательность_элементов }
	 *                                     [ <<условия>> ]
	 *
	 * последовательность элементов := элемент_шаблона { элемент_шаблона }
	 *
	 */
	std::vector<MatcherPtr> readPermutation() {
		std::vector<MatcherPtr> matchers;
		std::vector<MatcherPtr> permutation;
		permutation.push_back(readMatcher());
		if (!permutation.back()) {
			permutation.pop_back();
			return permutation;
		}

		static std::string followers = "[({\"~";

		while (!seekEndOfInput()
			   && (isLatin(buffer[pos]) || followers.find(buffer[pos]) != std::string::npos)
			   && permutation.back()) {
			if (strFollows("~"))
				readStrFollows("~");
			else
				matchers.push_back(makePermutationMatcher(std::move(permutation)));
			permutation.push_back(readMatcher());
		}

		if (!permutation.back())
			permutation.pop_back();
		if (!permutation.empty())
			matchers.push_back(makePermutationMatcher(std::move(permutation)));
		if (strFollows("<<"))
			readPermutationRestrictions(matchers);
		return matchers;
	}

	std::vector<std::vector<MatcherPtr> > readAlternatives() {
		std::vector<std::vector<MatcherPtr> > alts;
		alts.push_back(readPermutation());
		while (strFollows("|")) {
			readStrFollows("|");
			alts.push_back(readPermutation());
		}
		return alts;
	}

	/**
	 * Считать альтернативу шаблона pattern, сохранив её source
	 */
	void readAlternativeWithSource(const PatternPtr &pattern) {
		uint before_pos = pos;
		std::vector<MatcherPtr> alt = readPermutation();
		pattern->newAlternative(std::string(buffer + before_pos, buffer + pos)).addMatchers(alt);
	}

	/**
	 * Прицепить к альтернативе параметр шаблона
	 */
	void appendAlternativeBinding(Alternative &alt, Expression *exp) {
		AttributeKey key = AttributeKey::UNDEFINED;
		Variable var;

		if (dynamic_cast<AttributeExpression*>(exp)) {
			// Переменная с аттрибутом
			AttributeExpression* attrexp = static_cast<AttributeExpression*>(exp);
			key = attrexp->attribute;
			const VariableExpression* varexp = dynamic_cast<const VariableExpression*>(attrexp->base);
			if (!varexp) throw new std::logic_error("Invalid alternative binding in parser");
			var = varexp->getVariable();
		} else if (dynamic_cast<VariableExpression*>(exp)) {
			// Просто переменная, без аттрибута
			var = static_cast<VariableExpression*>(exp)->getVariable();
		} else
			throw new std::logic_error("Invalid alternative binding in parser");

		bool applicable = false;
		for (uint i = 0; i < alt.getMatcherCount(); ++i)
			if (alt.getMatcher(i).containsVariable(var))
				applicable = true;

		// Создаём связывание и добавляем его к альтенративе шаблона
		if (applicable) {
			Expression *e = new VariableExpression(var);
			if (key != AttributeKey::UNDEFINED)
				e = new AttributeExpression(e, key);
			alt.addBinding(key, e);
		}
	}

	/**
	 * Обработка шаблона
	 *
	 * описание_шаблона ::= имя_шаблона [ (параметры_шаблона) ] =
	 *                      шаблон_распознавания { | шаблон_распознавания }
	 *                      [ (параметры_шаблона) ]
	 *                      [ =text> шаблоны_извлечения_текста ]
	 *
	 */
	void readPattern() {
		std::string patternName = readPatternName();

		PatternPtr pattern(new Pattern(patternName));
		std::vector<Expression*> arguments;
		uint alternativeCountBefore = pattern->alternatives.size();
		bool hasPatternAttributes = false;
		// Параметры шаблона (слева)
		if (strFollows("(")) {
			hasPatternAttributes = true;
			arguments = readPatternArguments();
		}

		// Описание шаблона
		readStrFollows("=");
		readAlternativeWithSource(pattern);
		while (strFollows("|")) {
			readStrFollows("|");
			readAlternativeWithSource(pattern);
		}

		// Параметры шаблона (справа)
		if (strFollows("(")) {
			if (hasPatternAttributes)
				throw produceException("Double pattern attributes declaration");
			hasPatternAttributes = true;
			arguments = readPatternArguments();
		}

		// Подключение параметров шаблона к соответствующим им альтернативам
		for (Expression *exp : arguments) {
			for (uint i = alternativeCountBefore; i < pattern->alternatives.size(); ++i)
				appendAlternativeBinding(*pattern->alternatives[i], exp);
			delete exp;
		}

		// Преобразование(?)
		if (strFollows("=")) {
			readStrFollows("=");
			std::string transformName = readToken();
			readStrFollows(">");

			auto tf = transformBuilders.find(transformName);
			if (tf == transformBuilders.end())
				throw produceException("Undefined transform name =" + transformName + ">");

			// Предсказываем окончание преобразования
			uint end_pos = pos;
			while (buffer[end_pos] != '\0' && buffer[end_pos] != '\n' && buffer[end_pos] != '\r')
				++end_pos;

			for (uint i = alternativeCountBefore; i < pattern->alternatives.size(); ++i) {
				pattern->alternatives[i]->transformSource = std::string(buffer + pos, buffer + end_pos);
				pattern->alternatives[i]->setTransform(
						std::auto_ptr<transforms::Transform>(tf->second->build(
								*pattern->alternatives[i],
								pattern->alternatives[i]->getTransformSource()
						))
				);
			}

			pos = end_pos;
		}

		PatternRef patternRef = space->getPatternByName(patternName);
		if (!patternRef) {
			space->addPattern(pattern.release());
		} else
			patternRef->mergePattern(*pattern);
	}

public:
	ParserImpl(NamespaceRef space, const std::map<std::string, transforms::TransformBuilderRef>& tbs):
		Parser(space, tbs), buffer(nullptr), pos(0) {}
	~ParserImpl() {}

	PatternBuilder::BuildInfo build(const char * str) {
		buffer = str;
		pos = 0;

		while (!seekEndOfInput())
			readPattern();

		PatternBuilder::BuildInfo bi;
		bi.parseLength = pos;
		bi.parseTail = std::string(buffer + pos);
		return bi;
	}
};


PatternBuilder::PatternBuilder( const NamespaceRef & ns ) :
    space( ns ),
    parser( new ParserImpl( space, transformBuilders ) ) {
    transformBuilders.insert(std::make_pair("text", new transforms::TextTransformBuilder( space )));
    transformBuilders.insert(std::make_pair("pattern", new transforms::PatternTransformBuilder( space )));
}

PatternBuilder::~PatternBuilder() {
}

PatternBuilder::BuildInfo PatternBuilder::build( const std::string & str ) {
	return parser->build( str.c_str() );
}

} } // namespace lspl::patterns
