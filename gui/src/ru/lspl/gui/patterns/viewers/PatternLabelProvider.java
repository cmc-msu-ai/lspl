package ru.lspl.gui.patterns.viewers;

import org.eclipse.jface.viewers.BaseLabelProvider;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.swt.graphics.Image;

import ru.lspl.patterns.Pattern;
import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentHolder;
import ru.lspl.patterns.Alternative;

/**
 * @author  alno
 */
public class PatternLabelProvider extends BaseLabelProvider implements ITableLabelProvider, DocumentHolder {
	
	/**
	 * @uml.property  name="document"
	 * @uml.associationEnd  
	 */
	private Document document;
	
	private static final int COLUMN_PATTERN = 0;
	private static final int COLUMN_PARAMS = 1;
	private static final int COLUMN_MATCHES = 2;

	@Override
	public String getColumnText( Object obj, int column ) {
		switch ( column ) {
		case COLUMN_PATTERN:
			if ( obj instanceof Pattern)
				return ((Pattern) obj).name;
			
			if ( obj instanceof Alternative )
				return ((Alternative) obj).getSource();
			
			break;
		case COLUMN_PARAMS:
			break;
		case COLUMN_MATCHES:
			if ( obj instanceof Pattern) // Возвращаем количество сопоставлений
				return String.valueOf( document.getAnalyzedText().getMatches( (Pattern )obj ).size() );
			
			if ( obj instanceof Alternative )
				return "";
			
			break;
		}

		return "<Unknown>";
	}

	@Override
	public Image getColumnImage( Object arg0, int arg1 ) {
		return null;
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
}
