package ru.lspl.text;

import java.util.List;

import ru.lspl.patterns.Alternative;
import ru.lspl.patterns.Pattern;
import ru.lspl.utils.ImmutableArrayList;

/**
 * Match of pattern in the text
 * 
 * @author alno
 */
public class Match extends Transition {

	/**
	 * Pattern which was matched
	 */
	public final Pattern pattern;

	private List<MatchVariant> variants;
	private MatchVariant[] variantArray;

	private Match( int id, Text text, int start, int end, Pattern pattern ) {
		super( id, text, start, end );
		this.pattern = pattern;
	}

	public List<MatchVariant> getVariants() {
		if ( variants == null ) {
			variantArray = internalGetVariants();
			variants = new ImmutableArrayList<MatchVariant>( variantArray );
		}
		return variants;
	}

	private MatchVariant getVariant( int index ) {
		return getVariants().get( index );
	}

	private native MatchVariant[] internalGetVariants();

	public native Object getVariantTransformResult( int variantIndex );

	public native int getVariantTransitionCount( int variantIndex );

	public native Alternative getVariantAlternative( int variantIndex );

	public native Transition getVariantTransition( int variantIndex, int transitionIndex );

}