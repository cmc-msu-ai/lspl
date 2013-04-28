
#ifndef _LSPL_TRANSFORMS_TEXTTRANSFORM_H_
#define _LSPL_TRANSFORMS_TEXTTRANSFORM_H_

#include "Transform.h"

#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include "../patterns/matchers/Forward.h"

#include <map>
#include <utility>

#include "../patterns/Alternative.h"
#include "../text/Forward.h"
#include "../text/attributes/AttributeKey.h"
#include "../text/attributes/AttributeValue.h"
#include "../patterns/matchers/Variable.h"
#include "../patterns/expressions/Expression.h"

namespace lspl { namespace transforms {

class LSPL_EXPORT TextTransform : public TypedTransform<std::string> {
  public:
	typedef boost::ptr_map<text::attributes::AttributeKey, patterns::expressions::Expression> BindingMap;
	typedef std::multimap< patterns::matchers::Variable, text::TransitionConstRef > Map;
	typedef Map::const_iterator ConstIterator;
	typedef std::pair<ConstIterator,ConstIterator> ConstRange;

	struct cmp {
		bool operator() (const text::Transition* left, const text::Transition* right) const {
			return (left != right);
		}
	};
	typedef std::map<const text::Transition*,unsigned int, cmp> AttributesMap;
	typedef std::pair<const text::Transition*,unsigned int> AttributesPair;

	//преобразование части речи из АОТа в lspl
	static const unsigned int SpeechPartTable[22];
	//удалить значение атрибута в бит-маске 
	static const unsigned int GrammemClear[11];
	//удалить значение атрибутов у части речи
	static const unsigned int ClearSP[14];
	//бит-маска нормализации
	static const unsigned int Normalized = 0x08218502;
									

	TextTransform();
	virtual ~TextTransform();

    TextTransform( boost::ptr_vector<lspl::patterns::matchers::Matcher>* matchers, const BindingMap &bindings ): matchers( matchers ), bindings(bindings) {}

	const boost::ptr_vector<lspl::patterns::matchers::Matcher> & getMatchers() const {
		return *matchers;
	}
	
	const BindingMap & getBindings() const {
		return bindings;
	}
    
    virtual std::string apply( const lspl::text::MatchVariant & v ) const;  
    virtual std::string apply( const lspl::text::MatchVariant & v, unsigned int attributes) const;  
	void buildStr( 
		std::string & str, 
		const lspl::text::MatchVariant & v, 
		const boost::ptr_vector<lspl::patterns::matchers::Matcher> & matchers, 
		unsigned int globalAttributes
	) const;

  private :
	boost::ptr_vector<lspl::patterns::matchers::Matcher> *matchers;
	const BindingMap &bindings;
};

} } // namespace lspl::transforms

#endif /* _LSPL_TRANSFORMS_TEXTTRANSFORM_H_ */
