#include "../base/BaseInternal.h"

#include "Conversion.h"

#include <iconv.h>
#include <string.h>

namespace lspl { namespace utils {

const std::string Conversion::DEFAULT_ENCODING("CP1251");

Conversion::Conversion() :
	descriptor( 0 ), refcount( 0 ) {
}

Conversion::Conversion( const char * from, const char * to ) :
	descriptor( iconv_open( to, from ) ), refcount( new int( 1 ) ) {
}

Conversion::Conversion( const std::string & from, const std::string & to ) :
	descriptor( iconv_open( to.c_str(), from.c_str() ) ), refcount( new int( 1 ) ) {}

Conversion::Conversion( const Conversion & conv ) :
	descriptor( conv.descriptor ), refcount( conv.refcount ) {

	if ( descriptor )
		++ *refcount;
}

Conversion & Conversion::operator = ( const Conversion & conv ) {
	/*
	 * Предварительно сохраняем указатель на дескриптор и счетчик ссылок, чтобы избежать освобождения в случае присваивания себя
	 */
	void * descr = conv.descriptor;
	int * refc = conv.refcount;

	if ( conv.descriptor ) // Если кодировка осуществляет преобразование
		++ *conv.refcount; // Увеличиваем счетчик ссылок

	free();

	descriptor = descr;
	refcount = refc;

	return *this;
}

Conversion::~Conversion() {
	free();
}

void Conversion::free() {
	if ( descriptor == 0 )
		return;

	if ( -- *refcount <= 0 ) {
		delete refcount;
		iconv_close( descriptor );
	}

	refcount = 0;
	descriptor = 0;
}

std::string Conversion::convert( const char * str ) const {
	return convert( str, strlen( str ) );
}

std::string Conversion::convert( const char * in_data, size_t in_size ) const {
	if ( descriptor == 0 )
		return std::string( in_data, in_size );

	std::string out( in_size * 3, 0 );
	size_t out_size = out.size();
	size_t out_size_ = out.size();

	char * in_buf = const_cast<char*>( in_data );
	char * out_buf = const_cast<char*>( out.data() );
	char * out_buf_ = out_buf;

#ifdef WIN32
	iconv( descriptor, (const char **)&in_buf, &in_size, &out_buf_, &out_size_ );
#else
	iconv( descriptor, &in_buf, &in_size, &out_buf_, &out_size_ );
#endif

	out.resize( out_size - out_size_ );

	return out;
}

}}
