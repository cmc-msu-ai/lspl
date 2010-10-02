package ru.lspl.gui.text.viewers;

import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.Viewer;

import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentHolder;
import ru.lspl.gui.utils.RangeMatchGroup;
import ru.lspl.gui.utils.RangeMatchGroupper;
import ru.lspl.patterns.Pattern;

/**
 * @author alno
 */
public class TextMatchesContentProvider implements ITreeContentProvider, DocumentHolder {

	private static final Object[] EMPTY_ARRAY = new Object[0];

	private static final RangeMatchGroupper groupper = new RangeMatchGroupper();

	private Document document;

	private Pattern pattern = null;

	@Override
	public Object[] getChildren( Object obj ) {
		if ( document == null || obj == null )
			return EMPTY_ARRAY;

		if ( obj instanceof RangeMatchGroup )
			return ((RangeMatchGroup) obj).matches.toArray();

		if ( obj instanceof Pattern )
			return groupper.groupMatches( document.getAnalyzedText().getMatches( (Pattern) obj ) ).toArray();

		return EMPTY_ARRAY;
	}

	@Override
	public Object getParent( Object obj ) {
		//if ( obj instanceof Match )
		//	return ((Match) obj).pattern;

		return null;
	}

	@Override
	public boolean hasChildren( Object obj ) {
		if ( document == null || obj == null )
			return false;

		if ( obj instanceof RangeMatchGroup )
			return ((RangeMatchGroup) obj).matches.size() > 1;

		if ( obj instanceof Pattern )
			return document.getAnalyzedText().getMatches( (Pattern) obj ).size() > 0;

		return false;
	}

	@Override
	public Object[] getElements( Object input ) {
		if ( document == null || input == null || pattern == null )
			return EMPTY_ARRAY;

		return groupper.groupMatches( document.getAnalyzedText().getMatches( pattern ) ).toArray();
	}

	@Override
	public void dispose() {
	}

	@Override
	public void inputChanged( Viewer viewer, Object arg1, Object arg2 ) {
		document = (Document) arg2;
	}

	@Override
	public Document getDocument() {
		return document;
	}

	@Override
	public void setDocument( Document doc ) {
		document = doc;
	}

	public Pattern getPattern() {
		return pattern;
	}

	public void setPattern( Pattern selectedPattern ) {
		this.pattern = selectedPattern;
	}

}
