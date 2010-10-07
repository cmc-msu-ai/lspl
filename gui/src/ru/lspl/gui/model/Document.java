package ru.lspl.gui.model;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import ru.lspl.gui.model.extractors.DocExtractor;
import ru.lspl.gui.model.extractors.OoxmlExtractor;
import ru.lspl.gui.model.extractors.PlainTextExtractor;
import ru.lspl.gui.model.extractors.TextExtractor;
import ru.lspl.gui.model.extractors.XlsExtractor;
import ru.lspl.patterns.Pattern;
import ru.lspl.patterns.PatternBuilder;
import ru.lspl.patterns.PatternBuildingException;
import ru.lspl.text.Text;
import ru.lspl.text.TextConfig;

/**
 * @author alno
 */
public class Document {

	/**
	 * @uml.property name="eMPTY_TEXT"
	 * @uml.associationEnd
	 */
	private static final Text EMPTY_TEXT = Text.create( "" );

	public boolean autoAnalyze = false;

	private boolean analysisNeeded = false;

	/**
	 * Конфиг парсера текста
	 * 
	 * @uml.property name="textConfig"
	 * @uml.associationEnd
	 */
	private TextConfig textConfig = new TextConfig();

	/**
	 * Имя сохраненного файла
	 * 
	 * @uml.property name="savedFileName"
	 */
	private String savedFileName = null;

	/**
	 * Исходный текст
	 * 
	 * @uml.property name="sourceText"
	 */
	private String sourceText = "";

	/**
	 * Проанализированный текст
	 * 
	 * @uml.property name="analyzedText"
	 * @uml.associationEnd
	 */
	private Text analyzedText = EMPTY_TEXT;

	/**
	 * Построитель шаблонов, используемый при анализе
	 * 
	 * @uml.property name="patternBuilder"
	 * @uml.associationEnd
	 */
	private PatternBuilder patternBuilder = PatternBuilder.create();

	/**
	 * Шаблоны, используемые при анализе
	 * 
	 * @uml.property name="patterns"
	 * @uml.associationEnd multiplicity="(0 -1)"
	 */
	private Pattern[] patterns = null;

	/**
	 * Подписчики событий
	 */
	private Collection<DocumentListener> listeners = new ArrayList<DocumentListener>();

	/**
	 * @return
	 * @uml.property name="analyzedText"
	 */
	public Text getAnalyzedText() {
		return analyzedText;
	}

	/**
	 * @return
	 * @uml.property name="sourceText"
	 */
	public String getSourceText() {
		return sourceText;
	}

	/**
	 * @param srcText
	 * @uml.property name="sourceText"
	 */
	public void setSourceText( String srcText ) {
		sourceText = srcText;

		fireDocumentUpdated();
		autoAnalyzeIfEnabled();
	}

	/**
	 * @return
	 * @uml.property name="textConfig"
	 */
	public TextConfig getTextConfig() {
		return textConfig;
	}

	/**
	 * @param textConfig
	 * @uml.property name="textConfig"
	 */
	public void setTextConfig( TextConfig textConfig ) {
		this.textConfig = textConfig;

		fireDocumentUpdated();
		autoAnalyzeIfEnabled();
	}

	public boolean isAnalysisNeeded() {
		return analysisNeeded;
	}

	public void analyze() {
		analyzedText = Text.create( sourceText, textConfig );

		for ( Pattern pattern : getPatternsArray() )
			analyzedText.getMatches( pattern ); // Обработать текст шаблоном

		fireDocumentAnalyzed();

		analysisNeeded = false;
	}

	public Pattern[] getPatternsArray() {
		if ( patterns != null ) // Если шаблоны уже определены
			return patterns; // Возвращаем их

		return (patterns = patternBuilder.getDefinedPatternsArray());
	}

	public List<Pattern> getPatternList() {
		return patternBuilder.definedPatterns;
	}

	public void buildPattern( String source ) throws PatternBuildingException {
		patternBuilder.build( source );
		patterns = null;

		fireDocumentUpdated();
		autoAnalyzeIfEnabled();
	}

	public void clearPatterns() {
		patternBuilder = PatternBuilder.create();
		patterns = null;

		fireDocumentUpdated();
		autoAnalyzeIfEnabled();
	}

	public void load( String fileName ) throws IOException {
		TextExtractor extractor = selectTextExtractor( fileName );

		FileInputStream is = new FileInputStream( fileName );
		String text = extractor.extractText( is );
		is.close();

		setSourceText( text );

		savedFileName = extractor.isLossless() ? fileName : null;
	}

	public boolean hasSavedFileName() {
		return savedFileName != null;
	}

	/**
	 * @return
	 * @uml.property name="savedFileName"
	 */
	public String getSavedFileName() {
		return savedFileName;
	}

	public void save() throws IOException {
		if ( savedFileName == null )
			throw new IllegalStateException( "Text hasn't saved filename" );

		save( savedFileName );
	}

	public void save( String fileName ) throws IOException {
		FileOutputStream fo = new FileOutputStream( fileName );
		fo.write( sourceText.getBytes() );
		fo.close();

		savedFileName = fileName;
	}

	public void addListener( DocumentListener listener ) {
		listeners.add( listener );
	}

	public void removeListener( DocumentListener listener ) {
		listeners.remove( listener );
	}

	public void clear() {
		setSourceText( "" );
	}

	protected TextExtractor selectTextExtractor( String fileName ) {
		String fileNameLower = fileName.toLowerCase();

		if ( fileNameLower.endsWith( ".doc" ) ) {
			return new DocExtractor();
		} else if ( fileNameLower.endsWith( ".xls" ) ) {
			return new XlsExtractor();
		} else if ( fileNameLower.endsWith( ".docx" ) ) {
			return new OoxmlExtractor();
		} else {
			return new PlainTextExtractor();
		}
	}

	protected void fireDocumentUpdated() {
		for ( DocumentListener listener : listeners )
			listener.documentUpdated( this ); // Извещаем подписчиков об обновлении документа
	}

	protected void fireDocumentAnalyzed() {
		for ( DocumentListener listener : listeners )
			listener.documentAnalyzed( this ); // Извещаем подписчиков об анализе документа
	}

	protected void autoAnalyzeIfEnabled() {
		if ( autoAnalyze ) // Если стоит флаг автоанализа, анализируем текст
			analyze();
		else
			analysisNeeded = true;
	}

}
