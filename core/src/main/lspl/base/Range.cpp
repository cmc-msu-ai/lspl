/*
 * Range.cpp
 *
 *	Created on: Jul 19, 2009
 *			Author: Vadim Antonov(avadim@gmail.com)
 */
#include "BaseInternal.h"
#include "Range.h"

namespace lspl { namespace base {
	bool Range::IsIncludeRange(const Range &range) const {
		return (start <= range.start && end >= range.end);
	}

	bool Range::IsIntersectRange(const Range &range) const {
		return (start <= range.start && range.end > start) ||
				(range.start < end && end <= range.end);
	}

	bool operator<(const Range& range1, const Range& range2) {
		return (range1.start < range2.start) ||
			((range1.start == range2.start) && (range1.end < range2.end));
	}

	bool operator>(const Range& range1, const Range& range2) {
		return (range1.start > range2.start) ||
			((range1.start == range2.start) && (range1.end > range2.end));
	}

	bool operator==(const Range& range1, const Range& range2) {
		return (range1.start == range2.start) && (range1.end == range2.end);
	}

	bool operator!=(const Range& range1, const Range& range2) {
		return (range1.start != range2.start) || (range1.end != range2.end);
	}


} } // namespace lspl::base

