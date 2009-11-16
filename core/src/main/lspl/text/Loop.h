#ifndef LOOPTRANSITION_H_
#define LOOPTRANSITION_H_

#include "../base/Base.h"

#include "Transition.h"

#include "../patterns/matchers/MatcherContainer.h"

namespace lspl { namespace text {

class LSPL_EXPORT LoopIterationVariant : public TransitionList {
public:
	LoopIterationVariant( const patterns::matchers::MatcherContainer & alternative );
	LoopIterationVariant( const LoopIterationVariant & variant );
	~LoopIterationVariant();
public:
	const patterns::matchers::MatcherContainer & alternative;
	int index;
};

class LSPL_EXPORT LoopIteration : public base::RefCountObject<LoopIteration> {
public:

	LoopIteration( const text::Node & endNode ) : end( endNode ) {}
	LoopIteration( const text::Node & endNode, LoopIterationVariant * v ) : end( endNode ) {
		variants.push_back( v );
	}

	const LoopIterationVariant & getVariant( uint index ) const {
		return variants[ index ];
	}

public:

	boost::ptr_vector<LoopIterationVariant> variants;

	const text::Node & end;

};

class LSPL_EXPORT Loop : public lspl::text::Transition
{
public:
	Loop( const text::Node & start, const text::Node & end, uint repeatCount );
	virtual ~Loop();

	virtual void dump( std::ostream & out, std::string tabs = "" ) const;

	const LoopIterationList & getIterations() const {
		return iterations;
	}

public:
	LoopIterationList iterations;

	const uint repeatCount;
};

} }

#endif /*LOOPTRANSITION_H_*/
