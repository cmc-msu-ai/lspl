/*
 * Normalization.h
 *
 *  Created on: Sep 28, 2009
 *      Author: alno
 */

#ifndef _LSPL_TRANSFORMS_NORMALIZATION_H_
#define _LSPL_TRANSFORMS_NORMALIZATION_H_

#include "Transform.h"

#include <string>

namespace lspl { namespace transforms {

class LSPL_EXPORT Normalization : public TypedTransform<std::string> {
public:
	Normalization();
	virtual ~Normalization();

	virtual std::string apply( const text::MatchVariant & matchVariant ) const;
	virtual std::string normalize( const text::MatchVariant & matchVariant ) const;
private:

	void appendToString( std::string & str, const text::Transition & transition ) const;
	void appendToString( std::string & str, const text::TransitionList & transitions ) const;
	void appendToString( std::string & str, const text::LoopIterationList & iterations ) const;

	void normalizeToString( std::string & str, const text::Transition & transition ) const;
	void normalizeToString( std::string & str, const text::TransitionList & transitions ) const;
	void normalizeToString( std::string & str, const text::LoopIterationList & iterations ) const;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_NORMALIZATION_H_ */
