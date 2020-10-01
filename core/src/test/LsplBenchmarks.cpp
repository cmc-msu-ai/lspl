#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <boost/timer/timer.hpp>

#include <lspl/Namespace.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/PatternBuilder.h>
#include <lspl/text/Text.h>
#include <lspl/text/readers/PlainTextReader.h>
#include <lspl/morphology/Morphology.h>

using namespace lspl;
using lspl::uint;
using lspl::morphology::Morphology;
using boost::timer::cpu_timer;

const uint DEFAULT_TIMER_PLACES = 3;

text::TextRef loadTextFromFile( const char * fileName ) {
	std::ifstream textStream( fileName );
	text::readers::PlainTextReader reader;

	std::cout << "Loading text from file " << fileName << "... ";
	std::cout.flush();

	cpu_timer tm;
	text::TextRef text = reader.readFromStream( textStream );
	std::cout << tm.format(DEFAULT_TIMER_PLACES, "Done in %t seconds.") << std::endl;

	return text;
}

void definePatterns( NamespaceRef ns ) {
	patterns::PatternBuilderRef builder = new patterns::PatternBuilder( ns );

	std::cout << "Building patterns... ";
	std::cout.flush();

	cpu_timer tm;

	builder->build( "Pact = N V" );              // 5582  [v]
	builder->build( "Act = N V <<N=V>>" );       // 4505  [v]
	builder->build( "AAA (A, Ap) = A | Ap");     // 27333 [v]
	builder->build( "ABB = {A} N <<A=N>>" );     // 48508 [v]
	builder->build( "ACC = {AAA} N <<AAA=N>>" ); // 49146 [v]
	builder->build( "ADD = \"à\" Act" );         // 55    [v]
	builder->build( "AEE = N \"è\" N" );         // 446   [v]
	builder->build( "ANom = N<c=nom> V" );       // 4199  [v]
	builder->build( "AGen = N<c=gen> V" );       // 1373  [v]
	std::cout << tm.format(DEFAULT_TIMER_PLACES, "Done in %t seconds.") << std::endl;
}

void findPatterns() {
	NamespaceRef ns = new Namespace();
	text::TextRef text = loadTextFromFile( "resources/test/stranger.txt" );

	definePatterns( ns );

	for ( uint i = 0; i < ns->getPatternCount(); ++ i ) {
		patterns::PatternRef pt = ns->getPatternByIndex( i );

		std::cout << "Matching " << pt->getSource() << "... ";
		std::cout.flush();

		cpu_timer tm;
		uint count = text->getMatches( *pt ).size();
		std::cout << tm.format(DEFAULT_TIMER_PLACES, "Done in %t seconds, ") << count << " matches found" << std::endl;
	}

	std::cout << text->getWords( text::attributes::SpeechPart::NOUN ).size() << std::endl;
}

void loadMorphology() {
	std::cout << "Loading morphology system... ";
	std::cout.flush();

	cpu_timer tm;
	Morphology::instance();
	std::cout << tm.format(DEFAULT_TIMER_PLACES, "Done in %t seconds.") << std::endl;
}

int main() {
	std::cout << "Testing matching performance..." << std::endl;

	loadMorphology();

	try {
		findPatterns();
	} catch (patterns::PatternBuildingException &e) {
		std::cerr << "Pattern building exception" << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << e.input << std::endl;
	}

	std::cout << "Exiting..." << std::endl;

	return 0;
}
