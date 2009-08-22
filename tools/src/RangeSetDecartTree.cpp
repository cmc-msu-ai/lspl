/**
 * Author: Vadim Antonov(avadim@gmail.com)
 */
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "RangeSetDecartTree.h"

namespace lspl {
	namespace base {
		RangeSetDecartTree::RangeSetDecartTree(bool need_srand) : 
				_size(0), _root(NULL) {
			if (need_srand) {
				srand(time(NULL));
			}
		}

		RangeSetDecartTree::~RangeSetDecartTree() {
			DeleteSubTree(_root);
		}

		void RangeSetDecartTree::DeleteSubTree(RangeSetDecartTreeElement *root) {
			if (root == NULL) {
				return;
			}
			DeleteSubTree(root->left_child());
			DeleteSubTree(root->right_child());
			set_size(size() - 1);
			delete root;
		}

		RangeSetDecartTreeElement* RangeSetDecartTree::root() const {
			return _root;
		}

		int RangeSetDecartTree::size() const {
			return _size;
		}

		void RangeSetDecartTree::set_root(RangeSetDecartTreeElement *root) {
			_root = root;
		}

		void RangeSetDecartTree::set_size(int size){
			_size = size;
		}

		void RangeSetDecartTree::AddRange(
				const Range &range) {
			RangeSetDecartTreeElement *element =
					new RangeSetDecartTreeElement(range);
			AddDecartTreeElement(element);
			set_size(size() + 1);
		}
		
		void RangeSetDecartTree::AddDecartTreeElement(
				RangeSetDecartTreeElement *element) {
			if (element == NULL) {
				// TODO: Think about exception.
				return;
			}
			if (root() == NULL) {
				set_root(element);
				return;
			}
			DecartTreeElementPushDown(root(), element);
			DecartTreeElementPushUp(element);
		}

		void RangeSetDecartTree::DecartTreeElementPushDown(
				RangeSetDecartTreeElement *root,
				RangeSetDecartTreeElement *element) {
			if (root == NULL || element == NULL) {
				// TODO: Think about exception.
				return;
			}
			if (*root > *element) {
				if (root->left_child() == NULL) {
					root->set_left_child(element);
					element->set_parent_node(root);
				} else {
					DecartTreeElementPushDown(root->left_child(), element);
				}
			} else {
				if (root->right_child() == NULL) {
					root->set_right_child(element);
					element->set_parent_node(root);
				} else {
					DecartTreeElementPushDown(root->right_child(), element);
				}
			}
			root->RenewSubtreeParameters();
		}

		void RangeSetDecartTree::DecartTreeElementPushUp(
				RangeSetDecartTreeElement *element) {
			if (element == NULL || element->IsRoot() ||
					element->parent_node()->priority() >= element->priority()) {
				// TODO: Think about exception on NULL element.
				if (element != NULL && element->IsRoot()) {
					set_root(element);
				}
				return;
			}
			if (element->IsLeftChild()) {
				// Element is left child.
				element->RotateRight();
			} else {
				// Element is right child.
				element->RotateLeft();
			}
			DecartTreeElementPushUp(element);
		}
		
		void RangeSetDecartTree::DeleteRange(
				const Range &range) {
			RangeSetDecartTreeElement *element = FindDecartTreeElement(range);
			DeleteDecartTreeElement(element);
		}

		void RangeSetDecartTree::DeleteDecartTreeElement(
				RangeSetDecartTreeElement *element) {
			if (element == NULL) {
				// TODO: Think about exception.
				return;
			}
			//Print();
			if (element->IsLeaf()) {
				if (element->IsRoot()) {
					set_root(NULL);
				} else {
					if (element->IsLeftChild()) {
						element->parent_node()->set_left_child(NULL);
					} else {
						element->parent_node()->set_right_child(NULL);
					}
				}
				RangeSetDecartTreeElement *parent= element->parent_node();
				while (parent != NULL) {
					parent->RenewSubtreeParameters();
					parent = parent->parent_node();
				}
				delete element;
				set_size(size() - 1);
				return;
			}
			if (element->left_child() == NULL) {
				if (element->IsRoot()) {
					set_root(element->right_child());
				}
				element->right_child()->RotateLeft();
			} else if (element->right_child() == NULL) {
				if (element->IsRoot()) {
					set_root(element->left_child());
				}
				element->left_child()->RotateRight();
			} else if (element->left_child()->priority() >
					element->right_child()->priority()) {
				if (element->IsRoot()) {
					set_root(element->left_child());
				}
				element->left_child()->RotateRight();
			} else {
				if (element->IsRoot()) {
					set_root(element->right_child());
				}
				element->right_child()->RotateLeft();
			}
			DeleteDecartTreeElement(element);
		}

		bool RangeSetDecartTree::FindRange(const Range &range) const {
			return FindDecartTreeElement(range) != NULL;
		}

		RangeSetDecartTreeElement* RangeSetDecartTree::FindDecartTreeElement(
				const Range &range) const {
			RangeSetDecartTreeElement* element = root();
			while (element != NULL and *(element->range()) != range) {
				if (*(element->range()) > range) {
					element = element->left_child();
				} else {
					element = element->right_child();
				}
			}
			return element;
		}

		const Range* RangeSetDecartTree::FindRangeExtension(
				const Range &range) const {
			RangeSetDecartTreeElement* element =
					FindDecartTreeElementExtension(range);
			if (element == NULL) {
				return NULL;
			}
			return element->range();
		}

		RangeSetDecartTreeElement* 
				RangeSetDecartTree::FindDecartTreeElementExtension(
				const Range &range) const {
			RangeSetDecartTreeElement* element = root();
			while (element != NULL && !element->range()->IsIncludeRange(range)) {
				if (element->range()->start > range.start) {
					element = element->left_child();
					continue;
				}
				// Now elements in the left tree have their starts less than
				// range.start.
				if (element->left_child() != NULL && 
						(element->left_child()->range()->end >= range.end ||
						(element->left_child()->subtree_max_right_part_of_ranges() != NULL
						&& element->left_child()->subtree_max_right_part_of_ranges()->end >=
						range.end))) {
					element = element->left_child();
				} else {
					element = element->right_child();
				}
			}
			return element;
		}

		void RangeSetDecartTree::Print() const {
			PrintSubtree(root());
		}

		void RangeSetDecartTree::PrintSubtree(
				RangeSetDecartTreeElement *element, const int deep) const {
			if (element == NULL) {
				return;
			}
			for(int i = 0; i < deep; ++i) {
				printf("  ");
			}
			if (element->subtree_max_right_part_of_ranges() == NULL) {
				printf("P:%d range:(%d, %d)", element->priority(),
						element->range()->start, element->range()->end);
			} else {
				printf("P:%d range:(%d, %d) mrp:(%d %d)", element->priority(),
						element->range()->start, element->range()->end,
						element->subtree_max_right_part_of_ranges()->start,
						element->subtree_max_right_part_of_ranges()->end);
			}
			printf("\r\n");
			printf("L:");
			PrintSubtree(element->left_child(), deep + 1);
			printf("\r\n");
			printf("R:");
			PrintSubtree(element->right_child(), deep + 1);
			printf("\r\n");
		}
	} // namespace base
} // namespace lspl
