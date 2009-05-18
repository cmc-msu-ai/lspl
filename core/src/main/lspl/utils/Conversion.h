#ifndef _LSPL_UTILS_CONVERSION_H_
#define _LSPL_UTILS_CONVERSION_H_

#include "../base/Base.h"

#include <string>

namespace lspl { namespace utils {

/**
 * Класс, осуществляющий преобразование строк из одной кодировки в другую.
 *
 * Преобразование осуществляется методами convert, или вызовом оператора. Пример использования:
 *
 * Conversion out("UTF-8","CP1251");
 * std::cout << out( "Сообщение" );
 * std::cout << out.convert( "Сообщение" );
 */
class LSPL_EXPORT Conversion {
public:
	static const std::string DEFAULT_ENCODING;
public:
	/**
	 * Конструктор пустого преобразования (то есть не выполняющий никаких действий вообще)
	 */
	Conversion();

	/**
	 * Конструктор преобразования между заданными кодировками
	 * @param from имя кодировки из которой преобразовывать
	 * @param to имя кодировки в которую преобразовывать
	 */
	Conversion( const char * from, const char * to );

	/**
	 * Конструктор преобразования между заданными кодировками
	 * @param from имя кодировки из которой преобразовывать
	 * @param to имя кодировки в которую преобразовывать
	 */
	Conversion( const std::string & from, const std::string & to );

	/**
	 * Конструктор копирования
	 */
	Conversion( const Conversion & conv );

	virtual ~Conversion();

	/**
	 * Преобразование для строки STL
	 */
	std::string convert( const std::string & s ) const {
		if ( descriptor == 0 )
			return s;

		return convert( s.data(), s.size() );
	}

	/**
	 * Преобразование для C-строки
	 */
	std::string convert( const char * str ) const;

	/**
	* Преобразование для C-строки заданной длины
	 */
	std::string convert( const char * data, size_t size ) const;

	std::string operator ()( const std::string & s ) const {
		return convert( s );
	}

	/**
	 * Перегруженный оператор, позволяет использовать преобразование следующим образом:
	 * <code>
	 *   Conversion out = ...;
	 *   std::string ss = out( "123" );
	 * </code>
	 */
	std::string operator ()( const char * str ) const {
		return convert( str );
	}

	Conversion & operator = ( const Conversion & conv );
private:
	void free();
private:
	void * descriptor;
	int * refcount;
};

} }

#endif /*_LSPL_UTILS_CONVERSION_H_*/
