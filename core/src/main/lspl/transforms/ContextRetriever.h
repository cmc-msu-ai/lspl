/*
 * ContextRetriever.h
 *
 *  Created on: Dec 6, 2009
 *      Author: alno
 */

#ifndef _LSPL_TRANSFORMS_CONTEXTRETRIEVER_H_
#define _LSPL_TRANSFORMS_CONTEXTRETRIEVER_H_

#include "Transform.h"
#include "../patterns/matchers/Forward.h"

namespace lspl { namespace transforms {

class LSPL_EXPORT ContextRetriever : public TypedTransform<patterns::matchers::Context> {
public:
	ContextRetriever();
	virtual ~ContextRetriever();

	virtual patterns::matchers::Context apply( const text::MatchVariant & matchVariant ) const;

private:

	void appendToContext( patterns::matchers::Context & ctx, const text::Transition & transition, const patterns::matchers::Matcher & matcher ) const;
	void appendToContext( patterns::matchers::Context & ctx, const text::TransitionList & transitions, const patterns::matchers::MatcherContainer & matchers ) const;

};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_CONTEXTRETRIEVER_H_ */
