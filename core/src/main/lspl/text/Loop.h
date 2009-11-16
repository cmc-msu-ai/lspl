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

class LSPL_EXPORT LoopIteration : public lspl::text::Transition {
public:

	LoopIteration( const text::Node & start, const text::Node & end ) : Transition( Transition::ITERATION, start, end ) {}
	LoopIteration( const text::Node & start, const text::Node & end, LoopIterationVariant * v ) : Transition( Transition::ITERATION, start, end ) {
		variants.push_back( v );
	}

	const LoopIterationVariant & getVariant( uint index ) const {
		return variants[ index ];
	}

	uint getVariantCount() const {
		return variants.size();
	}

	void addVariant( LoopIterationVariant * variant ) {
		variant->index = variants.size();

		variants.push_back( variant );
	}

	virtual void dump( std::ostream & out, std::string tabs = "" ) const;

private:

	boost::ptr_vector<LoopIterationVariant> variants;

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

	const LoopIterationRef & getIteration( uint index ) const {
		return iterations[ index ];
	}

	uint getIterationCount() const {
		return iterations.size();
	}

public:
	LoopIterationList iterations;

	const uint repeatCount;
};

} }

#endif /*LOOPTRANSITION_H_*/
