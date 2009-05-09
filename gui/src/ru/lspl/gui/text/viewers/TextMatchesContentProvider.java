package ru.lspl.gui.text.viewers;

import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.Viewer;

import ru.lspl.patterns.Pattern;
import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentHolder;
import ru.lspl.text.Match;

/**
 * @author  alno
 */
public class TextMatchesContentProvider implements ITreeContentProvider, DocumentHolder {

	private static final Object[] EMPTY_ARRAY = new Object[0];

	/**
	 * @uml.property  name="document"
	 * @uml.associationEnd  
	 */
	private Document document;
	
	/**
	 * @uml.property  name="pattern"
	 * @uml.associationEnd  
	 */
	private Pattern pattern = null;

	@Override
	public Object[] getChildren( Object obj ) {
		if ( document == null || obj == null )
			return EMPTY_ARRAY;

		if ( obj instanceof Pattern )
			return document.getAnalyzedText().getMatches( (Pattern) obj ).toArray();

		return EMPTY_ARRAY;
	}

	@Override
	public Object getParent( Object obj ) {
		if ( obj instanceof Match )
			return ((Match) obj).pattern;

		return null;
	}

	@Override
	public boolean hasChildren( Object obj ) {
		if ( document == null || obj == null )
			return false;
		
		if ( obj instanceof Pattern )
			return document.getAnalyzedText().getMatchCount( (Pattern) obj ) > 0;

		return false;
	}

	@Override
	public Object[] getElements( Object input ) {
		if ( document == null || input == null || pattern == null )
			return EMPTY_ARRAY;

		return document.getAnalyzedText().getMatches( pattern ).toArray();
	}

	@Override
	public void dispose() {
	}

	@Override
	public void inputChanged( Viewer viewer, Object arg1, Object arg2 ) {		
		document = (Document) arg2;
	}

	/**
	 * @return
	 * @uml.property  name="document"
	 */
	@Override
	public Document getDocument() {
		return document;
	}

	/**
	 * @param doc
	 * @uml.property  name="document"
	 */
	@Override
	public void setDocument( Document doc ) {
		document = doc;
	}

	/**
	 * @return
	 * @uml.property  name="pattern"
	 */
	public Pattern getPattern() {
		return pattern;
	}

	/**
	 * @param selectedPattern
	 * @uml.property  name="pattern"
	 */
	public void setPattern( Pattern selectedPattern ) {
		this.pattern = selectedPattern;
	}

}
