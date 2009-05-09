/*
 * Parser.cpp
 *
 *  Created on: Dec 24, 2008
 *      Author: alno
 */

#include "Parser.h"

#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/utility/functor_parser.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/spirit/utility/distinct.hpp>
#include <boost/spirit/dynamic/switch.hpp>
#include <boost/spirit/phoenix/binders.hpp>
#include <boost/spirit/phoenix/functions.hpp>

#include <boost/ptr_container/ptr_vector.hpp>

#include <boost/shared_ptr.hpp>

#include <lspl/utils/RusConsts.h>

using namespace boost::spirit;
using namespace phoenix;

namespace lspl { namespace terms {

struct TermClosure : public boost::spirit::closure< TermClosure, boost::shared_ptr<Term> > {
	member1 term;
};

struct SequenceClosure : public boost::spirit::closure< SequenceClosure, boost::shared_ptr<Sequence> > {
	member1 term;
};

struct GroupClosure : public boost::spirit::closure< GroupClosure, boost::shared_ptr<Group>, boost::shared_ptr<Term> > {
	member1 group;
	member2 first;
};

struct WordClosure : public boost::spirit::closure< WordClosure, boost::shared_ptr<Term>, bool, std::string > {
	member1 term;
	member2 fixed;
	member3 token;
};

struct CreateGroupImpl {

	template <typename Arg1, typename Arg2>
	struct result { typedef boost::shared_ptr<Group> type; };

	boost::shared_ptr<Group> operator()( TermPtr first, bool oc ) const {
		return boost::shared_ptr<Group>( new Group( first, oc ) );
	}
};

struct CreateSequenceImpl {

	template <typename Arg1>
	struct result { typedef boost::shared_ptr<Sequence> type; };

	boost::shared_ptr<Sequence> operator()(uint size) const {
		return boost::shared_ptr<Sequence>( new Sequence() );
	}
};

struct CreateWordImpl {

	template <typename Arg1, typename Arg2>
	struct result { typedef boost::shared_ptr<Term> type; };

	TermPtr operator()( const std::string token, bool fixed ) const {
		if ( fixed )
			return TermPtr( new Token( token ) );
		else
			return TermPtr( new Word( token ) );
	}
};

struct AppendImpl {

	template <typename Arg1, typename Arg2>
	struct result { typedef void type; };

	template <typename Arg1, typename Arg2>
	void operator()( boost::shared_ptr<Arg1> & cont, boost::shared_ptr<Arg2> arg ) const {
		cont->append( arg );
	}
};

class Parser::Impl: public grammar<Parser::Impl,TermClosure::context_t> {
public:

	template<typename ScannerT> class definition {
	public:

		definition( const Parser::Impl & self ) {
			function<CreateGroupImpl> createGroup;
			function<CreateWordImpl> createWord;
			function<CreateSequenceImpl> createSequence;
			function<AppendImpl> append;

			top = wordGroup[self.term = arg1];

			wordGroup =
				'[' >> word[ wordGroup.first = arg1 ] >>
				( ch_p('#')[ wordGroup.group = createGroup( wordGroup.first, true ) ] | epsilon_p[ wordGroup.group = createGroup( wordGroup.first, false ) ] ) >>
				* wordGroupElement[ append( wordGroup.group, arg1 ) ] >>
				']';

			wordGroupElement =
				word[ wordGroupElement.term = arg1 ] | wordGroup[ wordGroupElement.term = arg1 ] | wordSequence[ wordGroupElement.term = arg1 ];

			wordSequence =
				( '{' >>
				wordGroupElement[ wordSequence.term = createSequence( 0 ) ][ append( wordSequence.term, arg1 ) ] >>
				*wordGroupElement[ append( wordSequence.term, arg1 ) ] >>
				'}' )
			| ( '"' >>
				wordGroupElement[ wordSequence.term = createSequence( 0 ) ][ append( wordSequence.term, arg1 ) ] >>
				*wordGroupElement[ append( wordSequence.term, arg1 ) ] >>
				'"' ); // Последовательность слов

			word = lexeme_d[ epsilon_p[ word.fixed = true ] >> +(
					( chset_p("a-zA-Z" RUS_ALPHA "-") )[ word.token += arg1 ] |
					( ch_p('|')[ word.fixed = false ] | chset_p("0-9/,.") )
				) ][ word.term = createWord( word.token, word.fixed ) ]
				| ( '"' >> word[ word.term = arg1 ] >> '"' );

		}

		rule<ScannerT> const & start() const {
			return top;
		}

	private:
		rule<ScannerT> top;
		rule<ScannerT,GroupClosure::context_t> wordGroup;
		rule<ScannerT,SequenceClosure::context_t> wordSequence;
		rule<ScannerT,TermClosure::context_t> wordGroupElement;
		rule<ScannerT,WordClosure::context_t> word;
	};
};

Parser::Parser() : impl( new Impl() ) {
}

Parser::~Parser() {
}

std::auto_ptr<ParseInfo> Parser::parse( const char * str ) {
	boost::shared_ptr<Term> term;
	parse_info<> bpi = boost::spirit::parse( str, (*impl)[ var(term) = arg1 ], space_p );

	std::auto_ptr<ParseInfo> pi( new ParseInfo() );
	pi->term = term;
	pi->stop = bpi.stop;
	pi->hit = bpi.hit;

	return pi;
}

} } // namespace lspl::terms
