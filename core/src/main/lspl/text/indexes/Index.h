/*
 * Index.h
 *
 *  Created on: Mar 18, 2009
 *      Author: alno
 */

#ifndef _LSPL_TEXT_INDEX_H_
#define _LSPL_TEXT_INDEX_H_

#include "../../base/Base.h"
#include "../Transition.h"

namespace lspl { namespace text { namespace indexes {

/**
 * Базовый класс индекса, используемого при поиске в тексте.
 * Индексы используются для ускорения процесса поиска шаблонов в тексте.
 */
class LSPL_EXPORT Index {
public:

	/**
	 * Итератор, используемый для обхода аннотаций, извлеченных из индекса.
	 */
	class LSPL_EXPORT Iterator {
	public:
		Transition * operator  *() { return get(); }
		Transition * operator ->() { return get(); }

		void operator ++() { increment(); }

		virtual Transition * get() = 0;
		virtual void increment() = 0;
		virtual bool finished() = 0;
	};

public:
	Index();
	virtual ~Index();
};

} } } // namespace lspl::text::indexes

#endif /* _LSPL_TEXT_INDEX_H_ */
