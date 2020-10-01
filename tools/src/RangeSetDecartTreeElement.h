/**
 * Author: Vadim Antonov(avadim@gmail.com).
 *
 * This class represents modified Decart tree element for the RangeSet purposes.
 *
 * Only RangeSetDecartTree class can use this class.
 */

#ifndef __LSPL_BASE_RANGE_SET_DECART_TREE_ELEMENT
#define __LSPL_BASE_RANGE_SET_DECART_TREE_ELEMENT

#include <boost/scoped_ptr.hpp>

#include "lspl/base/Range.h"

#define MAX_PRIORITY 1000000000

namespace lspl {
	namespace base {
		class RangeSetDecartTreeElement {
		 protected:
			RangeSetDecartTreeElement *_left_child, *_right_child, *_parent_node;
			boost::scoped_ptr<Range> _range;
			int _priority;
			int _subtree_height;
			// It doesn't include the element. Only it's children.
			const Range *_subtree_max_right_part_of_ranges;
			int _subtree_size;

			// New object will use range elements, which we will pass
			// into the constructor.
			RangeSetDecartTreeElement(const Range &range);
			RangeSetDecartTreeElement(const RangeSetDecartTreeElement& element);
			RangeSetDecartTreeElement(const Range &range,
					RangeSetDecartTreeElement *parent);
			// It deletes range, which we use in the tree element.
			virtual ~RangeSetDecartTreeElement();
			
			void Initialization(const Range &range,
					RangeSetDecartTreeElement *parent);

			// Renew subtree size and max right part of ranges in subtree.
			void RenewSubtreeParameters();

			// Getters.
			const Range *range() const;
			RangeSetDecartTreeElement* left_child() const;
			RangeSetDecartTreeElement* right_child() const;
			RangeSetDecartTreeElement* parent_node() const;
			int priority() const;
			int subtree_height() const;
			const Range *subtree_max_right_part_of_ranges() const;
			int subtree_size() const;

			// Setters.
			void set_left_child(RangeSetDecartTreeElement *element);
			void set_right_child(RangeSetDecartTreeElement *element);
			void set_parent_node(RangeSetDecartTreeElement *element);
			void set_subtree_max_right_part_of_ranges(const Range *subtree_max_right_part_of_ranges);
			void set_subtree_height(const int &subtree_height);
			void set_subtree_size(const int &subtree_size);

			// Returns true, if the element is a left child.
			bool IsLeftChild() const;
			// The same thing for a right child.
			bool IsRightChild() const;
			// Returns true, if the element is a root.
			bool IsRoot() const;
			// Returns true, if the element is a leaf.
			bool IsLeaf() const;

			// Rotates Y
			//   X          Y
			//  / \        / \
			// A   Y   -> X  C
			//    / \    / \
			//   B   C  A   B
			void RotateLeft();
			// Rotates X
			//   X          Y
			//  / \        / \
			// A   Y   <- X  C
			//    / \    / \
			//   B   C  A   B
			void RotateRight();
			
			bool operator<(const RangeSetDecartTreeElement &element);
			bool operator>(const RangeSetDecartTreeElement &element);
			bool operator==(const RangeSetDecartTreeElement &element);

			friend class RangeSetDecartTree;
		};
	} // namespace base
} // namespace lspl

#endif // __LSPL_BASE_RANGE_SET_DECART_TREE_ELEMENT
