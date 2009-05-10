/*
 * AotGraphan.h
 *
 *  Created on: May 9, 2009
 *      Author: alno
 */

#ifndef _LSPL_GRAPHAN_AOTGRAPHAN_H_
#define _LSPL_GRAPHAN_AOTGRAPHAN_H_

#include "Graphan.h"

namespace lspl { namespace graphan {

class AotGraphan: public lspl::graphan::Graphan {
public:
	AotGraphan();
	virtual ~AotGraphan();

	void analyzeString( const std::string & str, boost::ptr_vector<Unit> & tokens );
private:
	const char * findRml();
	void setupRml();
};

} }

#endif /* _LSPL_GRAPHAN_AOTGRAPHAN_H_ */
