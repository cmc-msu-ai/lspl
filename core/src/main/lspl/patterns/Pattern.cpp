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
