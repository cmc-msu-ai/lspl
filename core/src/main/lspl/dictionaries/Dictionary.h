/*
 * Dictionary.h
 *
 *  Created on: Sep 27, 2008
 *      Author: alno
 */
#ifndef _LSPL_DICTIONARIES_DICTIONARY_H_
#define _LSPL_DICTIONARIES_DICTIONARY_H_

#include "../base/Base.h"
#include "../base/RefCountObject.h"
#include "../base/RefCountPtr.h"

#include <string>
#include <vector>

namespace lspl { namespace dictionaries {

LSPL_REFCOUNT_FORWARD(Dictionary);

class LSPL_EXPORT Dictionary : public base::RefCountObject<Dictionary> {
public:
	Dictionary( const std::string & name );
	virtual ~Dictionary();

	/**
	 * Проверить, принимает ли словарь заданный набор слов
	 * @param words вектор слов
	 * @return true, если набор слов принимается словарем
	 */
	bool accepts( const std::vector<std::string> & words ) const {
		return acceptsVector( words );
	};

	bool accepts( const std::string & w1 ) const;

	bool accepts( const std::string & w1, const std::string & w2 ) const;

	bool accepts( const std::string & w1, const std::string & w2, const std::string & w3 ) const;

protected:

	/**
	 * Проверить, принимает ли словарь заданный набор слов. Этот метод является полностью вирутальным и требует перегрузки в классах-потомках.
	 * @param words вектор слов
	 * @return true, если набор слов принимается словарем
	 */
	virtual bool acceptsVector( const std::vector<std::string> & words ) const = 0;

public:

	/**
	 * Имя словаря
	 */
	const std::string name;
};

} }

#endif//_LSPL_DICTIONARIES_DICTIONARY_H_
