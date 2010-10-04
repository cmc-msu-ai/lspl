package ru.lspl.text;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.patterns.Alternative;
import ru.lspl.patterns.Pattern;

/**
 * Сопоставление в тексте.
 * 
 * @author alno
 */
public class Match extends Transition {

	private class VariantList extends AbstractList<MatchVariant> {

		@Override
		public int size() {
			return getVariantCount();
		}

		@Override
		public MatchVariant[] toArray() {
			MatchVariant[] ts = new MatchVariant[size()];

			for ( int i = 0; i < ts.length; ++i )
				ts[i] = getVariant( i );

			return ts;
		}

		@Override
		public MatchVariant get( int index ) {
			return getVariant( index );
		}

	};

	/**
	 * Шаблон, который был сопоставлен
	 */
	public final Pattern pattern;

	public final List<MatchVariant> variants = new VariantList();

	private Match( int id, Text text, int start, int end, Pattern pattern ) {
		super( id, text, start, end );
		this.pattern = pattern;
	}

	public List<MatchVariant> getVariants() {
		return variants;
	}

	/**
	 * Получить количество вариантов сопоставления
	 */
	public native int getVariantCount();

	/**
	 * Получить вариант сопоставления по его индексу
	 */
	public native MatchVariant getVariant( int variantIndex );

	public native Object getVariantTransformResult( int variantIndex );

	public native int getVariantTransitionCount( int variantIndex );

	public native Alternative getVariantAlternative( int variantIndex );

	public native Transition getVariantTransition( int variantIndex, int transitionIndex );

	native void finalizeVariant( int variantIndex );
}