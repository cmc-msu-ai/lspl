/*
 * Term.h
 *
 *  Created on: Dec 24, 2008
 *      Author: alno
 */

#ifndef _LSPL_TERMS_TERM_H_
#define _LSPL_TERMS_TERM_H_

#include <lspl/text/attributes/SpeechPart.h>

#include <ostream>
#include <string>
#include <vector>
#include <utility>
#include <complex>

#include <boost/shared_ptr.hpp>

namespace lspl { namespace terms {

class Term {
public:
	Term();
	virtual ~Term();

	std::string buildPattern() const;
	void appendPatternTo( std::ostream & out ) const;

	virtual void appendPatternTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx ) const = 0;
	virtual bool needChildren() const = 0;

protected:

	void appendRestrictionsTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx, uint begin, uint end ) const;

public:
	static uint aliveCount;
};

typedef boost::shared_ptr<Term> TermPtr;
typedef std::vector<TermPtr> TermList;

class Word : public Term {
public:
	Word( const std::string & token );

	void appendPatternTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx ) const;
	virtual bool needChildren() const;

	lspl::text::attributes::SpeechPart getSpeechPart() const {
		return speechPart;
	}

	std::string getBase() const {
		return base;
	}

private:
	lspl::text::attributes::SpeechPart speechPart;

	std::string base;
};

class Token : public Term {
public:
	Token( const std::string & token );

	void appendPatternTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx ) const;
	virtual bool needChildren() const;

	const std::string & getToken() const {
		return token;
	}

private:
	std::string token;
};

class Group : public Term {
public:
	Group( TermPtr first, bool optionalChildren );

	void appendPatternTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx ) const;
	virtual bool needChildren() const;

	void append( TermPtr t ) {
		children.push_back( t );
	}

	const TermPtr & getFirst() const {
		return first;
	}

	const TermList & getChildren() const {
		return children;
	}

private:
	void appendChildrenTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx ) const;
	void appendChildTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx, const TermPtr & child ) const;
private:
	bool optionalChildren;

	TermPtr first;
	TermList children;
};

class Sequence : public Term {
public:
	Sequence();

	void appendPatternTo( std::ostream & out, std::vector<std::pair<lspl::text::attributes::SpeechPart,uint> > & ctx ) const;
	virtual bool needChildren() const;

	void append( TermPtr t ) {
		children.push_back( t );
	}

private:
	TermList children;
};

} } // namespace lspl::terms

#endif /* _LSPL_TERMS_TERM_H_ */
