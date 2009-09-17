package ru.lspl.text;

import ru.lspl.patterns.Pattern;

import java.util.List;
import java.util.AbstractList;

/**
 * Цикл в тексте
 * 
 * @author alno
 */
public class Loop extends Transition {
	
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
	
	public final List<Transition> transitions = new TransitionList();

	private Loop(int id, Text text, Node start, Node end) {
		super(id, text, start, end);
	}

	public List<Transition> getTransitions() {
		return transitions;
	}
	
	public native int getTransitionCount();
	
	public native Transition getTransition(int index);
	
}