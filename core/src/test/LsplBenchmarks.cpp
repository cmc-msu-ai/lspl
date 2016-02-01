#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <boost/timer.hpp>

#include <lspl/Namespace.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/PatternBuilder.h>
#include <lspl/text/Text.h>
#include <lspl/text/readers/PlainTextReader.h>
#include <lspl/morphology/Morphology.h>

using namespace lspl;
using lspl::uint;
using lspl::morphology::Morphology;

text::TextRef loadTextFromFile( const char * fileName ) {
	std::ifstream textStream( fileName );
	text::readers::PlainTextReader reader;

	std::cout << "Loading text from file " << fileName << "... ";
	std::cout.flush();

	boost::timer tm;
	text::TextRef text = reader.readFromStream( textStream );
	std::cout << "Done in " << tm.elapsed() << " seconds" << std::endl;

	return text;
}

void definePatterns( NamespaceRef ns ) {
	patterns::PatternBuilderRef builder = new patterns::PatternBuilder( ns );

	std::cout << "Building patterns... ";
	std::cout.flush();

	boost::timer tm;
	builder->build( "Pact = N V" );
	builder->build( "Act = N V <N=V>" );
	builder->build( "AAA = A (A) | Ap (Ap)" );
	builder->build( "ABB = {A} N <A=N>" );
	builder->build( "ACC = {AAA} N <AAA=N>" );
	builder->build( "ADD = \"à\" Act" );
	builder->build( "AEE = N \"è\" N" );
	builder->build( "ANom = N<c=nom> V" );
	builder->build( "AGen = N<c=gen> V" );

	std::cout << "Done in " << tm.elapsed() << " seconds." << std::endl;
}

void findPatterns() {
	NamespaceRef ns = new Namespace();
	text::TextRef text = loadTextFromFile( "resources/test/stranger.txt" );

	definePatterns( ns );

	for ( uint i = 0; i < ns->getPatternCount(); ++ i ) {
		patterns::PatternRef pt = ns->getPatternByIndex( i );

		std::cout << "Matching " << pt->getSource() << "... ";
		std::cout.flush();

		boost::timer tm;
		uint count = text->getMatches( *pt ).size();
		std::cout << "Done in " << tm.elapsed() << " seconds, " << count << " matches found"<< std::endl;
	}

	std::cout << text->getWords( text::attributes::SpeechPart::NOUN ).size() << std::endl;
}

void loadMorphology() {
	std::cout << "Loading morphology system... ";
	std::cout.flush();

	boost::timer tm;
	Morphology::instance();
	std::cout << "Done in " << tm.elapsed() << " seconds." << std::endl;
}

int main() {
	std::cout << "Testing matching performance..." << std::endl;

	loadMorphology();
	findPatterns();

	std::cout << "Exiting..." << std::endl;

	return 0;
}
