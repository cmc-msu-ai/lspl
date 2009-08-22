/**
 * Author: Vadim Antonov(avadim@gmail.com)
 *
 * This class implements RangeSet, based on Decart Tree.
 * RangeSet can include equal ranges.
 */

#ifndef __LSPL_BASE_RANGE_SET
#define __LSPL_BASE_RANGE_SET

#include "lspl/base/Range.h"
#include "RangeSetDecartTree.h"

namespace lspl {
	namespace base {
		class RangeSet {
		 private:
			boost::scoped_ptr<RangeSetDecartTree> tree;
		 public:
			// If parameter need_srand is true, it calls srand(time(NULL)).
			RangeSet(bool need_srand = true);
			virtual ~RangeSet();

			// Add range into the RangeSet.
			void AddRange(const Range &range);
			// Delete Range from the RangeSet.
			void DeleteRange(const Range &range);
			// Find Range in the RangeSet.
			bool FindRange(const Range &range);
			// Find range in the RangeSet, which includes range from the parameter.
			// Returns range, if it exists, or NULL otherwise.
			const Range *FindRangeExtension(const Range &range);

			// Get number of ranges in the RangeSet.
			int size();
		};
	} // namespace base
} // namespace lspl

#endif // __LSPL__BASE_RANGE_SET
