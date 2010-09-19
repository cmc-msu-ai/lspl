package ru.lspl.gui.text;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;

import org.eclipse.jface.viewers.DoubleClickEvent;
import org.eclipse.jface.viewers.IDoubleClickListener;
import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;

import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentListenerAdapter;
import ru.lspl.gui.text.listeners.IMatchSelectionListener;
import ru.lspl.gui.text.viewers.TextMatchesContentProvider;
import ru.lspl.gui.text.viewers.TextMatchesLabelProvider;
import ru.lspl.patterns.Pattern;
import ru.lspl.text.Match;
import ru.lspl.text.Text;

/**
 * @author  alno
 */
public class TextMatchesViewer extends Composite {

	private Document document = null;

	private Pattern pattern = null;

	private Tree matchesTree = null;
	private TreeViewer matchesViewer = null;

	private final Collection<IMatchSelectionListener> textMatchListeners = new ArrayList<IMatchSelectionListener>();

	/**
	 * @uml.property  name="documentListener"
	 * @uml.associationEnd  
	 */
	private DocumentListenerAdapter documentListener = new DocumentListenerAdapter() {

		@Override
		public void documentAnalyzed( Document document ) {
			matchesViewer.setInput( document );
			updateStats();
		}
	};

	public TextMatchesViewer( Composite parent, int style ) {
		super( parent, style );
		initialize();
	}

	private void initialize() {
		setLayout( new FillLayout() );

		createMatchesTree(); // Создаем дерево
		createMatchesViewer(); // Создаем просмотрщик для дерева
	}

	private void createMatchesTree() {
		matchesTree = new Tree( this, SWT.BORDER );
		matchesTree.setHeaderVisible( true );
		matchesTree.setLinesVisible( true );

		/*
		 * Создаем колонки
		 */
		TreeColumn tc1 = new TreeColumn( matchesTree, SWT.LEFT );
		tc1.setText( "Текст" );
		tc1.setWidth( 300 );
		TreeColumn tc2 = new TreeColumn( matchesTree, SWT.LEFT );
		tc2.setText( "Параметры" );
		tc2.setWidth( 200 );
	}

	private void createMatchesViewer() {
		matchesViewer = new TreeViewer( matchesTree );
		matchesViewer.setContentProvider( new TextMatchesContentProvider() );
		matchesViewer.setLabelProvider( new TextMatchesLabelProvider() );
		matchesViewer.addSelectionChangedListener( new ISelectionChangedListener() {
			@Override
			public void selectionChanged( SelectionChangedEvent ev ) {
				Iterator<?> iter = ((IStructuredSelection) ev.getSelection()).iterator();

				while ( iter.hasNext() ) {
					Object obj = iter.next();

					if ( obj instanceof Text ) {
						showTextInfo( (Text) obj );

						for ( IMatchSelectionListener listener : textMatchListeners )
							listener.textSelect( (Text) obj );
					} else if ( obj instanceof Pattern ) {
						showPatternInfo( (Pattern) obj );

						for ( IMatchSelectionListener listener : textMatchListeners )
							listener.patternSelect( (Pattern) obj );
					} else if ( obj instanceof Match ) {
						showMatchInfo( (Match) obj );
						for ( IMatchSelectionListener listener : textMatchListeners )
							listener.matchSelect( (Match) obj );
					}
				}
			}
		} );
		matchesViewer.addDoubleClickListener( new IDoubleClickListener() {

			@Override
			public void doubleClick( DoubleClickEvent ev ) {
				Iterator<?> iter = ((IStructuredSelection) ev.getSelection()).iterator();

				while ( iter.hasNext() ) {
					Object obj = iter.next();

					if ( obj instanceof Text ) {
						for ( IMatchSelectionListener listener : textMatchListeners )
							listener.textDoubleClick( (Text) obj );
					}
					if ( obj instanceof Pattern ) {
						for ( IMatchSelectionListener listener : textMatchListeners )
							listener.patternDoubleClick( (Pattern) obj );
					} else if ( obj instanceof Match ) {
						for ( IMatchSelectionListener listener : textMatchListeners )
							listener.matchDoubleClick( (Match) obj );
					}
				}
			}
		} );
		matchesViewer.setInput( document );
	}

	private void showTextInfo( Text obj ) {
		// TODO Auto-generated method stub

	}

	private void showMatchInfo( Match obj ) {
		// TODO Auto-generated method stub

	}

	private void showPatternInfo( Pattern obj ) {
		// TODO Auto-generated method stub

	}

	public void addMatchSelectionListener( IMatchSelectionListener listener ) {
		textMatchListeners.add( listener );
	}

	public void removeMatchSelectionListener( IMatchSelectionListener listener ) {
		textMatchListeners.remove( listener );
	}

	private void updateStats() {

		/*
		 * ArrayList<Match> matches = new ArrayList<Match>(); for ( Pattern
		 * pattern : (Pattern[])patternsViewer.getInput()) for ( Match m :
		 * document.getAnalyzedText().getMatches( pattern ) ) matches.add( m );
		 * 
		 * Collections.sort( matches, TransitionStartComparator.INSTANCE );
		 * 
		 * int overMatches = 0;
		 * 
		 * for ( int i = 0; i < matches.size() - 1; ++ i ) { if (
		 * matches.get(i).end.startOffset > matches.get(i+1).start.endOffset )
		 * overMatches ++; }
		 * 
		 * matchesInfo.setText("Сопоставлений: " + matches.size() +
		 * "\nПерекрывающихся сопоставлений: " + overMatches );
		 */
	}

	public void setDocument( Document doc ) {
		if ( document != null )
			document.removeListener( documentListener );

		document = doc;
		document.addListener( documentListener );
		matchesViewer.setInput( document );
		updateStats();
	}

	public Pattern getPattern() {
		return pattern;
	}

	public void setPattern( Pattern selectedPattern ) {
		this.pattern = selectedPattern;

		((TextMatchesContentProvider) matchesViewer.getContentProvider()).setPattern( selectedPattern );
		matchesViewer.refresh();
	}
}
