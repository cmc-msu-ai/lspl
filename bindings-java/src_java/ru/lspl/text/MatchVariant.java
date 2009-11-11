package ru.lspl.text;

import ru.lspl.patterns.Pattern;

import java.util.List;
import java.util.AbstractList;

/**
 * Вариант сопоставления
 * 
 * @author alno
 */
public class MatchVariant {
	
	private class TransitionList extends AbstractList<Transition> {
		
		@Override
		public int size() {
			return getTransitionCount();
		}

		@Override
		public Transition[] toArray() {
			Transition[] ts = new Transition[size()];

			for (int i = 0; i < ts.length; ++i)
				ts[i] = get(i);

			return ts;
		}

		@Override
		public Transition get(int index) {
			return getTransition( index );
		}
		
	};

	/**
	 * Сопоставление
	 */
	public final Match match;
	
	public final int index;
	
	public final List<Transition> transitions = new TransitionList();

	private MatchVariant(Match match,int index) {
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
		
	protected void finalize() {
		match.finalizeVariant( index );
	}
}