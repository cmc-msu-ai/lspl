#ifndef _LSPL_MATCHERS_BASECOMPARATOR_H_
#define _LSPL_MATCHERS_BASECOMPARATOR_H_

#include "Forward.h"
#include "../../text/markup/Word.h"

#include <pcrecpp.h>

namespace lspl { namespace patterns { namespace matchers {

/**
 * Base Comparator является частью WordMatcher и позволяет ограничить
 * применение сопоставителя только для слов с определенной леммой или основой
 */
class LSPL_EXPORT BaseComparator {
public:
	virtual ~BaseComparator();

	/**
	 * Метод возвращает true, если данное слово согласовано с ограничением
	 * В базовом классе по умолчанию согласует все слова
	 */
	virtual bool match(const lspl::text::markup::Word &) const = 0;
	virtual void dump(std::ostream & out, const std::string & tabs = "") const = 0;
	virtual bool equals(const BaseComparator &) const = 0;
};


class LSPL_EXPORT AlternativeBaseComparator: public BaseComparator {
public:
	virtual std::string getWordValue(const lspl::text::markup::Word &) const = 0;
	virtual const std::string& getBaseTypeName() const = 0;
	virtual bool match(const lspl::text::markup::Word &) const;
	virtual bool equals(const BaseComparator &) const;
	virtual void addAlternativeBase(const std::string &);
	virtual void dump(std::ostream & out, const std::string & tabs = "") const;

public:
	std::vector<std::string> alts;
	bool negative; // Если true, то !=
};

class LSPL_EXPORT RegexpBaseComparator: public BaseComparator {
public:
	RegexpBaseComparator(const std::string &token, bool negative = false);

	virtual std::string getWordValue(const lspl::text::markup::Word &) const = 0;
	virtual const std::string& getBaseTypeName() const = 0;
	virtual bool match(const lspl::text::markup::Word &) const;
	virtual void dump(std::ostream & out, const std::string & tabs = "") const;
	virtual bool equals(const BaseComparator &) const;

public:
	const pcrecpp::RE exp;
	bool negative;
};

/**
 * Ограничение на лемму
 */
class LSPL_EXPORT LemmaComparator: public AlternativeBaseComparator {
public:
	LemmaComparator(bool negative = false);

	virtual std::string getWordValue(const lspl::text::markup::Word &) const;
	virtual const std::string& getBaseTypeName() const;
};

/**
 * Ограничение на основу
 */
class LSPL_EXPORT StemComparator: public AlternativeBaseComparator {
public:
	StemComparator(bool negative = false);

	virtual std::string getWordValue(const lspl::text::markup::Word &) const;
	virtual const std::string& getBaseTypeName() const;
};

/**
 * Ограничение на лемму по регулярному выражению
 */
class LSPL_EXPORT LemmaRegexpComparator: public RegexpBaseComparator {
public:
	LemmaRegexpComparator(const std::string &token, bool negative = false);

	virtual std::string getWordValue(const lspl::text::markup::Word &) const;
	virtual const std::string& getBaseTypeName() const;
};

/**
 * Ограничение на основу по регулярному выражению
 */
class LSPL_EXPORT StemRegexpComparator: public RegexpBaseComparator {
public:
	StemRegexpComparator(const std::string &token, bool negative = false);

	virtual std::string getWordValue(const lspl::text::markup::Word &) const;
	virtual const std::string& getBaseTypeName() const;
};

}}}

#endif /*_LSPL_MATCHERS_BASECOMPARATOR_H_*/
