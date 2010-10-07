package ru.lspl.gui.text.viewers;

import java.util.ArrayList;

import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.Viewer;

import ru.lspl.gui.model.Document;
import ru.lspl.text.Node;
import ru.lspl.text.Transition;
import ru.lspl.text.attributes.SpeechPart;

/**
 * @author alno
 */
public class TextWordsProvider implements ITreeContentProvider {

	private static final Object[] EMPTY_ARRAY = new Object[0];

	/**
	 * @uml.property name="speechPart"
	 * @uml.associationEnd
	 */
	private final SpeechPart speechPart;

	public TextWordsProvider( SpeechPart sp ) {
		speechPart = sp;
	}

	@Override
	public Object[] getChildren( Object obj ) {
		if ( obj instanceof Node )
			return ((Node) obj).getWords( speechPart ).toArray();

		return EMPTY_ARRAY;
	}

	@Override
	public Object getParent( Object obj ) {
		if ( obj instanceof Transition )
			return ((Transition) obj).start;

		return null;
	}

	@Override
	public boolean hasChildren( Object obj ) {
		if ( obj instanceof Node )
			return !((Node) obj).getWords( speechPart ).isEmpty();

		return false;
	}

	@Override
	public Object[] getElements( Object input ) {
		if ( input == null )
			return EMPTY_ARRAY;

		if ( !(input instanceof Document) )
			throw new IllegalArgumentException( "Input is " + input.getClass().getName() + ", while Document expected" );

		ArrayList<Object> elems = new ArrayList<Object>();

		for ( Node n : ((Document) input).getAnalyzedText().getNodes() )
			if ( hasChildren( n ) )
				elems.add( n );

		return elems.toArray();
	}

	@Override
	public void dispose() {
		// TODO Auto-generated method stub
	}

	@Override
	public void inputChanged( Viewer arg0, Object arg1, Object arg2 ) {
		// TODO Auto-generated method stub
	}

}
