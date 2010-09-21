package ru.lspl.text;

import java.util.AbstractList;
import java.util.List;

/**
 * Вариант сопоставления
 * 
 * @author alno
 */
public class MatchVariant implements TextRange {

	private class TransitionList extends AbstractList<Transition> {

		@Override
		public int size() {
			return getTransitionCount();
		}

		@Override
		public Transition[] toArray() {
			Transition[] ts = new Transition[size()];

			for ( int i = 0; i < ts.length; ++i )
				ts[i] = get( i );

			return ts;
		}

		@Override
		public Transition get( int index ) {
			return getTransition( index );
		}

	};

	public final Match match;

	public final int index;

	public final List<Transition> transitions = new TransitionList();

	private MatchVariant( Match match, int index ) {
		this.match = match;
		this.index = index;
	}

	public Object getTransformResult() {
		return match.getVariantTransformResult( index );
	}

	public List<Transition> getTransitions() {
		return transitions;
	}

	public int getTransitionCount() {
		return match.getVariantTransitionCount( index );
	}

	public Transition getTransition( int transitionIndex ) {
		return match.getVariantTransition( index, transitionIndex );
	}

	@Override
	protected void finalize() {
		match.finalizeVariant( index );
	}

	@Override
	public Text getText() {
		return match.text;
	}

	@Override
	public int getStartOffset() {
		return match.getStartOffset();
	}

	@Override
	public int getEndOffset() {
		return match.getEndOffset();
	}

	@Override
	public String getContent() {
		return match.getContent();
	}

}