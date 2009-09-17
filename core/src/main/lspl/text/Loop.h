#ifndef LOOPTRANSITION_H_
#define LOOPTRANSITION_H_

#include "../base/Base.h"

#include "Transition.h"

namespace lspl { namespace text {

class LSPL_EXPORT Loop : public lspl::text::Transition
{
public:
	Loop( const text::Node & start, const text::Node & end, const TransitionList & transitions );
	virtual ~Loop();

	virtual void dump( std::ostream & out, std::string tabs = "" ) const;

	const TransitionList & getTransitions() const {
		return transitions;
	}

public:
	const TransitionList transitions;
};

} }

#endif /*LOOPTRANSITION_H_*/
