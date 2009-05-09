#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <boost/spirit/core.hpp>

#include "lspl/terms/Parser.h"

using lspl::uint;
using namespace lspl::terms;

void assertParses( const char * str ) {
	std::cout << "Testing '" << str << "', must be parsed... ";
	std::cout.flush();

	Parser p;
	std::auto_ptr<ParseInfo> pi = p.parse( str );

	if ( !pi->hit ) {
		std::cerr << std::endl;
		std::cerr.flush();

		throw std::logic_error( "Error! String must be parsed" );
	} else {
		std::cout << "Ok" << std::endl;
		std::cout << pi->term->buildPattern() << std::endl;
		std::cout.flush();
	}
}

void assertParses( const char * str, const std::string & result ) {
	std::cout << "Testing '" << str << "', must be converted to '" << result << "'... ";
	std::cout.flush();

	Parser p;
	std::auto_ptr<ParseInfo> pi = p.parse( str );

	if ( !pi->hit ) {
		std::cerr << "Not parsed:" << pi->stop;
		std::cerr.flush();

		throw std::logic_error( "Error! String must be parsed" );
	}

	std::string res = pi->term->buildPattern();

	if ( result != res ) {
		std::cerr << std::endl << "Result: '" << res << "'" << std::endl;
		std::cerr.flush();

		throw std::logic_error( "Error! Wrong string got" );
	} else {
		std::cout << "Ok" << std::endl;
		std::cout.flush();
	}
}

void assertNotParses( const char * str ) {
	std::cout << "Testing '" << str << "', should not be parsed... ";
	std::cout.flush();

	Parser p;
	std::auto_ptr<ParseInfo> pi = p.parse( str );

	if ( pi->hit ) {
		std::cerr << std::endl;
		std::cerr.flush();

		throw std::logic_error( "Error! String should not be parsed" );
	} else {
		std::cout << "Ok" << std::endl;
		std::cout.flush();
	}
}

void checkMemoryLeaks() {
	if ( Term::aliveCount ) {
		std::cerr << "Error: there are " << Term::aliveCount << " alive terms!"<< std::endl;
		throw std::logic_error( "Error! Memory leak detected!" );
	}

	std::cout << "Ok, no memory leaks" << std::endl;
}

void runTests() {
	std::cout << "Testing LSPL generator..." << std::endl;

	assertParses( "[ абонент|19 сети ]", "N1<абонент> \"сети\"" );
	assertParses( "[ абсолютн|ая адресаци|я ]", "A1<абсолютный> N2<адресация> <A1=N2>" );
	assertParses( "[ абсолютн|ая# адресаци|я ]", "A1<абсолютный> N2<адресация> <A1=N2>" );
	assertParses( "[ абсолютн|ая# адресаци|я позици|я]", "A1<абсолютный> { N2<адресация> <A1=N2> | N2<позиция> <A1=N2> }<1,1>" );

	assertParses( "[ абстрактн|ый75/1# {последовательн|ый75/1 файл|1} {тип|1 данных} файл|1 ]", "A1<абстрактный> { A2<последовательный> N3<файл> <A1=N3> <A2=N3> | N2<тип> \"данных\" <A1=N2> | N2<файл> <A1=N2> }<1,1>" );

	assertParses( "[ язык# ассемблера ]", "\"язык\" [ \"ассемблера\" ]" );
	assertParses( "[ язык ассемблера взаимодействия ]", "\"язык\" { \"ассемблера\" | \"взаимодействия\" }<1,1>" );
	assertParses( "[ язык# {высокого уровня} ]", "\"язык\" [ \"высокого\" \"уровня\" ]" );

	assertParses( "[ язык [определения данных] ]" );

	assertParses( "[ язык|# ассемблера программирования]", "N1<язык> [ \"ассемблера\" | \"программирования\" ]" );
	assertParses( "[ язык| ассемблера программирования]", "N1<язык> { \"ассемблера\" | \"программирования\" }<1,1>" );

	assertParses( "[ язык|4 ассемблера ]" );
	assertParses( "[яче|йка45 памяти]" );
	assertParses( "[яче|йка4/5 памяти]" );
	assertParses( "[\"яче|йка45\" памяти]" );
	assertParses( "[грамматик|а {ван Вейгардена}]" );

	assertParses( "[прерывани|е70# ввода-вывода]" );

	checkMemoryLeaks();

	assertNotParses( "[ мама мыла [ раму]" );
	assertNotParses( "[ мама мыла" );
	assertNotParses( "мама [мыла]" );
	assertNotParses( "мама ]" );

	checkMemoryLeaks();

	std::cout << "All tests passed succesfully!" << std::endl;
}

void printHelp() {
	std::cout << "Usage: lspl-gen [options]" << std::endl
		<< "Options:" << std::endl
		<< "  -h - show this help message" << std::endl
		<< "  -t - run tests" << std::endl
		<< "  -i <file> - specify input file (in CP1251 encoding)" << std::endl
		<< "  -o <file> - specify output file (in CP1251 encoding)" << std::endl
		<< "  -e <file> - specify error output file (in CP1251 encoding)" << std::endl;
}

class PatternSetBuilder {
public:
	void append( TermPtr & term );

	void flush( std::ostream & out, std::ostream & err );

private:
	typedef std::map<std::string,boost::shared_ptr<Group> > GroupMap;

private:
	void mergeGroup( const boost::shared_ptr<Group> & mg, const boost::shared_ptr<Group> & g ) const;
	void addGroup( GroupMap & m, const std::string & key, const boost::shared_ptr<Group> & g ) const;

private:
	GroupMap wordGroups;
	GroupMap tokenGroups;
	std::vector<TermPtr> others;
};

void PatternSetBuilder::append( TermPtr & term ) {

	if ( boost::shared_ptr<Group> g = boost::shared_dynamic_cast<Group>( term ) ) {
		if ( boost::shared_ptr<Word> w = boost::shared_dynamic_cast<Word>( g->getFirst() ) ) {
			addGroup( wordGroups, w->getBase(), g );
		} else if ( boost::shared_ptr<Token> t = boost::shared_dynamic_cast<Token>( g->getFirst() ) ) {
			addGroup( wordGroups, t->getToken(), g );
		} else {
			others.push_back( term );
		}

		return;
	}

	others.push_back( term );
}

void PatternSetBuilder::addGroup( GroupMap & m, const std::string & key, const boost::shared_ptr<Group> & g ) const {
	GroupMap::const_iterator it = m.find( key );

	if ( it == m.end() ) { // Если группа, которая начинается с того же слова нет в списке, то просто кладем туда группу
		m.insert( std::make_pair( key ,g ) );
	} else {
		mergeGroup( it->second, g );
	}
}

void PatternSetBuilder::mergeGroup( const boost::shared_ptr<Group> & mg, const boost::shared_ptr<Group> & g ) const {
	const TermList & children = g->getChildren();

	for ( uint i = 0, sz = children.size(); i < sz; ++ i ) {
		mg->append( children[i] );
	}
}

void PatternSetBuilder::flush( std::ostream & out, std::ostream & err ) {
	for ( GroupMap::const_iterator it = wordGroups.begin(); it != wordGroups.end(); ++ it ) {
		out << it->first << " = " << it->second->buildPattern() << std::endl;
	}

	for ( GroupMap::const_iterator it = tokenGroups.begin(); it != tokenGroups.end(); ++ it ) {
		out << it->first << " = " << it->second->buildPattern() << std::endl;
	}

	for ( uint i = 0; i < others.size(); ++ i ) {
		err << "Non-group term: " << others[i]->buildPattern() << std::endl;
	}
}

void convertFile( std::istream * in, std::ostream * out, std::ostream * err ) {
	std::string s;
	char buffer[1000];
	while ( !in->eof() ) {
		in->read( buffer, 1000 );
		s.append( buffer, in->gcount() );
	}

	Parser p;
	PatternSetBuilder b;
	int i;

	const char * start = s.c_str();
	const char * cur = start;
	int len = s.length();

	while ( cur < start + len ) {
		std::auto_ptr<ParseInfo> pi = p.parse( cur );

		if ( pi->hit ) {
			b.append( pi->term );

			cur = boost::spirit::parse( pi->stop, *boost::spirit::space_p ).stop;
		} else {
			char const * reset = cur;

			while ( true ) {

				if ( ( reset - start + 1 ) >= len || ( *reset == '\n' && *(reset+1) == '[' ) )
					break;

				reset ++;
			}

			*err << "Some data not parsed: " << std::string( cur, reset + 1 );

			cur = reset + 1;
		}
	}

	b.flush( *out, *err );
}

int main(int argc, char **argv) {
	opterr = 0;

	std::istream * in = &std::cin;
	std::ostream * out = &std::cout;
	std::ostream * err = &std::cerr;

	char c;
	while ((c = getopt(argc, argv, "hti:o:e:")) != -1) {
		switch (c) {
		case 'i':
			in = new std::ifstream(optarg);
			break;
		case 'o':
			out = new std::ofstream(optarg);
			break;
		case 'e':
			err = new std::ofstream(optarg);
			break;
		case 't':
			runTests();
			return 0;
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

	convertFile( in, out, err );

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

