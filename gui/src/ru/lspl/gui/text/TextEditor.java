package ru.lspl.gui.text;

import java.util.ArrayList;
import java.util.Collection;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyleRange;
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

import com.onpositive.richtexteditor.viewer.RichTextViewer;

/**
 * @author  alno
 */
public class TextEditor extends Composite implements DocumentHolder {

	private static final Transition[] NO_TRANSITIONS = new Transition[0];
	
	private final RichTextViewer textViewer;

	private final StyleRangeBuilder rangeBuilder;

	private Transition[] marks = NO_TRANSITIONS;
	
	private final ArrayList<ITextSelectionListener> selectionListeners = new ArrayList<ITextSelectionListener>();
	
	/**
	 * @uml.property  name="document"
	 * @uml.associationEnd  
	 */
	private Document document = null;
	
	private DocumentListenerAdapter documentListener = new DocumentListenerAdapter() {
		
	};
	
	/**
	 * @param doc
	 */
	@Override
	public void setDocument( Document doc ) {
		if ( document != null )
			document.removeListener( documentListener );
		
		document = doc;
		document.addListener( documentListener );
	}
	
	/**
	 * @return
	 */
	@Override
	public Document getDocument() {
		return document;
	}

	public TextEditor(Composite parent, int style) {
		super(parent, style);
		
		textViewer = new RichTextViewer( this, SWT.BORDER | SWT.V_SCROLL | SWT.H_SCROLL | SWT.WRAP );		
		textViewer.getTextWidget().addMouseMoveListener(new MouseMoveListener(){

			@Override
			public void mouseMove(MouseEvent ev) {
				int offset = -1;
				
				try {
					offset = textViewer.getTextWidget().getOffsetAtLocation(new Point(ev.x,ev.y));
				} catch ( Throwable e ) {}
				
				String text = textViewer.getTextWidget().getText();
				
				for ( ITextSelectionListener listener : selectionListeners )
					listener.positionOver( text, offset );				
			}
		});
		
		textViewer.getTextWidget().addModifyListener( new ModifyListener() {

			@Override
			public void modifyText( ModifyEvent ev ) {
				document.setSourceText( textViewer.getTextWidget().getText() );
			}		
			
		});
		
		rangeBuilder = new StyleRangeBuilder( getDisplay() );
		
		setLayout(new FillLayout());
		setSize(300, 200);
	}

	public void setText( String text ) {
		textViewer.getTextWidget().setText( text );
		textViewer.getTextWidget().replaceStyleRanges(0, text.length(), new StyleRange[0 ]);
		marks = NO_TRANSITIONS;
	}
	
	public String getText() {
		return textViewer.getTextWidget().getText();
	}
	
	public void addSelectionListener( ITextSelectionListener listener ) {
		selectionListeners.add( listener );
	}
	
	public void removeSelectionListener( ITextSelectionListener listener ) {
		selectionListeners.remove( listener );
	}
	
	public void mark(Transition transition) {
		marks = new Transition[]{ transition };
		textViewer.getTextWidget().replaceStyleRanges(0, textViewer.getTextWidget().getText().length(), rangeBuilder.buildRanges( marks ));
	}
	
	public void mark(Transition[] transitions) {
		marks = transitions;
		textViewer.getTextWidget().replaceStyleRanges(0, textViewer.getTextWidget().getText().length(), rangeBuilder.buildRanges( marks ));
	}
	
	public void mark(Collection<? extends Transition> transitions) {
		marks = transitions.toArray( new Transition[ transitions.size() ]);
		textViewer.getTextWidget().replaceStyleRanges(0, textViewer.getTextWidget().getText().length(), rangeBuilder.buildRanges( marks ));
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
		
		textViewer.getTextWidget().setText( document.getSourceText() );
	}

	public void select(Transition transition) {
		textViewer.getTextWidget().setSelection( transition.start.endOffset, transition.end.startOffset );
		textViewer.getTextWidget().showSelection();		
	}
}
