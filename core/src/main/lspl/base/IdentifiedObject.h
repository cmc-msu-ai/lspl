#ifndef _LSPL_BASE_IDENTIFIEDOBJECT_H_
#define _LSPL_BASE_IDENTIFIEDOBJECT_H_

#include "Base.h"

namespace lspl { namespace base {

template< class Type >
class LSPL_EXPORT IdentifiedObject {
public:
	IdentifiedObject() : id( count ++ ) {}
	~IdentifiedObject() {}

public:

	/**
	 * Уникальный идентификатор объекта
	 */
	const uint id;

private:

	/**
	 * Кол-во созданных объектов заданного типа
	 */
	static uint count;
};

} } //namespace lspl::base

#endif /*_LSPL_BASE_IDENTIFIEDOBJECT_H_*/
