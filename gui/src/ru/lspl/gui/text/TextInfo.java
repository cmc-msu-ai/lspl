package ru.lspl.gui.text;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentListenerAdapter;

/**
 * @author  alno
 */
public class TextInfo extends Composite {
	
	/**
	 * @uml.property  name="document"
	 * @uml.associationEnd  
	 */
	private Document document = null;
	private Text infoArea = null;
	
	/**
	 * @uml.property  name="documentListener"
	 * @uml.associationEnd  
	 */
	private DocumentListenerAdapter documentListener = new DocumentListenerAdapter() {
		
		@Override
		public void documentAnalyzed( Document document ) {
			infoArea.setText( document.getAnalyzedText().dump() );
		}
	};

	public TextInfo(Composite parent, int style) {
		super(parent, style);
		initialize();
	}

	private void initialize() {
		infoArea = new org.eclipse.swt.widgets.Text(this, SWT.MULTI | SWT.WRAP | SWT.V_SCROLL );
		infoArea.setEditable( false );
		setSize(new Point(300, 200));
		setLayout(new FillLayout());
	}

	/**
	 * @param doc
	 * @uml.property  name="document"
	 */
	public void setDocument( Document doc ) {
		if ( document != null )
			document.removeListener( documentListener );
		
		document = doc;
		document.addListener( documentListener );
		infoArea.setText( document.getAnalyzedText().dump() );
	}

}
