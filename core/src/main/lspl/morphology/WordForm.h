/*
 * WordForm.h
 *
 *  Created on: Dec 24, 2008
 *      Author: alno
 */

#ifndef _LSPL_MORPHOLOGY_WORDFORM_H_
#define _LSPL_MORPHOLOGY_WORDFORM_H_

#include "../base/Base.h"
#include "../base/RefCountObject.h"
#include "../base/RefCountPtr.h"

#include "../text/attributes/SpeechPart.h"
#include "../text/attributes/AttributeContainer.h"

#include <string>
#include <vector>

#include <boost/scoped_array.hpp>

namespace lspl { namespace morphology {

LSPL_REFCOUNT_FORWARD(WordForm);

class LSPL_EXPORT WordForm : public base::RefCountObject<WordForm> {
public:
	WordForm( text::attributes::SpeechPart speechPart, const std::string & base, uint64 * attributeSets, uint attributeSetCount ) :
		speechPart( speechPart ), base( base ), attributeSets( attributeSets ), attributeSetCount( attributeSetCount ) {}

	virtual ~WordForm() {}

	const std::string & getBase() const {
		return base;
	}

	text::attributes::SpeechPart getSpeechPart() const {
		return speechPart;
	}

	text::attributes::AttributeValue getAttribute( uint setIndex, text::attributes::AttributeKey key ) const;

	uint64 getAttributeSet( uint index ) const {
		return attributeSets[ index ];
	}

	uint getAttributeSetCount() const {
		return attributeSetCount;
	}
private:
	text::attributes::SpeechPart speechPart;

	std::string base;

	boost::scoped_array<uint64> attributeSets;
	uint attributeSetCount;
};

} } // namespace lspl::morphology

#endif /* _LSPL_MORPHOLOGY_WORDFORM_H_ */
