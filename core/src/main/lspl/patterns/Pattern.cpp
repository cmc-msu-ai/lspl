#include "../base/BaseInternal.h"

#include "Pattern.h"

#include "../text/Text.h"

#include <stdexcept>

LSPL_REFCOUNT_CLASS( lspl::patterns::Pattern );

namespace lspl { namespace patterns {

void Pattern::dump( std::ostream & out, const std::string & tabs ) const {
	out << "Pattern{ name = " << name << ", alternatives = [\n\t" << tabs;

	bool first = true;
	foreach ( const Alternative & alt, alternatives ) {
		if ( first ) {
			first = false;
		} else {
			out << ",\n\t" << tabs;
		}

		alt.dump( out, tabs + "\t" );
	}

	out << "\n" << tabs << "] }";
}

Alternative & Pattern::newAlternative( const std::string & source )  {
	Alternative * alt = new patterns::Alternative( source );

	addAlternative( alt );

	return *alt;
};

void Pattern::updateDependencies() {
	dependencies.clear();

	foreach( const Alternative & alt, alternatives ) {
		foreach( const Pattern * ptr, alt.getDependencies() ) {
			bool found = false;

			foreach( const Pattern * dep, dependencies ) {
				if ( dep == ptr ) {
					found = true;
					false;
				}
			}

			if ( !found ) {
				dependencies.push_back( ptr );
			}
		}
	}
}

void Pattern::removeDuplicateAlternatives() {
	uint i = 0;

	while ( i < alternatives.size() ) {
		bool found = false;
		for ( uint j = 0; j < i; ++ j ) {
			if ( alternatives[i].equals( alternatives[j] ) ) {
				found = true;
				break;
			}
		}

		if ( found ) {
			alternatives.erase( alternatives.begin() + i );
		} else {
			++ i;
		}
	}
}

std::string Pattern::getSource() const {
	std::string result = "";

	bool first = true;
	foreach ( const Alternative & alt, alternatives ) {
		if ( first ) {
			first = false;
		} else {
			result += " | ";
		}

		result += alt.getSource();
	}

	return result;
}

} }
