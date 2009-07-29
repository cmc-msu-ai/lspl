/**
 * Author: Vadim Antonov(avadim@gmail.com).
 */

#include <algorithm>
#include <iostream>
#include "RangeSetDecartTreeElement.h"

namespace lspl {
	namespace base {
		RangeSetDecartTreeElement::RangeSetDecartTreeElement(const Range &range) {
			Initialization(range, NULL);
		}
		RangeSetDecartTreeElement::RangeSetDecartTreeElement(const RangeSetDecartTreeElement& element) {
			Initialization(*(element.range()), NULL);
		}
		RangeSetDecartTreeElement::RangeSetDecartTreeElement(const Range &range,
				RangeSetDecartTreeElement *parent) {
			Initialization(range, parent);
		}
		RangeSetDecartTreeElement::~RangeSetDecartTreeElement() {
		}
			
		void RangeSetDecartTreeElement::Initialization(const Range &range,
				RangeSetDecartTreeElement *parent_node) {
			_range.reset(new Range(range));
			_parent_node = parent_node;
			_left_child = NULL;
			_right_child = NULL;
			_priority = rand() % MAX_PRIORITY;
			RenewSubtreeParameters();
		}

		void RangeSetDecartTreeElement::RenewSubtreeParameters() {
			set_subtree_size(1);
			set_subtree_height(1);
			set_subtree_max_right_part_of_ranges(NULL);
			if (left_child() != NULL) {
				set_subtree_size(subtree_size() + left_child()->subtree_size());
				set_subtree_height(1 + left_child()->subtree_height());
				set_subtree_max_right_part_of_ranges(
						left_child()->subtree_max_right_part_of_ranges());
				if (subtree_max_right_part_of_ranges() == NULL ||
						left_child()->range()->end >
						subtree_max_right_part_of_ranges()->end) {
					set_subtree_max_right_part_of_ranges(left_child()->range());
				}
			}
			if (right_child() != NULL) {
				set_subtree_size(subtree_size() + right_child()->subtree_size());
				set_subtree_height(std::max(subtree_height(),
						1 + right_child()->subtree_height()));
				if (subtree_max_right_part_of_ranges() == NULL ||
						(right_child()->subtree_max_right_part_of_ranges() != NULL &&
						right_child()->subtree_max_right_part_of_ranges()->end >
						subtree_max_right_part_of_ranges()->end)) {
					set_subtree_max_right_part_of_ranges(
							right_child()->subtree_max_right_part_of_ranges());
				}
				if (subtree_max_right_part_of_ranges() == NULL ||
						right_child()->range()->end >
						subtree_max_right_part_of_ranges()->end) {
					set_subtree_max_right_part_of_ranges(right_child()->range());
				}
			}
		}

		// Getters.
		const Range* RangeSetDecartTreeElement::range() const {
			return _range.get();
		}
		RangeSetDecartTreeElement* RangeSetDecartTreeElement::left_child() const {
			return _left_child;
		}
		RangeSetDecartTreeElement* RangeSetDecartTreeElement::right_child() const {
			return _right_child;
		}
		RangeSetDecartTreeElement* RangeSetDecartTreeElement::parent_node() const {
			return _parent_node;
		}
		int RangeSetDecartTreeElement::priority() const {
			return _priority;
		}
		int RangeSetDecartTreeElement::subtree_height() const {
			return _subtree_height;
		}
		const Range* RangeSetDecartTreeElement::subtree_max_right_part_of_ranges() const {
			return _subtree_max_right_part_of_ranges;
		}
		int RangeSetDecartTreeElement::subtree_size() const {
			return _subtree_size;
		}

		// Setters.
		void RangeSetDecartTreeElement::set_left_child(
				RangeSetDecartTreeElement *element) {
			_left_child = element;
			RenewSubtreeParameters();
		}
		void RangeSetDecartTreeElement::set_right_child(
				RangeSetDecartTreeElement *element) {
			_right_child = element;
			RenewSubtreeParameters();
		}
		void RangeSetDecartTreeElement::set_parent_node(
				RangeSetDecartTreeElement *element) {
			_parent_node = element;
		}
		int RangeSetDecartTreeElement::set_subtree_max_right_part_of_ranges(
				const Range *subtree_max_right_part_of_ranges) {
			_subtree_max_right_part_of_ranges = subtree_max_right_part_of_ranges;
		}
		int RangeSetDecartTreeElement::set_subtree_height(
				const int &subtree_height) {
			_subtree_height = subtree_height;
		}
		int RangeSetDecartTreeElement::set_subtree_size(const int &subtree_size) {
			_subtree_size = subtree_size;
		}
			
		bool RangeSetDecartTreeElement::IsLeftChild() const {
			return !IsRoot() && parent_node()->left_child() == this;
		}
		bool RangeSetDecartTreeElement::IsRightChild() const {
			return !IsRoot() && parent_node()->right_child() == this;
		}
		bool RangeSetDecartTreeElement::IsRoot() const {
			return parent_node() == NULL;
		}
		bool RangeSetDecartTreeElement::IsLeaf() const {
			return left_child() == NULL && right_child() == NULL;
		}

		void RangeSetDecartTreeElement::RotateLeft() {
			if (IsRoot()) {
				return;
			}
			parent_node()->set_right_child(left_child());
			if (left_child() != NULL) {
				left_child()->set_parent_node(parent_node());
			}
			set_left_child(parent_node());
			if (!parent_node()->IsRoot()) {
				if (parent_node()->IsLeftChild()) {
					parent_node()->parent_node()->set_left_child(this);
				} else {
					parent_node()->parent_node()->set_right_child(this);
				}
			}
			set_parent_node(parent_node()->parent_node());
			left_child()->set_parent_node(this);
			// RenewSubtreeParameters was called in set_right_child and
			// set_left_child.
		}
		void RangeSetDecartTreeElement::RotateRight() {
			if (IsRoot()) {
				return;
			}
			parent_node()->set_left_child(right_child());
			if (right_child() != NULL) {
				right_child()->set_parent_node(parent_node());
			}
			set_right_child(parent_node());
			if (!parent_node()->IsRoot()) {
				if (parent_node()->IsLeftChild()) {
					parent_node()->parent_node()->set_left_child(this);
				} else {
					parent_node()->parent_node()->set_right_child(this);
				}
			}
			set_parent_node(parent_node()->parent_node());
			right_child()->set_parent_node(this);
			// RenewSubtreeParameters was called in set_right_child and
			// set_left_child.
		}
			
		bool RangeSetDecartTreeElement::operator<(const RangeSetDecartTreeElement &element) {
			return *range() < *(element.range());
		}
		bool RangeSetDecartTreeElement::operator>(const RangeSetDecartTreeElement &element) {
			return *range() > *(element.range());
		}
		bool RangeSetDecartTreeElement::operator==(const RangeSetDecartTreeElement &element) {
			return *range() == *(element.range());
		}
	}
}

