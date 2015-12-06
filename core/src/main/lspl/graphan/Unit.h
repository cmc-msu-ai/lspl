/*
 * Token.h
 *
 *  Created on: May 9, 2009
 *      Author: alno
 */

#ifndef _LSPL_GRAPHAN_TOKEN_H_
#define _LSPL_GRAPHAN_TOKEN_H_

#include <string>
#include "../base/Base.h"

namespace lspl { namespace graphan {

class LSPL_EXPORT Unit {
public:
	enum Type {
		WORD,
		PUNCT,
		UNKNOWN
	};
public:
	Unit( const std::string & str, uint offset, Type type );
	Unit( const char * begin, uint length, uint offset, Type type );
	virtual ~Unit();

	Type getType() const {
		return type;
	}

	const std::string & getString() const {
		return str;
	}

	uint getOffset() const {
		return offset;
	}

	uint getLength() const {
		return str.length();
	}

private:
	std::string str;

	uint offset;

	Type type;
};

} }

#endif /* _LSPL_GRAPHAN_TOKEN_H_ */
