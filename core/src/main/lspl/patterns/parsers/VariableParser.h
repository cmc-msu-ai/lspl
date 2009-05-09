#ifndef _LSPL_PATTERNS_PARSERS_VARIABLEPARSER_H_
#define _LSPL_PATTERNS_PARSERS_VARIABLEPARSER_H_

#include "../matchers/Variable.h"

#include <string>

#include <boost/spirit/core.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/utility/distinct.hpp>
#include <boost/spirit/utility/functor_parser.hpp>

namespace lspl { namespace patterns { namespace parsers {

struct VariableParserImpl {
	typedef matchers::Variable result_t;
	
	VariableParserImpl( boost::spirit::symbols<uint> & typeSymbol ) : typeSymbol( typeSymbol ) {
	}
	
	template<typename ScannerT> std::ptrdiff_t operator()(ScannerT const& scan, result_t& result) const {
		using namespace phoenix;
		using namespace boost::spirit;
		using matchers::Variable;
				
		uint type = 0;
		uint index = 0;		
		rule< ScannerT > p = typeSymbol[ var( type ) = arg1 ] >> !uint_p[ var( index ) = arg1 ];		
		std::ptrdiff_t r = p.parse_main( scan ).length();
		
		result = Variable( type, index );		
		return r;
	}
	
private:
	boost::spirit::symbols<uint> & typeSymbol;	
};

typedef boost::spirit::functor_parser<VariableParserImpl> VariableParser;

} } }

#endif//_LSPL_PATTERNS_PARSERS_VARIABLEPARSER_H_
