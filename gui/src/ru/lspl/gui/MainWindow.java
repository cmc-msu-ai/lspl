package ru.lspl.gui;

import java.util.Set;

import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.jface.action.Separator;
import org.eclipse.jface.window.ApplicationWindow;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.SashForm;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.FileDialog;

import ru.lspl.patterns.Pattern;
import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentListener;
import ru.lspl.gui.model.DocumentListenerAdapter;
import ru.lspl.gui.patterns.viewers.PatternListener;
import ru.lspl.gui.text.TextEditor;
import ru.lspl.gui.text.listeners.MatchSelectionListener;
import ru.lspl.gui.text.listeners.TextSelectionListener;
import ru.lspl.gui.text.listeners.WordSelectionListener;
import ru.lspl.text.Match;
import ru.lspl.text.Node;
import ru.lspl.text.Text;
import ru.lspl.text.Transition;
import ru.lspl.text.Word;

/**
 * @author  alno
 */
public class MainWindow extends ApplicationWindow {
	
	private static final String WINDOW_TITLE = "Lspl Text Analyzer";
	/**
	 * @uml.property  name="nO_TRANSITIONS"
	 * @uml.associationEnd  multiplicity="(0 -1)"
	 */
	private static final Transition[] NO_TRANSITIONS = new Transition[0];

	private final MenuManager textMenu = new MenuManager( "&Текст", "textMenu" );
	private final MenuManager patternsMenu = new MenuManager( "&Шаблоны", "patternsMenu" );
	private final MenuManager analyzeMenu = new MenuManager( "&Анализ", "analyzeMenu" );
	private final MenuManager viewMenu = new MenuManager( "&Вид", "viewMenu" );
	
	/**
	 * @uml.property  name="lastSelection"
	 * @uml.associationEnd  multiplicity="(0 -1)"
	 */
	private Transition[] lastSelection = NO_TRANSITIONS;
	/**
	 * @uml.property  name="transitions"
	 * @uml.associationEnd  multiplicity="(0 -1)"
	 */
	private Transition[] transitions = NO_TRANSITIONS;

	/**
	 * @uml.property  name="textEditor"
	 * @uml.associationEnd  
	 */
	private TextEditor textEditor = null;
	/**
	 * @uml.property  name="infoPanel"
	 * @uml.associationEnd  
	 */
	private InfoPanel infoPanel = null;
	/**
	 * @uml.property  name="editPanel"
	 * @uml.associationEnd  
	 */
	private EditPanel editPanel = null;

	private int infoSelectionIndex = -1;

	private FileDialog openFileDialog = null;
	private FileDialog saveFileDialog = null;
	
	/**
	 * @uml.property  name="document"
	 * @uml.associationEnd  
	 */
	private Document document = new Document();
	
	private final Action clearFileAction = new Action("&Новый") {
		
		@Override
		public void run() {
			document.clear();
			document.analyze();
			textEditor.update();
			
			transitions = NO_TRANSITIONS;
			lastSelection = NO_TRANSITIONS;
			updateTitle();
		}
	};
	
	private final Action openTextAction = new Action("&Открыть...") {
		
		@Override
		public void run() {
			try {
				String fileName = openFileDialog.open();
				
				if ( fileName == null )
					return;
				
				document.load( fileName );
				textEditor.update();
			} catch (Throwable e) {
				e.printStackTrace();
			}
			
			transitions = NO_TRANSITIONS;
			lastSelection = NO_TRANSITIONS;
			updateTitle();
		}
	};
	
	private final Action saveTextAction = new Action("&Сохранить") {
		
		@Override
		public void run() {
			try {
				if (document.hasSavedFileName())
					document.save();
				else {
					String fileName = saveFileDialog.open();

					if (fileName != null)
						document.save(fileName);
				}
				
				updateTitle();
			} catch (Throwable e) {
				e.printStackTrace();
			}
		}
	};
	
	private final Action saveAsTextAction = new Action("Сохранить как...") {
		
		@Override
		public void run() {
			try {
				String fileName = saveFileDialog.open();

				if (fileName != null)
					document.save(fileName);
				
				updateTitle();
			} catch (Throwable e) {
				e.printStackTrace();
			}
		}
	};
	
	private final Action analyzeTextAction = new Action("&Анализировать") {
		
		@Override
		public void run() {
			document.analyze();
		}
	};
	
	private final Action clearPatternsAction = new Action("Очистить") {
		
		@Override
		public void run() {
			editPanel.patternEditor.clear();
		}
	};
	
	private final Action importPatternsAction = new Action("Загрузить...") {
		
		@Override
		public void run() {
			try {
				String fileName = openFileDialog.open();
				
				if ( fileName == null )
					return;
				
				editPanel.patternEditor.load(fileName);
			} catch (Throwable e) {
				e.printStackTrace();
			}
		}
	};
	
	private final Action exportPatternsAction = new Action("Сохранить..") {
		
		@Override
		public void run() {
			try {
				String fileName = saveFileDialog.open();

				if (fileName != null)
					editPanel.patternEditor.save(fileName);
			} catch (Throwable e) {
				e.printStackTrace();
			}
		}
	};
 
	/**
	 * @uml.property  name="patternListener"
	 * @uml.associationEnd  
	 */
	private final PatternListener patternListener = new PatternListener() {

		@Override
		public void patternSelect(Pattern pattern) {
			infoPanel.patternInfo.setPattern(pattern); // @jve:decl-index=0:			
			infoPanel.matches.setPattern( pattern );
		}
		
		@Override
		public void patternChecked( Pattern pattern, boolean checked, Set<Pattern> checkedPatterns ) {
			textEditor.mark(document.getAnalyzedText().getMatches(checkedPatterns));
		}

		@Override
		public void patternDoubleClick(Pattern pattern) {
			infoPanel.showPatternInfoAction.run();
		}
	};
	
	/**
	 * @uml.property  name="textSelectionListener"
	 * @uml.associationEnd  
	 */
	private final TextSelectionListener textSelectionListener = new TextSelectionListener() {

		@Override
		public void positionOver(String text, int position) {
			if (position < 0) {
				if (lastSelection != NO_TRANSITIONS)
					textEditor.mark(lastSelection);

				if (infoSelectionIndex >= 0)
					infoPanel.setSelection(infoSelectionIndex);

				lastSelection = NO_TRANSITIONS;
				infoSelectionIndex = -1;
			} else {
				if (lastSelection == NO_TRANSITIONS)
					lastSelection = textEditor.getMarks();

				if (infoSelectionIndex < 0)
					infoSelectionIndex = infoPanel.getSelectionIndex();

				Transition[] selectedArray = Transition.selectIncludesPosition( transitions, position );

				textEditor.mark(selectedArray);
				infoPanel.transitionInfo.setTransitions(selectedArray);
				infoPanel.showTransitionInfoAction.run();
			}
		}

	};
	
	/**
	 * @uml.property  name="wordSelectionListener"
	 * @uml.associationEnd  
	 */
	private final WordSelectionListener wordSelectionListener = new WordSelectionListener() {

		@Override
		public void wordSelect(Word word) {
			infoPanel.transitionInfo.setTransitions(word);
			textEditor.mark(word);
		}
		
		@Override
		public void wordDoubleClick(Word word) {
			textEditor.select(word);
		}
		
		@Override
		public void nodeSelect(Node node) {
			Transition[] transitions = node.getWordsArray();
			
			infoPanel.transitionInfo.setTransitions(transitions);
			textEditor.mark(transitions);
		}

		@Override
		public void nodeDoubleClick(Node node) {
			for ( Transition t : node.transitions )
				if ( t instanceof Word ) {
					textEditor.select( t );
					return;
				}
		}

	};
	
	/**
	 * @uml.property  name="matchSelectionListener"
	 * @uml.associationEnd  
	 */
	private final MatchSelectionListener matchSelectionListener = new MatchSelectionListener() {

		@Override
		public void textSelect(Text arg) {
			//textEditor.mark(document.getAnalyzedText().getMatches(document.getPatternList()));
		}

		@Override
		public void matchSelect(Match match) {
			infoPanel.transitionInfo.setTransitions(match);
			//textEditor.mark(match);
		}
		
		@Override
		public void matchDoubleClick(Match match) {
			textEditor.select(match);
		}

		@Override
		public void patternSelect(Pattern pattern) {
			//textEditor.mark(document.getAnalyzedText().getMatches(pattern));
		}

	};
	
	/**
	 * @uml.property  name="documentListener"
	 * @uml.associationEnd  
	 */
	private final DocumentListener documentListener = new DocumentListenerAdapter() {

		@Override
		public void documentAnalyzed( Document doc ) {
			transitions = document.getAnalyzedText().getTransitionsArray();
			
			if ( editPanel.patternEditor.isVisible() ) {				
				textEditor.mark(document.getAnalyzedText().getMatches(editPanel.patternEditor.getCheckedPatterns()));
			}
		}
		
	};
	
	public static void main(String[] args) {
		MainWindow window = new MainWindow(); // Создаем наше окно
		window.setBlockOnOpen( true ); // Устанавливаем флаг - ждать закрытия окна
		window.open(); // Открываем окно

		Display.getCurrent().dispose(); // Освобождаем ресурсы
	}

	public MainWindow() {
		super( null );
		
		addMenuBar(); // Добавляем к окну панель меню
		addStatusLine(); // Добавляем к окну строку статуса
		
		document.addListener( documentListener );
	}

	/**
	 * Создать содержимое окна
	 */
	@Override
	protected Control createContents( Composite parent ) {
		getShell().setText( WINDOW_TITLE ); // Устанавливаем заголовок окна
		
		createDialogs(); // Создаем диалоги открытия и сохранения файла
		
		SashForm sfVertical = new SashForm( parent, SWT.NONE);
		sfVertical.setOrientation(SWT.VERTICAL);
		SashForm sfHorizontal = new SashForm(sfVertical, SWT.NONE);
		sfHorizontal.setOrientation(SWT.HORIZONTAL);
		createTextEditor( sfHorizontal );
		createEditPanel( sfHorizontal );
		createInfoPanel( sfVertical );

		createMenuBar(); // Заполняем меню

		return sfVertical;
	}
	
	/**
	 * Создать содержимое меню в окне
	 */
	private void createMenuBar() {
		getMenuBarManager().add( textMenu ); // Добавляем меню "Teкст"
		textMenu.add( clearFileAction );
		textMenu.add( openTextAction );
		textMenu.add( new Separator() );
		textMenu.add( saveTextAction );
		textMenu.add( saveAsTextAction );		

		getMenuBarManager().add( patternsMenu ); // Добавляем меню "Шаблоны"
		patternsMenu.add( clearPatternsAction );
		patternsMenu.add( importPatternsAction );
		patternsMenu.add( new Separator() );
		patternsMenu.add( exportPatternsAction );
		
		getMenuBarManager().add( analyzeMenu ); // Добавляем меню "Анализ"
		analyzeMenu.add( analyzeTextAction );		

		getMenuBarManager().add( viewMenu ); // Добавляем меню "Вид"
		viewMenu.add( infoPanel.showPatternInfoAction );
		viewMenu.add( infoPanel.showTextInfoAction );
		viewMenu.add( infoPanel.showTransitionInfoAction );
		viewMenu.add( infoPanel.showMatchesAction );
		viewMenu.add( infoPanel.showWordsAction );
		viewMenu.add( new Separator() );
		viewMenu.add( editPanel.showPatternsAction );		
		viewMenu.add( editPanel.showConfigAction );
		
		getMenuBarManager().update( true ); // Обновляем содержимое меню
	}

	/**
	 * Создать диалоги открытия и сохранения файлов
	 */
	private void createDialogs() {
		openFileDialog = new FileDialog(getShell());
		openFileDialog.setText("Открыть файл");
		
		saveFileDialog = new FileDialog(getShell());
		saveFileDialog.setText("Сохранить файл");
	}	

	/**
	 * Создать редактор текста
	 * @param parent родительский композит
	 */
	private void createTextEditor(Composite parent) {
		textEditor = new TextEditor( parent, SWT.NONE );
		textEditor.addSelectionListener( textSelectionListener );
		textEditor.setDocument( document );
	}

	/**
	 * Создать панель информации
	 * @param parent родительский композит
	 */
	private void createInfoPanel( Composite parent ) {
		infoPanel = new InfoPanel( parent, SWT.NONE );
		infoPanel.setDocument( document );
		infoPanel.matches.addMatchSelectionListener( matchSelectionListener );
		infoPanel.words.addWordSelectionListener( wordSelectionListener );	
	}

	/**
	 * Создать панель списков
	 * @param parent родительский композит
	 */
	private void createEditPanel( Composite parent ) {
		editPanel = new EditPanel( parent, SWT.NONE );
		editPanel.setDocument( document );
		editPanel.patternEditor.addPatternListener( patternListener );
	}
	
	private void updateTitle() {
		if ( document.hasSavedFileName() ) {
			getShell().setText( WINDOW_TITLE + " - " + document.getSavedFileName() );
		} else {
			getShell().setText( WINDOW_TITLE );
		}		
	}
}
