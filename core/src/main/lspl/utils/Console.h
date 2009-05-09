#ifndef _LSPL_UTILS_CONSOLE_H_
#define _LSPL_UTILS_CONSOLE_H_

#include "../base/Base.h"

#include "Conversion.h"

#include <iostream>
#include <string>

namespace lspl { namespace utils {

class LSPL_EXPORT Console {
public:
	Console( std::istream & input, std::ostream & output );
	Console( std::istream & input, std::ostream & output, const std::string & encoding );
	virtual ~Console();
	
	void setEncoding( const std::string & encoding );
	
	void run();
public:
	static const std::string INTERNAL_ENCODING; 
private:
	/*
	 * Потоки ввода-вывода
	 */
	std::istream & input; // Поток ввода консоли
	std::ostream & output; // Поток вывода консоли
		
	/*
	 * Преобразование кодировок
	 */
	std::string encoding; // Кодировка консоли
	bool conversionNeeded; // Флаг необходимости преобразования кодировки
	Conversion inputConversion; // Объект преобразования ввода
	Conversion outputConversion; // Объект преобразования вывода
};

} }

#endif /*_LSPL_UTILS_CONSOLE_H_*/
