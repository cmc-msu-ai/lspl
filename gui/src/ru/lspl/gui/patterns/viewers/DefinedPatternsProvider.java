package ru.lspl.gui.patterns.viewers;

import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.Viewer;

import ru.lspl.patterns.Pattern;
import ru.lspl.gui.model.Document;
import ru.lspl.patterns.Alternative;

public class DefinedPatternsProvider implements ITreeContentProvider {
	
	private static final Object[] EMPTY_ARRAY = new Object[0];

	@Override
	public Object[] getChildren( Object obj ) {
		if ( obj instanceof Pattern )
			return ((Pattern) obj).getAlternatives().toArray();
		
		return EMPTY_ARRAY;
	}

	@Override
	public Object getParent( Object obj ) {
		if ( obj instanceof Alternative )
			return ((Alternative) obj).pattern;
		
		return null;
	}

	@Override
	public boolean hasChildren( Object obj ) {
		return obj instanceof Pattern;
	}

	@Override
	public Object[] getElements( Object obj ) {
		return ((Document) obj).getPatternList().toArray();
	}

	@Override
	public void dispose() {
		// TODO Auto-generated method stub		
	}

	@Override
	public void inputChanged( Viewer arg0, Object arg1, Object arg2 ) {
		// TODO Auto-generated method stub		
	}

	/*@Override
	public Object[] getElements(Object input) {
		if ( input == null )
			return new Object[0]; 
			
		return ((Document)input).getPatternsArray();		
	}

	@Override
	public void dispose() {
		// TODO Auto-generated method stub

	}

	@Override
	public void inputChanged(Viewer arg0, Object arg1, Object arg2) {
		// TODO Auto-generated method stub

	}*/

}
