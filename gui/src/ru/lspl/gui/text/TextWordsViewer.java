package ru.lspl.gui.text;

import java.util.ArrayList;
import java.util.Iterator;

import org.eclipse.jface.viewers.ComboViewer;
import org.eclipse.jface.viewers.DoubleClickEvent;
import org.eclipse.jface.viewers.IDoubleClickListener;
import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.StructuredSelection;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;

import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentListenerAdapter;
import ru.lspl.gui.text.listeners.IWordSelectionListener;
import ru.lspl.gui.text.viewers.SpeechPartLabelProvider;
import ru.lspl.gui.text.viewers.TextWordsLabelProvider;
import ru.lspl.gui.text.viewers.TextWordsProvider;
import ru.lspl.gui.utils.SimpleContentProvider;
import ru.lspl.text.Node;
import ru.lspl.text.Word;
import ru.lspl.text.attributes.SpeechPart;

/**
 * @author  alno
 */
public class TextWordsViewer extends Composite {

	/**
	 * @uml.property  name="document"
	 * @uml.associationEnd  
	 */
	private Document document = null;

	private Tree wordsTree = null;
	private TreeViewer wordsViewer = null;

	private Combo spCombo = null;
	private ComboViewer spViewer = null;

	/**
	 * @uml.property  name="documentListener"
	 * @uml.associationEnd  
	 */
	private DocumentListenerAdapter documentListener = new DocumentListenerAdapter() {

		@Override
		public void documentAnalyzed( Document document ) {
			wordsViewer.setInput( document );
		}
	};

	private final ArrayList<IWordSelectionListener> wordSelectionListeners = new ArrayList<IWordSelectionListener>();

	public TextWordsViewer( Composite parent, int style ) {
		super( parent, style );
		initialize();
	}

	private void initialize() {
		GridLayout gridLayout = new GridLayout();
		gridLayout.numColumns = 1;
		this.setLayout( gridLayout );

		createSpCombo();
		createWordsTree();
		createWordsViewer();
	}

	private void createSpCombo() {
		GridData gd = new GridData();
		gd.horizontalAlignment = GridData.FILL;
		gd.verticalAlignment = GridData.CENTER;
		spCombo = new Combo( this, SWT.NONE );
		spCombo.setLayoutData( gd );
		spViewer = new ComboViewer( spCombo );
		spViewer.setContentProvider( new SimpleContentProvider() );
		spViewer.setLabelProvider( new SpeechPartLabelProvider() );
		spViewer.setInput( SpeechPart.values() );
		spViewer.setSelection( new StructuredSelection( SpeechPart.ANY ) );
		spViewer.addSelectionChangedListener( new ISelectionChangedListener() {

			@Override
			public void selectionChanged( SelectionChangedEvent ev ) {
				IStructuredSelection sel = (IStructuredSelection) ev.getSelection();

				wordsViewer.setContentProvider( new TextWordsProvider( (SpeechPart) sel.getFirstElement() ) );
			}
		} );
	}

	private void createWordsTree() {
		GridData gd = new GridData();
		gd.horizontalAlignment = GridData.FILL;
		gd.grabExcessHorizontalSpace = true;
		gd.grabExcessVerticalSpace = true;
		gd.verticalAlignment = GridData.FILL;

		wordsTree = new Tree( this, SWT.BORDER );
		wordsTree.setLayoutData( gd );
		wordsTree.setHeaderVisible( true );
		wordsTree.setLinesVisible( true );

		/*
		 * Создаем колонки
		 */
		TreeColumn tc1 = new TreeColumn( wordsTree, SWT.LEFT );
		tc1.setText( "Слово" );
		tc1.setWidth( 300 );
		TreeColumn tc2 = new TreeColumn( wordsTree, SWT.LEFT );
		tc2.setText( "Параметры" );
		tc2.setWidth( 200 );
	}

	private void createWordsViewer() {
		wordsViewer = new TreeViewer( wordsTree );
		wordsViewer.setContentProvider( new TextWordsProvider( SpeechPart.ANY ) );
		wordsViewer.setLabelProvider( new TextWordsLabelProvider() );
		wordsViewer.addSelectionChangedListener( new ISelectionChangedListener() {

			@Override
			public void selectionChanged( SelectionChangedEvent ev ) {
				Iterator<?> iter = ((IStructuredSelection) ev.getSelection()).iterator();

				while ( iter.hasNext() ) {
					Object o = iter.next();

					if ( o instanceof Word )
						for ( IWordSelectionListener listener : wordSelectionListeners )
							listener.wordSelect( (Word) o );

					if ( o instanceof Node )
						for ( IWordSelectionListener listener : wordSelectionListeners )
							listener.nodeSelect( (Node) o );
				}
			}
		} );
		wordsViewer.addDoubleClickListener( new IDoubleClickListener() {

			@Override
			public void doubleClick( DoubleClickEvent ev ) {
				Iterator<?> iter = ((IStructuredSelection) ev.getSelection()).iterator();

				while ( iter.hasNext() ) {
					Object o = iter.next();

					if ( o instanceof Word )
						for ( IWordSelectionListener listener : wordSelectionListeners )
							listener.wordDoubleClick( (Word) o );

					if ( o instanceof Node )
						for ( IWordSelectionListener listener : wordSelectionListeners )
							listener.nodeDoubleClick( (Node) o );
				}

			}
		} );
	}

	public void addWordSelectionListener( IWordSelectionListener listener ) {
		wordSelectionListeners.add( listener );
	}

	public void removeWordSelectionListener( IWordSelectionListener listener ) {
		wordSelectionListeners.remove( listener );
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

		wordsViewer.setInput( document );
	}
}
