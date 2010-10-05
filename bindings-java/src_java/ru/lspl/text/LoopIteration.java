package ru.lspl.text;

import java.util.List;

import ru.lspl.utils.ImmutableArrayList;

/**
 * Итерация цикла в тексте
 * 
 * @author alno
 */
public class LoopIteration extends Transition {

	private List<LoopIterationVariant> variants;
	private LoopIterationVariant[] variantsArray;

	private LoopIteration( int id, Text text, int start, int end ) {
		super( id, text, start, end );
	}

	public List<LoopIterationVariant> getVariants() {
		if ( variants == null ) {
			variantsArray = internalGetVariants();
			variants = new ImmutableArrayList<LoopIterationVariant>( variantsArray );
		}
		return variants;
	}

	private native LoopIterationVariant[] internalGetVariants();

	public native int getVariantTransitionCount( int variantIndex );

	public native Transition getVariantTransition( int variantIndex, int transitionIndex );

}