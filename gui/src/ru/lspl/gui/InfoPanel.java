package ru.lspl.gui;

import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;

import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentHolder;
import ru.lspl.gui.patterns.PatternInfo;
import ru.lspl.gui.text.TextInfo;
import ru.lspl.gui.text.TextMatchesViewer;
import ru.lspl.gui.text.TextWordsViewer;
import ru.lspl.gui.text.TransitionInfo;

/**
 * @author  alno
 */
public class InfoPanel extends Composite implements DocumentHolder{
	
	public final Action showPatternInfoAction = new Action("Инфо ш&аблона") {
		
		@Override
		public void run() {
			if ( tiPatternInfo == null || tiPatternInfo.isDisposed()  ) {
				tiPatternInfo = new CTabItem(tabFolder, SWT.CLOSE);
				tiPatternInfo.setControl( patternInfo );
				tiPatternInfo.setText( showPatternInfoAction.getText() );
			}
			
			tabFolder.setSelection( tiPatternInfo );
		}
	};
	
	public final Action showTextInfoAction = new Action("Инфо &текста") {
		
		@Override
		public void run() {
			if ( tiTextInfo == null || tiTextInfo.isDisposed() ) {
				tiTextInfo = new CTabItem(tabFolder, SWT.CLOSE);
				tiTextInfo.setControl( textInfo );
				tiTextInfo.setText( showTextInfoAction.getText() );
			}
			
			tabFolder.setSelection( tiTextInfo );
		}
	};
	
	public final Action showTransitionInfoAction = new Action("Инфо &фрагмента") {
		
		@Override
		public void run() {
			if ( tiTransitionInfo == null || tiTransitionInfo.isDisposed() ) {
				tiTransitionInfo = new CTabItem(tabFolder, SWT.CLOSE);
				tiTransitionInfo.setControl( transitionInfo );
				tiTransitionInfo.setText( showTransitionInfoAction.getText() );
			}
			
			tabFolder.setSelection( tiTransitionInfo );
		}
	};
	
	public final Action showMatchesAction = new Action("Сопоставления") {
		
		@Override
		public void run() {
			if ( tiMatches == null || tiMatches.isDisposed() ) {
				tiMatches = new CTabItem(tabFolder, SWT.CLOSE);
				tiMatches.setControl( matches );
				tiMatches.setText( showMatchesAction.getText() );
			}
			
			tabFolder.setSelection( tiMatches );
		}
	};
	
	public final Action showWordsAction = new Action("С&лова") {
		
		@Override
		public void run() {
			if ( tiWords == null || tiWords.isDisposed()  ) {
				tiWords = new CTabItem(tabFolder, SWT.CLOSE);
				tiWords.setControl( words );
				tiWords.setText( showWordsAction.getText() );
			}
			
			tabFolder.setSelection( tiWords );
		}
	};
	
	/**
	 * @uml.property  name="patternInfo"
	 * @uml.associationEnd  
	 */
	public final PatternInfo patternInfo;
	/**
	 * @uml.property  name="textInfo"
	 * @uml.associationEnd  
	 */
	public final TextInfo textInfo;
	/**
	 * @uml.property  name="transitionInfo"
	 * @uml.associationEnd  
	 */
	public final TransitionInfo transitionInfo;
	
	/**
	 * @uml.property  name="matches"
	 * @uml.associationEnd  
	 */
	public final TextMatchesViewer matches;
	/**
	 * @uml.property  name="words"
	 * @uml.associationEnd  
	 */
	public final TextWordsViewer words;

	private final Color bgColor;
	private final CTabFolder tabFolder;
	
	private CTabItem tiPatternInfo = null;
	private CTabItem tiTextInfo = null;
	private CTabItem tiTransitionInfo = null;
	
	private CTabItem tiMatches = null;
	private CTabItem tiWords = null;
	
	/**
	 * @uml.property  name="document"
	 * @uml.associationEnd  
	 */
	private Document document = null;

	public InfoPanel(Composite parent, int style) {
		super(parent, style);
		
		bgColor = new Color( getDisplay(), 255, 255, 255 );
		
		tabFolder = new CTabFolder(this, SWT.BORDER);
		
		/*
		 * Создаем компоненты
		 */
		patternInfo = new PatternInfo(tabFolder, SWT.NONE);
		patternInfo.setBackground( bgColor );
		textInfo = new TextInfo(tabFolder, SWT.NONE);
		textInfo.setBackground( bgColor );
		transitionInfo = new TransitionInfo(tabFolder, SWT.NONE);
		transitionInfo.setBackground( bgColor );
		matches = new TextMatchesViewer(tabFolder, SWT.NONE);
		matches.setBackground( bgColor );
		words = new TextWordsViewer(tabFolder,SWT.NONE);
		words.setBackground(bgColor );
		
		/*
		 * Устанавливаем лайоут
		 */
		setLayout(new FillLayout());
		
		/*
		 * Показываем вкладки
		 */
		showPatternInfoAction.run();
		showTextInfoAction.run();
		showTransitionInfoAction.run();
		showMatchesAction.run();
		showWordsAction.run();
		
		tiPatternInfo.dispose();
		tiTextInfo.dispose();
		
		tabFolder.setSelection( tiMatches );
	}
	
	public int getSelectionIndex() {
		return tabFolder.getSelectionIndex();
	}
	
	public void setSelection( int index ) {
		tabFolder.setSelection(index);
	}

	/**
	 * @param doc
	 * @uml.property  name="document"
	 */
	@Override
	public void setDocument( Document doc ) {
		document = doc;
		textInfo.setDocument( document );
		matches.setDocument( document );
		words.setDocument( document );
	}

	/**
	 * @return
	 * @uml.property  name="document"
	 */
	@Override
	public Document getDocument() {
		return document;
	}

	public void extendViewMenu( MenuManager viewMenu ) {
		viewMenu.add( showPatternInfoAction );
		viewMenu.add( showTextInfoAction );
		viewMenu.add( showTransitionInfoAction );
		viewMenu.add( showMatchesAction );
		viewMenu.add( showWordsAction );
	}
}
