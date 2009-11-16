package ru.lspl.text;

import java.util.List;
import java.util.AbstractList;

/**
 * Вариант итерации цикла
 * 
 * @author alno
 */
public class LoopIterationVariant {
	
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
	 * Итерация
	 */
	public final LoopIteration iteration;
	
	public final int index;
	
	public final List<Transition> transitions = new TransitionList();

	private LoopIterationVariant(LoopIteration iteration,int index) {
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
		
	protected void finalize() {
		iteration.finalizeVariant( index );
	}
}