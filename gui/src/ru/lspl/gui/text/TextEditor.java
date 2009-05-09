package ru.lspl.gui.text;

import java.util.ArrayList;
import java.util.Collection;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseMoveListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;

import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentHolder;
import ru.lspl.gui.model.DocumentListenerAdapter;
import ru.lspl.gui.text.listeners.ITextSelectionListener;
import ru.lspl.text.Transition;

/**
 * @author  alno
 */
public class TextEditor extends Composite implements DocumentHolder {

	/**
	 * @uml.property  name="nO_TRANSITIONS"
	 * @uml.associationEnd  multiplicity="(0 -1)"
	 */
	private static final Transition[] NO_TRANSITIONS = new Transition[0];
	
	private final StyledText styledText;
	/**
	 * @uml.property  name="rangeBuilder"
	 * @uml.associationEnd  
	 */
	private final StyleRangeBuilder rangeBuilder;
	
	/**
	 * @uml.property  name="marks"
	 * @uml.associationEnd  multiplicity="(0 -1)"
	 */
	private Transition[] marks = NO_TRANSITIONS;
	
	private final ArrayList<ITextSelectionListener> selectionListeners = new ArrayList<ITextSelectionListener>();
	
	/**
	 * @uml.property  name="document"
	 * @uml.associationEnd  
	 */
	private Document document = null;
	
	/**
	 * @uml.property  name="documentListener"
	 * @uml.associationEnd  
	 */
	private DocumentListenerAdapter documentListener = new DocumentListenerAdapter() {
		
	};
	
	/**
	 * @param doc
	 * @uml.property  name="document"
	 */
	public void setDocument( Document doc ) {
		if ( document != null )
			document.removeListener( documentListener );
		
		document = doc;
		document.addListener( documentListener );
	}
	
	/**
	 * @return
	 * @uml.property  name="document"
	 */
	public Document getDocument() {
		return document;
	}

	public TextEditor(Composite parent, int style) {
		super(parent, style);
		
		styledText = new StyledText(this, SWT.BORDER | SWT.V_SCROLL | SWT.H_SCROLL | SWT.WRAP );
		styledText.addMouseMoveListener(new MouseMoveListener(){

			@Override
			public void mouseMove(MouseEvent ev) {
				int offset = -1;
				
				try {
					offset = styledText.getOffsetAtLocation(new Point(ev.x,ev.y));
				} catch ( Throwable e ) {}
				
				String text = styledText.getText();
				
				for ( ITextSelectionListener listener : selectionListeners )
					listener.positionOver( text, offset );				
			}
		});
		
		styledText.addModifyListener( new ModifyListener() {

			@Override
			public void modifyText( ModifyEvent ev ) {
				document.setSourceText( styledText.getText() );
			}		
			
		});
		
		rangeBuilder = new StyleRangeBuilder( getDisplay() );
		
		setLayout(new FillLayout());
		setSize(300, 200);
	}

	public void setText( String text ) {
		styledText.setText( text );
		styledText.replaceStyleRanges(0, text.length(), new StyleRange[0 ]);
		marks = NO_TRANSITIONS;
	}
	
	public String getText() {
		return styledText.getText();
	}
	
	public void addSelectionListener( ITextSelectionListener listener ) {
		selectionListeners.add( listener );
	}
	
	public void removeSelectionListener( ITextSelectionListener listener ) {
		selectionListeners.remove( listener );
	}
	
	public void mark(Transition transition) {
		marks = new Transition[]{ transition };
		styledText.replaceStyleRanges(0, styledText.getText().length(), rangeBuilder.buildRanges( marks ));
	}
	
	public void mark(Transition[] transitions) {
		marks = transitions;
		styledText.replaceStyleRanges(0, styledText.getText().length(), rangeBuilder.buildRanges( marks ));
	}
	
	public void mark(Collection<? extends Transition> transitions) {
		marks = transitions.toArray( new Transition[ transitions.size() ]);
		styledText.replaceStyleRanges(0, styledText.getText().length(), rangeBuilder.buildRanges( marks ));
	}
	
	/**
	 * @return
	 * @uml.property  name="marks"
	 */
	public Transition[] getMarks() {
		return marks;
	}

	@Override
	public void update() {		
		super.update();
		
		styledText.setText( document.getSourceText() );
	}

	public void select(Transition transition) {
		styledText.setSelection( transition.start.endOffset, transition.end.startOffset );
		styledText.showSelection();		
	}
}
