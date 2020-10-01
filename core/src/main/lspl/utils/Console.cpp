#include "../base/BaseInternal.h"

#include "Console.h"

#include "../patterns/Pattern.h"
#include "../patterns/PatternBuilder.h"
#include "../patterns/matchers/Matcher.h"

#include "../text/Text.h"
#include "../text/readers/PlainTextReader.h"
#include "../text/readers/JsonTextReader.h"
#include "../text/writers/JsonTextWriter.h"

#include <sstream>
#include <fstream>

namespace lspl { namespace utils {

const std::string Console::INTERNAL_ENCODING = "CP1251";

Console::Console( std::istream & input, std::ostream & output ) :
	input( input ), output( output ) {

	setEncoding( INTERNAL_ENCODING );
}

Console::Console( std::istream & input, std::ostream & output, const std::string & encoding ) :
	input( input ), output( output ) {

	setEncoding( encoding );
}

Console::~Console()
{
}

void Console::setEncoding( const std::string & enc ) {
	encoding = enc;

	if ( encoding == INTERNAL_ENCODING ) {
		inputConversion = Conversion();
		outputConversion = Conversion();
		conversionNeeded = false;
	} else {
		inputConversion = Conversion( encoding, INTERNAL_ENCODING );
		outputConversion = Conversion( INTERNAL_ENCODING, encoding );
		conversionNeeded = true;
	}
}

void Console::run() {
	lspl::text::TextRef text;
	patterns::PatternBuilder patternBuilder;

	while ( !input.eof() ) {
		std::string command, args;
		char args_buf[1000];

		output << "lspl> ";

		input >> command;
		input.getline( args_buf, 1000, '\n' );

		args = inputConversion.convert( args_buf );

		if ( command == "exit" ) {
			return;
		} else if ( command == "define" ) {
			std::ostringstream out;
			bool success = true;

			patterns::PatternBuilder::BuildInfo bi;
			try {
				 bi = patternBuilder.build( args );
			} catch (patterns::PatternBuildingException &e) {
				success = false;
				out << "Exception happened during parsing: " << std::endl;
				out << "  " << e.what() << std::endl;
				out << "Context:" << std::endl;
				out << "  " << e.input << std::endl << "  ";
				for (uint i = 0; i < e.errorPos; ++i)
					out << ' ';
				out << '^' << std::endl;
			}

			if (success)
				out << "Ok, parsed " << bi.parseLength << " characters";

			output << outputConversion.convert( out.str() ) << std::endl;
		} else if ( command == "dump" ){
			uint count = patternBuilder.space->getPatternCount();
			for ( uint i = 0; i < count; ++i ) {
				std::ostringstream out;
				const patterns::Pattern & p = *patternBuilder.space->getPatternByIndex( i );

				p.dump( out );
				output << outputConversion.convert( out.str() ) << std::endl;
			}
		} else if ( command == "analyze" ) {
			lspl::text::readers::PlainTextReader reader;
			text = reader.readFromString( args );
			std::ostringstream out;

			output << outputConversion.convert( text->getContent() ) << std::endl;

			uint count = patternBuilder.space->getPatternCount();
			for ( uint i = 0; i < count; ++i )
				text->getMatches( *patternBuilder.space->getPatternByIndex( i ) );

			text->dump( out );

			output << outputConversion.convert( out.str() ) << std::endl;
		} else if ( command == "savejs" ) {
			lspl::text::writers::JsonTextWriter writer;
			std::ofstream os( args.c_str() );
			writer.writeToStream( *text, os );
			os.close();
		} else if ( command == "loadjs" ) {
			lspl::text::readers::JsonTextReader reader( patternBuilder.space );
			std::ifstream is( args.c_str() );
			text = reader.readFromStream( is );
			is.close();
			std::ostringstream out;

			output << outputConversion.convert( text->getContent() ) << std::endl;

			uint count = patternBuilder.space->getPatternCount();
			for ( uint i = 0; i < count; ++i )
				text->getMatches( *patternBuilder.space->getPatternByIndex( i ) );

			text->dump( out );

			output << outputConversion.convert( out.str() ) << std::endl;
		} else {
			output << outputConversion.convert( "Undefined command \"" + command + "\"" ) << std::endl;
		}
	}
}

} }
