package ru.lspl.gui.patterns;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Collection;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.Set;

import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.jface.action.Separator;
import org.eclipse.jface.viewers.CheckStateChangedEvent;
import org.eclipse.jface.viewers.CheckboxTreeViewer;
import org.eclipse.jface.viewers.DoubleClickEvent;
import org.eclipse.jface.viewers.ICheckStateListener;
import org.eclipse.jface.viewers.IDoubleClickListener;
import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.ITreeViewerListener;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.TreeExpansionEvent;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;

import ru.lspl.gui.model.Document;
import ru.lspl.gui.model.DocumentHolder;
import ru.lspl.gui.model.DocumentListener;
import ru.lspl.gui.patterns.viewers.DefinedPatternsProvider;
import ru.lspl.gui.patterns.viewers.IPatternListener;
import ru.lspl.gui.patterns.viewers.PatternLabelProvider;
import ru.lspl.patterns.Alternative;
import ru.lspl.patterns.Pattern;

/**
 * @author  alno
 */
public class PatternEditor extends Composite {
	
	private final FileDialog openFileDialog;
	private final FileDialog saveFileDialog;
	
	private final Action clearPatternsAction = new Action("Очистить") {
		
		@Override
		public void run() {
			clear();
		}
	};
	
	private final Action importPatternsAction = new Action("Загрузить...") {
		
		@Override
		public void run() {
			try {
				String fileName = openFileDialog.open();
				
				if ( fileName == null )
					return;
				
				load(fileName);
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
					save(fileName);
			} catch (Throwable e) {
				e.printStackTrace();
			}
		}
	};

	private Tree patternsTree = null;	
	private CheckboxTreeViewer patternsViewer = null;

	private Document document = null;
	
	private final Collection<IPatternListener> patternListeners = new LinkedList<IPatternListener>();

	private Text text = null;

	private Button button = null;

	private Pattern selectedPattern = null;

	private final Set<Pattern> checkedPatterns = new HashSet<Pattern>();

	private DocumentListener documentListener = new DocumentListener() {

		@Override
		public void documentAnalyzed( Document doc ) {
			patternsViewer.refresh();
		}

		@Override
		public void documentUpdated( Document doc ) {
			patternsViewer.refresh();
		}
		
	};

	public PatternEditor(Composite parent, int style) {
		super(parent, style);
		initialize();
		
		openFileDialog = new FileDialog(getShell());
		openFileDialog.setText("Открыть файл с шаблонами");
		
		saveFileDialog = new FileDialog(getShell());
		saveFileDialog.setText("Сохранить файл с шаблонами");
	}

	private void initialize() {		
		createPatternsTree(); // Создаем дерево для шаблонов
		createPatternsViewer(); // Создаем просмотрщик для шаблонов
		
		createDefineText();		
		createDefineButton();
		
		GridLayout gridLayout = new GridLayout();
		gridLayout.numColumns = 3;
		gridLayout.verticalSpacing = 5;
		gridLayout.horizontalSpacing = 5;
		this.setLayout(gridLayout);
		setSize(new Point(300, 200));
	}
	
	private void createDefineText() {
		GridData gridData1 = new GridData();
		gridData1.grabExcessHorizontalSpace = true;
		gridData1.verticalAlignment = GridData.CENTER;
		gridData1.horizontalAlignment = GridData.FILL;
		gridData1.heightHint = 50;
		gridData1.horizontalSpan = 2;
		text = new Text(this, SWT.BORDER | SWT.MULTI );
		text.setLayoutData(gridData1);
	}

	private void createDefineButton() {
		button = new Button(this, SWT.NONE);
		button.setText("Добавить");
		button.addSelectionListener(new org.eclipse.swt.events.SelectionAdapter() {
			
			@Override
			public void widgetSelected(org.eclipse.swt.events.SelectionEvent e) {
				if ( document == null ) 
					return;
				
				try {
					document.buildPattern( text.getText() );
				} catch ( Throwable ex ) {
					MessageBox mb = new MessageBox(getShell(), SWT.OK | SWT.ICON_ERROR);
			        mb.setText("Ошибка компиляции шаблона");
			        mb.setMessage(ex.getMessage());
			        mb.open();
				}
				
				update();
			}
		});
	}
	
	@Override
	public void update() {
		patternsViewer.setInput( document );
	}

	private void createPatternsTree() {
		patternsViewer = new CheckboxTreeViewer(this);
		
		GridData gridData = new GridData();
		gridData.grabExcessVerticalSpace = true;
		gridData.horizontalAlignment = GridData.FILL;
		gridData.verticalAlignment = GridData.FILL;
		gridData.horizontalSpan = 3;
		gridData.grabExcessHorizontalSpace = true;
		
		patternsTree = patternsViewer.getTree();
		patternsTree.setLayoutData(gridData);
		patternsTree.setHeaderVisible( true );
		patternsTree.setLinesVisible( true );
		
		TreeColumn tcPattern = new TreeColumn( patternsTree, SWT.LEFT );
		tcPattern.setText( "Шаблон" );
		tcPattern.setWidth( 150 );
		
		TreeColumn tcParams = new TreeColumn( patternsTree, SWT.LEFT );
		tcParams.setText( "Параметры" );
		tcParams.setWidth( 100 );
		
		TreeColumn tcMatches = new TreeColumn( patternsTree, SWT.LEFT );
		tcMatches.setText( "Сопоставлений" );
		tcMatches.setWidth( 30 );
		
	}
	
	private void createPatternsViewer() {
		patternsViewer.setLabelProvider(new PatternLabelProvider());
		patternsViewer.setContentProvider(new DefinedPatternsProvider());
		patternsViewer.addDoubleClickListener(new IDoubleClickListener() {

			@Override
			public void doubleClick(DoubleClickEvent ev) {
				for (Object obj : ((IStructuredSelection) ev.getSelection()).toArray()) {
					if ( obj instanceof Pattern ) {
						Pattern pattern = (Pattern)obj;
						
						for (IPatternListener listener : patternListeners)
							listener.patternDoubleClick(pattern);
					}
				}
			}
		});

		patternsViewer.addSelectionChangedListener(new ISelectionChangedListener() {

			@Override
			public void selectionChanged(SelectionChangedEvent ev) {
				for (Object obj : ((IStructuredSelection) ev.getSelection()).toArray()) {
					if ( obj instanceof Pattern ) {
						Pattern pattern = (Pattern)obj;
						
						selectedPattern = pattern; // Обновляем выбранный шаблон
						
						for (IPatternListener listener : patternListeners)
							listener.patternSelect( pattern );
					} else if ( obj instanceof Alternative ) {
						Pattern pattern = ((Alternative)obj).pattern;
						
						selectedPattern = pattern; // Обновляем выбранный шаблон
						
						for (IPatternListener listener : patternListeners)
							listener.patternSelect( pattern );
					}
				}
			}
		});
		patternsViewer.addTreeListener( new ITreeViewerListener(){

			@Override
			public void treeCollapsed( TreeExpansionEvent ev ) {
			}

			@Override
			public void treeExpanded( TreeExpansionEvent ev ) {
				Object element = ev.getElement();
				
				if ( element instanceof Pattern ) {
					patternsViewer.setSubtreeChecked( element, patternsViewer.getChecked( element ) );
				}		
			}
			
		});
		patternsViewer.addCheckStateListener( new ICheckStateListener(){

			@Override
			public void checkStateChanged( CheckStateChangedEvent ev ) {
				Object element = ev.getElement();
				
				if ( element instanceof Pattern ) {
					patternsViewer.setSubtreeChecked( element, ev.getChecked() );
					
					patternChecked( (Pattern) element, ev.getChecked() );
				} else if ( element instanceof Alternative ) {
					Object parent = ((ITreeContentProvider)patternsViewer.getContentProvider()).getParent( element );
					patternsViewer.setSubtreeChecked( parent, ev.getChecked() );
					
					patternChecked( (Pattern) parent, ev.getChecked() );
				}
			}
			
		});
	}

	public void removePatternListener(IPatternListener patternListener) {
		patternListeners.remove( patternListener );
	}

	public void addPatternListener(IPatternListener patternListener) {
		patternListeners.add( patternListener );
	}
	
	public void load(String fileName) throws FileNotFoundException {
		Scanner scanner = new Scanner( new File( fileName ) );
		
		try {
			int i = 0;
			while ( scanner.hasNextLine() ) {
				String line = scanner.nextLine();
				
				if ( line.startsWith("//") || line.startsWith("#") )
					continue;
				
				try {
					document.buildPattern( line );
				} catch ( Throwable ex ) {
					MessageBox mb = new MessageBox(getShell(), SWT.OK | SWT.ICON_ERROR);
			        mb.setText("Ошибка компиляции шаблона");
			        mb.setMessage("Ошибка в шаблоне на строке " + i + ":\n\n" + ex.getMessage());
			        mb.open();
				}
				
				++ i;
			}		
		} finally {
			scanner.close();
		}
		
		update();
	}
	
	public void save(String fileName) throws IOException {
		FileWriter fw = new FileWriter( fileName );
		
		try {
			for ( Pattern pattern : document.getPatternsArray() ) {
				fw.append( "\n\n" );
				fw.append( pattern.name );
				fw.append(" = ");
				fw.append( pattern.getSource() );
			}
		} finally {
			fw.close();
		}
	}

	public void clear() {
		document.clearPatterns();
		patternsViewer.refresh();
	}

	public Pattern getSelectedPattern() {
		return selectedPattern;
	}

	public void setDocument( Document doc ) {
		if ( document != null )
			document.removeListener( documentListener );
		
		document = doc;		
		document.addListener( documentListener );
		
		((DocumentHolder) patternsViewer.getLabelProvider()).setDocument( doc );
		
		patternsViewer.refresh();
	}

	public Set<Pattern> getCheckedPatterns() {
		return checkedPatterns;
	}

	private void patternChecked( Pattern pattern, boolean checked ) {
		if ( checked )
			checkedPatterns.add( pattern );
		else
			checkedPatterns.remove( pattern );
		
		for (IPatternListener listener : patternListeners)
			listener.patternChecked( pattern, checked, checkedPatterns );
	}

	public void extendPatternsMenu( MenuManager patternsMenu ) {
		patternsMenu.add( clearPatternsAction );
		patternsMenu.add( importPatternsAction );
		patternsMenu.add( new Separator() );
		patternsMenu.add( exportPatternsAction );
	}
}
