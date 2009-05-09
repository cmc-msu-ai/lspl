/*
 * Parser.h
 *
 *  Created on: Dec 24, 2008
 *      Author: alno
 */

#ifndef _LSPL_TERMS_PARSER_H_
#define _LSPL_TERMS_PARSER_H_

#include "Term.h"

#include <memory>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

namespace lspl { namespace terms {

struct ParseInfo {
	boost::shared_ptr<Term> term;

	bool hit;
	const char * stop;
};

class Parser {
public:
	Parser();
	~Parser();

	std::auto_ptr<ParseInfo> parse(const char * str);
private:
	class Impl;
private:
	boost::scoped_ptr<Impl> impl;
};

} }

#endif /* _LSPL_TERMS_PARSER_H_ */
