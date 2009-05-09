/*
 * AotMorphology.h
 *
 *  Created on: Dec 24, 2008
 *      Author: alno
 */

#ifndef _LSPL_MORPHOLOGY_AOTMORPHOLOGY_H_
#define _LSPL_MORPHOLOGY_AOTMORPHOLOGY_H_

#include "Morphology.h"

class CLemmatizer;
class CAgramtab;

namespace lspl { namespace morphology {

class LSPL_EXPORT AotMorphology : public Morphology {
public:
	AotMorphology();
	~AotMorphology();

	const char * findRml();

	void setupRml();
	void appendWordForms( std::string token, WordFormList & forms );

	text::attributes::SpeechPart getSpeechPart( const char * gramCode );

	uint64 getAttributes( const char * gramCode );

	std::string getAttributesString( uint64 attValues );

	std::string upcase( const char * str );
	std::string lowcase( const char * str );

private:
	CLemmatizer * lemmatizer;
	CAgramtab * agramtab;
};

} } // namespace lspl::morphology

#endif /* _LSPL_MORPHOLOGY_AOTMORPHOLOGY_H_ */
