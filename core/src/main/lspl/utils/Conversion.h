#ifndef _LSPL_UTILS_CONVERSION_H_
#define _LSPL_UTILS_CONVERSION_H_

#include "../base/Base.h"

#include <string>

namespace lspl { namespace utils {

class LSPL_EXPORT Conversion
{
public:
	Conversion();
	Conversion( const char * from, const char * to );
	Conversion( const std::string & from, const std::string & to );
	
	Conversion( const Conversion & conv );
	
	virtual ~Conversion();
	
	std::string convert( const std::string & s ) const { 
		if ( descriptor == 0 )
			return s;

		return convert( s.data(), s.size() ); 
	}

	std::string convert( const char * data, size_t size ) const;
	
	Conversion & operator = ( const Conversion & conv );
private:
	void free();
private:
	void * descriptor;
	int * refcount;
};

} }

#endif /*_LSPL_UTILS_CONVERSION_H_*/
