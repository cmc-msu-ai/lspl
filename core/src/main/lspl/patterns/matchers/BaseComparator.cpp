#include "BaseComparator.h"
#include "../../morphology/Morphology.h"

using lspl::text::markup::Word;
using lspl::morphology::Morphology;

namespace lspl { namespace patterns { namespace matchers {

BaseComparator::~BaseComparator() {
}

void AlternativeBaseComparator::addAlternativeBase(const std::string &base) {
	alts.push_back(Morphology::instance().upcase(base));
}

bool AlternativeBaseComparator::equals(const BaseComparator &other) const {
	if (typeid(other) != typeid(*this))
		return false;
	return alts == dynamic_cast<const AlternativeBaseComparator &>(other).alts;
}

void AlternativeBaseComparator::dump(std::ostream & out, const std::string & tabs) const {
	out << tabs << "BaseComparator { " << getBaseTypeName() << ' ' << (negative ? "!=" : "==") << ' ';
	for (uint i = 0; i < alts.size(); ++i) {
		if (i) out << " | ";
		out << alts[i];
	}
	out << " }";
}

bool AlternativeBaseComparator::match(const Word &word) const {
	bool result = negative;
	for (const std::string &alt : alts)
		if ((getWordValue(word) == alt) ^ negative) {
			result = !result;
			break;
		}
	return result;
}

RegexpBaseComparator::RegexpBaseComparator(const std::string &token, bool negative):
	exp(Morphology::instance().upcase( token )), negative(negative) {}

void RegexpBaseComparator::dump(std::ostream &out, const std::string &tabs) const {
	out << tabs << "BaseComparator { " << getBaseTypeName() << " = RegExp("
		<< exp.pattern() << ") }";
}

bool RegexpBaseComparator::match(const Word &word) const {
	return exp.FullMatch(getWordValue(word));
}

bool RegexpBaseComparator::equals(const BaseComparator &other) const {
	if (typeid(other) != typeid(*this))
		return false;
	return exp.pattern() == dynamic_cast<const RegexpBaseComparator&>(other).exp.pattern();
}

// Конструкторы

LemmaComparator::LemmaComparator(bool negative) { this->negative = negative; }
StemComparator::StemComparator(bool negative) { this->negative = negative; }

LemmaRegexpComparator::LemmaRegexpComparator(const std::string &token, bool negative):
		RegexpBaseComparator(token, negative) {}
StemRegexpComparator::StemRegexpComparator(const std::string &token, bool negative):
		RegexpBaseComparator(token, negative) {}

// Имена типов

static std::string LEMMA = "lemma";
static std::string STEM = "stem";

const std::string& LemmaComparator::getBaseTypeName() const { return LEMMA; }
const std::string& LemmaRegexpComparator::getBaseTypeName() const { return LEMMA; }
const std::string& StemComparator::getBaseTypeName() const { return STEM; }
const std::string& StemRegexpComparator::getBaseTypeName() const { return STEM; }

// Получение значений из слова

std::string LemmaComparator::getWordValue(const Word &word) const {
	return word.getBase();
}

std::string LemmaRegexpComparator::getWordValue(const Word &word) const {
	return word.getBase();
}

std::string StemComparator::getWordValue(const Word &word) const {
	return word.getStem();
}

std::string StemRegexpComparator::getWordValue(const Word &word) const {
	return word.getStem();
}

}}}
