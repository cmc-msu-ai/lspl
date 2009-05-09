package ru.lspl.gui;

import org.eclipse.jface.action.Action;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;

import ru.lspl.gui.model.Document;
import ru.lspl.gui.patterns.PatternEditor;
import ru.lspl.gui.text.TextConfigEditor;

/**
 * @author  alno
 */
public class EditPanel extends Composite {	

	private static final String PATTERNS_TEXT = "Шаблоны";
	private static final String TEXT_CONFIG_TEXT = "Опции";
	
	public final Action showPatternsAction = new Action("&Шаблоны") {
		
		@Override
		public void run() {
			showPatterns();
		}
	};
		
	public final Action showConfigAction = new Action("Опции") {
		
		@Override
		public void run() {
			showConfig();
		}
	};
		
	/**
	 * @uml.property  name="patternEditor"
	 * @uml.associationEnd  
	 */
	public final PatternEditor patternEditor;
	/**
	 * @uml.property  name="textConfigEditor"
	 * @uml.associationEnd  
	 */
	public final TextConfigEditor textConfigEditor;
	
	private CTabFolder tabFolder;
	
	private CTabItem tiPatterns = null;
	private CTabItem tiConfig = null;	

	public EditPanel(Composite parent, int style) {
		super(parent, style);
		
		Color bgColor = new Color( getDisplay(), 255, 255, 255 );
		
		tabFolder = new CTabFolder(this, SWT.BORDER);
		patternEditor = new PatternEditor(tabFolder, SWT.NONE);
		patternEditor.setBackground(bgColor );
		
		textConfigEditor = new TextConfigEditor(tabFolder,SWT.NONE);
		textConfigEditor.setBackground(bgColor );
		
		/*
		 * Устанавливаем лайоут
		 */
		setLayout(new FillLayout());
		
		/*
		 * Показываем вкладки
		 */
		showPatterns();
		showConfig();
		
		tabFolder.setSelection( tiPatterns );
	}

	public void showPatterns() {
		if ( tiPatterns == null || tiPatterns.isDisposed()  ) {
			tiPatterns = new CTabItem(tabFolder, SWT.CLOSE);
			tiPatterns.setControl( patternEditor );
			tiPatterns.setText( PATTERNS_TEXT );
		}
		
		tabFolder.setSelection( tiPatterns );
	}
		
	public void showConfig() {
		if ( tiConfig == null || tiConfig.isDisposed()  ) {
			tiConfig = new CTabItem(tabFolder, SWT.CLOSE);
			tiConfig.setControl( textConfigEditor );
			tiConfig.setText( TEXT_CONFIG_TEXT );
		}
		
		tabFolder.setSelection( tiConfig );
	}

	public void setDocument( Document document ) {
		patternEditor.setDocument( document );
		textConfigEditor.setDocument( document );
	}
}
