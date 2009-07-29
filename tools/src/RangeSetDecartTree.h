/**
 * Author: Vadim Antonov(avadim@gmail.com)
 *
 * This class represents modified Decart tree for the RangeSet purposes.
 *
 * If you want to learn more about Decart trees, you can read 
 * "Randomized search trees" paper by Raimund Seidel.
 *
 * IMPORTANT: constructor calls srand(time(NULL)). If you don't want to call
 * srand, you have to use RangeSetDecartTree(false).
 *
 * I don't use scoped_ptr for the tree elements, because element have pointer 
 * to their children and parent. If I'll use scoped_ptr and I'll delete one 
 * element, it will delete all tree elements.
 */

#ifndef __LSPL_BASE_RANGE_SET_DECART_TREE
#define __LSPL_BASE_RANGE_SET_DECART_TREE

#include "RangeSetDecartTreeElement.h"

namespace lspl {
	namespace base {
		class RangeSetDecartTree {
		 protected:
			RangeSetDecartTreeElement *_root;
			int _size;

			void DeleteSubTree(RangeSetDecartTreeElement *root);
 
			// Getter.
			RangeSetDecartTreeElement* root() const;
			
			// Setters.
			void set_size(int size);
			void set_root(RangeSetDecartTreeElement *root);
 
			// Add element into the subtree with root "root".
			void DecartTreeElementPushDown(RangeSetDecartTreeElement *root,
					RangeSetDecartTreeElement *element);
			// Push up element, according it's priority.
			void DecartTreeElementPushUp(RangeSetDecartTreeElement *element);

			// Add element into the tree.
			void AddDecartTreeElement(RangeSetDecartTreeElement *element);

			// Delete element from the tree.
			void DeleteDecartTreeElement(RangeSetDecartTreeElement *element);

			// Find RangeSetDecartTreeElement by range in the tree.
			// Returns RangeSetDecartTreeElement with this range, if it exists 
			// or NULL otherwise.
			RangeSetDecartTreeElement*
				FindDecartTreeElement(const Range &range) const;

			// Find RangeSetDecartTreeElement in the tree, which includes range.
			// Returns RangeSetDecartTreeElement, which includes this range, 
			// if it exists or NULL otherwise.
			// Main idea: find range, which includes range from the parameter.
			// Findind range is similar with Range Max Query.
			// After that find element with this range.
			RangeSetDecartTreeElement* FindDecartTreeElementExtension(
					const Range &range) const;

			void PrintSubtree(RangeSetDecartTreeElement *element,
					const int deep = 0) const;
		 public:
			RangeSetDecartTree(bool need_srand = true);

			virtual ~RangeSetDecartTree();

			// Getter.
			int size() const;

			// Add range into the tree.
			void AddRange(const Range &range);

			// Delete range from the tree.
			void DeleteRange(const Range &range);

			// Find range in the tree.
			// Retruns true, if it's in the tree.
			bool FindRange(const Range &range) const;

			// Find range in the tree, which includes range from the parameter.
			// Returns range, if it exists, or NULL otherwise.
			// Function returns copy of the found range, so, user must delete it
			// after using.
			const Range* FindRangeExtension(const Range &range) const;

			// Print tree to the stdout.
			void Print() const;
		};
	}
}

#endif // __LSPL_BASE_RANGE_SET_DECART_TREE
