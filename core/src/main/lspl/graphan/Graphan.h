/*
 * Graphan.h
 *
 *  Created on: May 9, 2009
 *      Author: alno
 */

#ifndef _LSPL_GRAPHAN_GRAPHAN_H_
#define _LSPL_GRAPHAN_GRAPHAN_H_

#include "../base/Base.h"

#include "Unit.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace lspl { namespace graphan {

class LSPL_EXPORT Graphan {
public:

	/**
	 * Получить экземпляр модуля графематики
	 */
	static Graphan & instance();

	/**
	 * Завершить работу модуля графематики
	 */
	static void shutdown();

private:
	static Graphan * singletonInstance;

public:
	Graphan();
	virtual ~Graphan();

	virtual void analyzeString( const std::string & str, boost::ptr_vector<Unit> & units ) = 0;
};

} } // namespace lspl::graphan

#endif /* _LSPL_GRAPHAN_GRAPHAN_H_ */
