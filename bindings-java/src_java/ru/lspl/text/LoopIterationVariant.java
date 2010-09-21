package ru.lspl.text;

import java.util.AbstractList;
import java.util.List;

/**
 * Вариант итерации цикла
 * 
 * @author alno
 */
public class LoopIterationVariant implements TextRange {

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

	/**
	 * Итерация
	 */
	public final LoopIteration iteration;

	public final int index;

	public final List<Transition> transitions = new TransitionList();

	private LoopIterationVariant( LoopIteration iteration, int index ) {
		this.iteration = iteration;
		this.index = index;
	}

	public List<Transition> getTransitions() {
		return transitions;
	}

	public int getTransitionCount() {
		return iteration.getVariantTransitionCount( index );
	}

	public Transition getTransition( int transitionIndex ) {
		return iteration.getVariantTransition( index, transitionIndex );
	}

	@Override
	protected void finalize() {
		iteration.finalizeVariant( index );
	}

	@Override
	public Text getText() {
		return iteration.text;
	}

	@Override
	public int getStartOffset() {
		return iteration.getStartOffset();
	}

	@Override
	public int getEndOffset() {
		return iteration.getEndOffset();
	}

	@Override
	public String getContent() {
		return iteration.getContent();
	}

	@Override
	public boolean containsPosition( int index ) {
		return index >= iteration.start.endOffset && index <= iteration.end.startOffset;
	}

	@Override
	public boolean coincidesWith( TextRange r ) {
		return r != null && r.getText() == iteration.text && r.getStartOffset() == iteration.start.endOffset && r.getEndOffset() == iteration.end.startOffset;
	}

	public boolean coincidesWith( Transition t ) {
		return t != null && t.text == iteration.text && t.start == iteration.start && t.end == iteration.end;
	}

	@Override
	public boolean intersectsWith( TextRange r ) {
		return r != null && r.getText() == iteration.text && r.getStartOffset() < iteration.end.startOffset && r.getEndOffset() > iteration.start.endOffset;
	}

}