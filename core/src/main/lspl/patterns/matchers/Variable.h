#ifndef _LSPL_MATCHERS_VARIABLE_H_
#define _LSPL_MATCHERS_VARIABLE_H_

#include "Forward.h"

#include "../../text/attributes/SpeechPart.h"

#include <ostream>

namespace lspl { namespace patterns {

class Pattern;

} }

namespace lspl { namespace patterns { namespace matchers {

class LSPL_EXPORT Variable {
public:
	explicit Variable( uint type, uint index ) : type( type ), index( index ) {}
	explicit Variable( text::attributes::SpeechPart sp, uint index ) : type( sp.id ), index( index ) {}
	explicit Variable( const Pattern & pt, uint index );
	explicit Variable() : type( 0 ), index( 0 ) {}

	bool isSpeechPart() { return type < text::attributes::SpeechPart::COUNT; }
	bool isPattern() { return type >= text::attributes::SpeechPart::COUNT; }

	bool operator == ( const Variable & p ) const {
		return ( type == p.type ) && ( index == p.index );
	}

	bool operator != ( const Variable & p ) const {
		return ( type != p.type ) || ( index != p.index );
	}

	bool operator < ( const Variable & p ) const {
		return ( type < p.type ) || ( ( type == p.type ) && ( index < p.index ) );
	}
public:
	uint type;
	uint index;
};

inline std::ostream & operator << ( std::ostream & out, const Variable var ) {
	out << var.type << ":" << var.index;

	return out;
}

} } } // namespace lspl::patterns::matchers

#endif /*_LSPL_MATCHERS_VARIABLE_H_*/
