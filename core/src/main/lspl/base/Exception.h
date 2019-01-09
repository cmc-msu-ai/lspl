#ifndef _LSPL_BASE_EXCEPTION_H_
#define _LSPL_BASE_EXCEPTION_H_

#include "Base.h"

#include <string>
#include <stdexcept>

namespace lspl { namespace base {

/**
 * Базовый класс исключения в библиотеке
 */
class Exception : public std::exception {
public:
	Exception(const std::string & description, const std::string & sourceFileName, int sourceLineNumber) :
		description(description),
		sourceFileName(sourceFileName),
		sourceLineNumber(sourceLineNumber) {}

	Exception(const std::string & description) :
		description(description) {}

	~Exception() {}
	const char* what() const noexcept override { return description.c_str(); }

private:

	/**
	 * Описание исключения
	 */
	const std::string description;

	/**
	 * Имя файла исходника
	 */
	std::string sourceFileName;

	/**
	 * Номер строки  исходника
	 */
	int sourceLineNumber;
};

} }

#endif /*_LSPL_BASE_EXCEPTION_H_*/
