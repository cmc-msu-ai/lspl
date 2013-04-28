#include "../base/BaseInternal.h"

#include <iostream>

#ifdef WIN32
	#include <windows.h>
	#include <winuser.h>
#endif

#include "AotMorphology.h"

#include "aot/Source/AgramtabLib/rus_consts.h"

using namespace lspl::text::attributes;

namespace lspl { namespace morphology {

Morphology * Morphology::singletonInstance = 0;

Morphology::Morphology() {

}

Morphology::~Morphology() {

}

AttributeValue Morphology::extractAttributeValue( uint64 attributeSet, AttributeKey key ) {
	struct AttributeInfo {
		int bitsCount;

		int bits[10];
		AttributeValue values[10];
	};

	static AttributeInfo attributeInfos[10] = {
		{
			8,
			{ rNominativ, rGenitiv, rDativ,	rAccusativ, rInstrumentalis, rLocativ, rVocativ, rIndeclinable },
			{ AttributeValue::NOMINATIVE, AttributeValue::GENITIVE, AttributeValue::DATIVE, AttributeValue::ACCUSATIVE,	AttributeValue::INSTRUMENTAL, AttributeValue::PREPOSITIONAL, AttributeValue::PREPOSITIONAL, AttributeValue::UNINFLECTED }
		},
		{
			2,
			{ rSingular, rPlural },
			{ AttributeValue::SINGULAR, AttributeValue::PLURAL }
		},
		{
			3,
			{ rMasculinum, rFeminum, rNeutrum },
			{ AttributeValue::MASCULINE, AttributeValue::FEMININE, AttributeValue::NEUTER }
		},
		{
			2,
			{ rComparative, rSuperlative },
			{ AttributeValue::COMPARATIVE, AttributeValue::SUPERLATIVE }
		},
		{
			3,
			{ rPresentTense, rPastTense, rFutureTense },
			{ AttributeValue::PRESENT, AttributeValue::PAST, AttributeValue::FUTURE }
		},
		{
			2,
			{ rAnimative, rNonAnimative },
			{ AttributeValue::ANIMATE, AttributeValue::INANIMATE }
		},
		{ // Форма
			0,
			{},
			{}
		},
		{
			1,
			{ rImperative },
			{ AttributeValue::IMPERATIVE }
		},
		{
			3,
			{ rFirstPerson, rSecondPerson, rThirdPerson },
			{ AttributeValue::FIRST, AttributeValue::SECOND, AttributeValue::THIRD }
		},
		{
			0,
			{},
			{}
		}
	};

	if ( key.index <= 0 || key.index > 10 ) // Если тип запрашиваемого аттрибута некорректен
		return AttributeValue::UNDEFINED; // Возвращаем неопределенное значение

	AttributeInfo info = attributeInfos[ key.index - 1 ];

	for ( int i = 0; i < info.bitsCount; ++ i )
		if ( attributeSet & ( ((uint64)1) << info.bits[ i ] ) )
			return info.values[ i ];

	return AttributeValue::UNDEFINED;
}

Morphology & Morphology::instance() {
	if ( !singletonInstance)
		singletonInstance = new AotMorphology();

	return *singletonInstance;
}

void Morphology::shutdown() {
	if ( !singletonInstance)
		return;

	delete singletonInstance;
	singletonInstance = 0;
}

} } // namespace lspl::morphology
