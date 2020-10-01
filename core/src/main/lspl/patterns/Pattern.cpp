#include "../base/BaseInternal.h"

#include "Pattern.h"
#include "Alternative.h"
#include "matchers/Matcher.h"

#include "../text/Text.h"

#include <stdexcept>

LSPL_REFCOUNT_CLASS( lspl::patterns::Pattern );

namespace lspl { namespace patterns {

uint Pattern::aliveObjectsCount = 0;

Pattern::Pattern( const std::string & name ) : name( name ) { ++aliveObjectsCount; }
Pattern::~Pattern() { --aliveObjectsCount; }

void Pattern::dump( std::ostream & out, const std::string & tabs ) const {
	out << "Pattern{ name = " << name << ", alternatives = [\n\t" << tabs;

	bool first = true;
	for( const std::unique_ptr<Alternative> &alt : alternatives ) {
		if ( first ) {
			first = false;
		} else {
			out << ",\n\t" << tabs;
		}

		alt->dump( out, tabs + "\t" );
	}

	out << "\n" << tabs << "] }";
}

Alternative & Pattern::newAlternative( const std::string & source )  {
	Alternative * alt = new patterns::Alternative( source );

	addAlternative( alt );

	return *alt;
};

void Pattern::addAlternative( Alternative * alt ) {
	alt->pattern = this;
	alternatives.emplace_back( alt );
}

void Pattern::addAlternatives( std::vector<std::unique_ptr<Alternative>> & r ) {
	for( const std::unique_ptr<Alternative> & alt : r ) {
		alt->pattern = this;
	}
	int oldSize = alternatives.size();
	alternatives.resize(oldSize + r.size());
	std::move(r.begin(), r.end(), alternatives.begin() + oldSize);
}

void Pattern::mergePattern ( Pattern &other ) {
	addAlternatives(other.alternatives);
	other.alternatives.clear();
}

void Pattern::updateDependencies() {
	dependencies.clear();

	for( const std::unique_ptr<Alternative> & alt : alternatives ) {

		for( const Pattern * ptr : alt->getDependencies() ) {
			bool found = false;

			for( const Pattern * dep : dependencies ) {
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
			if ( alternatives[i]->equals( *alternatives[j] ) ) {
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

bool Pattern::dependsOn( const Pattern & pattern, bool transitive ) const {
	if ( transitive ) {
		std::vector<const Pattern*> stack;

		stack.push_back( this );

		return deepDependsOn( &pattern, stack );
	} else {
		for( const Pattern * p : dependencies ) {
			if ( p == &pattern ) // Если нашли шаблон в зависимостях
				return true;
		}

		return false;
	}
}

bool Pattern::deepDependsOn( const Pattern * target, std::vector<const Pattern*> & stack ) const {
	for( const Pattern * p : stack )
		if ( p == this )
			return false;

	stack.push_back( this ); // Помещаем текущий шаблон в стек

	for( const Pattern * p : dependencies ) {
		if ( p == target ) // Если нашли шаблон в зависимостях
			return true;

		if ( p->deepDependsOn( target, stack ) ) // Если есть транзитивная зависимость
			return true;
	}

	stack.pop_back(); // Удаляем текущий шаблон из стека

	return false;
}

std::string Pattern::getSource() const {
	std::string result = "";

	bool first = true;
	for( const std::unique_ptr<Alternative> &alt : alternatives ) {
		if ( first ) {
			first = false;
		} else {
			result += " | ";
		}

		result += alt->getSource();
	}

	return result;
}

} }
