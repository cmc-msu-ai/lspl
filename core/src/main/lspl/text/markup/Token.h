#ifndef _LSPL_TEXT_MARKUP_TOKEN_H_
#define _LSPL_TEXT_MARKUP_TOKEN_H_

#include "../../base/Base.h"

#include <string>

#include "../Transition.h"

namespace lspl { namespace text { namespace markup {

/**
 * Аннотация, представляющаяо одну лексему в тексте.
 */
class LSPL_EXPORT Token : public Transition
{
public:
	Token( const text::Node & start, const text::Node & end, const std::string & token );
	virtual ~Token();

	virtual void dump(std::ostream & out, std::string tabs = "") const;

	/**
	 * Получить значение лексемы
	 * @return значение лексемы
	 */
	const std::string & getToken() const { return token; }

private:
	const std::string token;
};

} } } // namespace lspl::text::markup

#endif /*_LSPL_TEXT_MARKUP_TOKEN_H_*/
