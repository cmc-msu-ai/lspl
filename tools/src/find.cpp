/*
 * find.cpp
 *
 *  Created on: Dec 22, 2008
 *      Author: alno
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lspl/Namespace.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/PatternBuilder.h>

#include <lspl/text/Text.h>
#include <lspl/text/readers/PlainTextReader.h>

using lspl::uint;

void printHelp() {
	std::cout << "Usage: lspl-gen [options] patterns" << std::endl
		<< "Options:" << std::endl
		<< "  -h - show this help message" << std::endl
		<< "  -p <file> - specify patterns file (in CP1251 encoding)" << std::endl
		<< "  -i <file> - specify input file (in CP1251 encoding)" << std::endl
		<< "  -o <file> - specify output file (in CP1251 encoding)" << std::endl
		<< "  -e <file> - specify error output file (in CP1251 encoding)" << std::endl;
}

void buildPatterns( const lspl::patterns::PatternBuilderRef & builder, std::istream & in, std::ostream & err ) {
	char buffer[1000];
	while (!in.eof()) {
		in.getline( buffer, sizeof(buffer), '\n' );

		buffer[in.gcount()] = 0;

		try {
			lspl::patterns::PatternBuilder::BuildInfo info = builder->build( buffer );

			if ( info.parseTail.length() > 0 ) {
				err << "Error during parsing '" << buffer << "': '" << info.parseTail << "' not parsed" << std::endl;
			}
		} catch ( std::exception & ex ) {
			err << "Error during parsing '" << buffer << "': " << ex.what() << std::endl;
		} catch ( ... ) {
			err << "Unknown error during parsing '" << buffer << "'" << std::endl;
		}
	}
}

lspl::patterns::PatternList buildGoals( const lspl::patterns::PatternBuilderRef & builder, char ** argv, int start, int end, std::ostream & err ) {
	lspl::patterns::PatternList goals;

	for ( int i = start; i < end; ++i ) {
		std::string source( argv[i] );

		if ( source == "*" ) { // Цель * - найти ВСЕ шаблоны
			for ( uint i = 0, sz = builder->space->getPatternCount(); i < sz; ++i )
				goals.push_back( builder->space->getPatternByIndex( i ) );

			continue;
		}

		try {
			lspl::patterns::PatternRef goal = builder->space->getPatternByName( source );

			if ( !goal ) {
				lspl::patterns::PatternBuilder::BuildInfo info = builder->build( source );

				if ( info.parseTail.length() == 0 ) {
					goal = builder->space->getPatternByName( source );
				} else {
					err << "Error during parsing '" << source << "': '" << info.parseTail << "' not parsed" << std::endl;
				}
			}

			if ( goal )
				goals.push_back( goal );
		} catch ( std::exception & ex ) {
			err << "Error during parsing '" << source << "': " << ex.what() << std::endl;
		} catch ( ... ) {
			err << "Unknown error during parsing '" << source << "'" << std::endl;
		}
	}

	return goals;
}

void processGoal( const lspl::patterns::PatternRef & goal, const lspl::text::TextRef & text, std::ostream & out, std::ostream & err ) {
	lspl::text::MatchList matches = text->getMatches( goal );

	out << "\t\t<goal name=\"" << goal->name << "\">\n";

	for ( uint matchIndex = 0; matchIndex < matches.size(); ++ matchIndex ) {
		lspl::text::MatchRef match = matches[ matchIndex ];

		out << "\t\t\t<match startPos=\"" << match->getRangeStart() << "\" endPos=\"" << match->getRangeEnd() << "\">\n";
		out << "\t\t\t\t<fragment>" << match->getRangeString() << "</fragment>\n";
		out << "\t\t\t</match>\n";
	}

	out << "\t\t</goal>\n";
	out.flush();
}

int main(int argc, char ** argv) {
	opterr = 0;

	std::istream * in = &std::cin;
	std::istream * pin = 0;
	std::ostream * out = &std::cout;
	std::ostream * err = &std::cerr;

	if ( argc <= 1 ) {
		printHelp();
		return 0;
	}

	char c;
	while ((c = getopt(argc, argv, "hi:o:e:p:")) != -1) {
		switch (c) {
		case 'i':
			in = new std::ifstream( optarg );

			if ( in->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as input file" << std::endl;
				return 1;
			}
			break;
		case 'o':
			out = new std::ofstream( optarg );

			if ( out->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as output file" << std::endl;
				return 1;
			}
			break;
		case 'e':
			err = new std::ofstream( optarg );

			if ( err->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as error file" << std::endl;
				return 1;
			}
			break;
		case 'p': {
			pin = new std::ifstream( optarg );

			if ( pin->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as pattern file" << std::endl;
				return 1;
			}

			break;
		}
		case 'h':
		default:
			printHelp();
			return 0;
		}

		if ( opterr ) {
			printHelp();
			return 0;
		}
	}

	lspl::NamespaceRef ns = new lspl::Namespace();
	lspl::patterns::PatternBuilderRef builder = new lspl::patterns::PatternBuilder( ns );
	lspl::text::readers::PlainTextReader r;

	if ( pin ) {
		buildPatterns( builder, *pin, *err );
		delete pin;
		pin = 0;
	}

	lspl::patterns::PatternList goals = buildGoals( builder, argv, optind, argc, *err );
	lspl::text::TextRef text = r.readFromStream( *in );

	*out << "<?xml version=\"1.0\" encoding=\"windows-1251\"?>\n<texts>\n";
	*out << "\t<text>\n";

	for ( uint goalIndex = 0; goalIndex < goals.size(); ++ goalIndex ) {
		processGoal( goals[ goalIndex ], text, *out, *err );
	}

	*out << "\t</text>\n";
	*out << "</texts>\n";

	if ( in != &std::cin ) {
		delete in;
	}

	if ( out != &std::cout ) {
		delete out;
	}

	if ( err != &std::cerr ) {
		delete err;
	}

	return 0;
}
