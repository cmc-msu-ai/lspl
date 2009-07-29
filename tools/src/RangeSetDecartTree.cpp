/*
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
				// TODO: Think aboyt exception.
				return;
			}
			if (root() == NULL) {
				set_root(element);
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
			if (*root < *element) {
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
		}

		void RangeSetDecartTree::DecartTreeElementPushUp(
				RangeSetDecartTreeElement *element) {
			if (element == NULL || element->IsRoot() ||
					element->parent_node()->priority() >= element->priority()) {
				// TODO: Think about exception on NULL element.
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
		}

		void RangeSetDecartTree::DeleteDecartTreeElement(
				RangeSetDecartTreeElement *element) {
			if (element == NULL) {
				// TODO: Think about exception.
				return;
			}
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
				delete element;
				return;
			}
			if (element->left_child() == NULL) {
				element->RotateLeft();
			} else if (element->right_child() == NULL) {
				element->RotateRight();
			} else if (element->left_child()->priority() >
					element->right_child()->priority()) {
				element->RotateLeft();
			} else {
				element->RotateRight();
			}
			DeleteDecartTreeElement(element);
		}

		bool RangeSetDecartTree::FindRange(const Range &range) {
			return FindDecartTreeElement(range) != NULL;
		}

		RangeSetDecartTreeElement* RangeSetDecartTree::FindDecartTreeElement(
				const Range &range) {
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
				const Range &range) {
			RangeSetDecartTreeElement* element =
					FindDecartTreeElementExtension(range);
			if (element == NULL) {
				return NULL;
			}
			return element->range();
		}

		RangeSetDecartTreeElement* 
				RangeSetDecartTree::FindDecartTreeElementExtension(
				const Range &range) {
			RangeSetDecartTreeElement* element = root();
			//while (element.Range)
			return NULL;
		}
	}
}
