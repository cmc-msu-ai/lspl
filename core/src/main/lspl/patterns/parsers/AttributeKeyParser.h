#ifndef _LSPL_PATTERNS_PARSERS_ATTRIBUTEKEYPARSER_H_
#define _LSPL_PATTERNS_PARSERS_ATTRIBUTEKEYPARSER_H_

#include "../../text/attributes/AttributeKey.h"

#include <string>

#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/utility/distinct.hpp>
#include <boost/spirit/utility/functor_parser.hpp>

namespace lspl { namespace patterns { namespace parsers {

struct AttributeKeyParserImpl {
	typedef lspl::text::attributes::AttributeKey result_t;
	
	AttributeKeyParserImpl() {
	}
	
	template<typename ScannerT> std::ptrdiff_t operator()(ScannerT const& scan, result_t& result) const {
		using namespace phoenix;
		using namespace boost::spirit;
		using lspl::text::attributes::AttributeKey;
		
		std::string attKey;		
		rule< ScannerT > p = ( +chset_p("a-z") )[ var( attKey ) = construct_<std::string>( arg1, arg2 ) ];
		std::ptrdiff_t r = p.parse_main( scan ).length();
				
		if ( result_t::UNDEFINED != ( result = AttributeKey::findByAbbrevation( attKey ) ) ) // Ищем аттрибут по аббревиатуре
			return r;
 
		result = AttributeKey::create( attKey, attKey, attKey ); // Создаем новый аттрибут		
		return r;
	}
};

typedef boost::spirit::functor_parser<AttributeKeyParserImpl> AttributeKeyParser;

} } }

#endif//_LSPL_PATTERNS_PARSERS_ATTRIBUTEKEYPARSER_H_
