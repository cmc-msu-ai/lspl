package ru.lspl.text;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.patterns.Alternative;

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

	public Alternative getAlternative() {
		return match.getVariantAlternative( index );
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

	@Override
	public boolean containsPosition( int index ) {
		return index >= match.start.endOffset && index <= match.end.startOffset;
	}

	@Override
	public boolean coincidesWith( TextRange r ) {
		return r != null && r.getText() == match.text && r.getStartOffset() == match.start.endOffset && r.getEndOffset() == match.end.startOffset;
	}

	public boolean coincidesWith( Transition t ) {
		return t != null && t.text == match.text && t.start == match.start && t.end == match.end;
	}

	@Override
	public boolean intersectsWith( TextRange r ) {
		return r != null && r.getText() == match.text && r.getStartOffset() < match.end.startOffset && r.getEndOffset() > match.start.endOffset;
	}

}