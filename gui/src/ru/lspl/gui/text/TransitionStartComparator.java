package ru.lspl.gui.text;

import java.util.Comparator;

import ru.lspl.text.Transition;

/**
 * @author alno
 */
public class TransitionStartComparator implements Comparator<Transition> {

	/**
	 * @uml.property name="iNSTANCE"
	 * @uml.associationEnd
	 */
	public static final TransitionStartComparator INSTANCE = new TransitionStartComparator();

	@Override
	public int compare( Transition t1, Transition t2 ) {
		int diff = t1.start.endOffset - t2.start.endOffset;

		if ( diff < 0 )
			return -1;
		else if ( diff > 0 )
			return 1;
		else
			return 0;
	}

	private TransitionStartComparator() {
	}
}
