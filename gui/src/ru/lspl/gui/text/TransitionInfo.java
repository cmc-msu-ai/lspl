package ru.lspl.gui.text;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import ru.lspl.text.Transition;

/**
 * @author alno
 */
public class TransitionInfo extends Composite {

	/**
	 * @uml.property name="nO_TRANSITIONS"
	 * @uml.associationEnd multiplicity="(0 -1)"
	 */
	private static final Transition[] NO_TRANSITIONS = new Transition[0];

	private Text textArea = null;

	/**
	 * @uml.property name="transitions"
	 * @uml.associationEnd multiplicity="(0 -1)"
	 */
	private Transition[] transitions = NO_TRANSITIONS;

	public TransitionInfo( Composite parent, int style ) {
		super( parent, style );
		initialize();
	}

	private void initialize() {
		textArea = new Text( this, SWT.MULTI | SWT.WRAP | SWT.V_SCROLL );
		setSize( new Point( 300, 200 ) );
		setLayout( new FillLayout() );
	}

	/**
	 * @return
	 * @uml.property name="transitions"
	 */
	public Transition[] getTransitions() {
		return transitions;
	}

	public void setTransitions( Transition transition ) {
		setTransitions( new Transition[] { transition } );
	}

	/**
	 * @param transitions
	 * @uml.property name="transitions"
	 */
	public void setTransitions( Transition[] transitions ) {
		this.transitions = transitions;

		StringBuilder builder = new StringBuilder();

		for ( Transition transition : transitions ) {
			builder.append( transition.dump() );
			builder.append( "\n\n" );
		}

		textArea.setText( builder.toString() );
	}

}
