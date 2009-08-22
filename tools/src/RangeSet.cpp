/**
 * Author: Vadim Antonov(avadim@gmail.com)
 */

#include "RangeSet.h"

namespace lspl {
	namespace base {
		RangeSet::RangeSet(bool need_srand) {
			tree.reset(new RangeSetDecartTree(need_srand));
		}

		RangeSet::~RangeSet() {
		}

		void RangeSet::AddRange(const Range &range) {
			tree->AddRange(range);
		}
		void RangeSet::DeleteRange(const Range &range) {
			tree->DeleteRange(range);
		}
		bool RangeSet::FindRange(const Range &range) {
			return tree->FindRange(range);
		}
		const Range *RangeSet::FindRangeExtension(const Range &range) {
			return tree->FindRangeExtension(range);
		}

		int RangeSet::size() {
			return tree->size();
		}
	} // namespace base
} // namespace lspl
