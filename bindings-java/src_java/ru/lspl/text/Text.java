package ru.lspl.text;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import ru.lspl.LsplObject;
import ru.lspl.patterns.Pattern;
import ru.lspl.text.attributes.SpeechPart;
import ru.lspl.utils.ImmutableArrayList;
import ru.lspl.utils.MatchUtils;

/**
 * Текст в Lspl. Текст представляется в виде графа, вершинами в котором являются
 * некоторые точки текста, а ребрами элементы разметки текста, такие как: слова,
 * сопоставления шаблонов и т.п.
 * 
 * @author alno
 */
public class Text extends LsplObject implements TextRange {

	public final String content;

	/**
	 * Immutable lists of words for different speech parts
	 */
	@SuppressWarnings( "unchecked" )
	private final List<Word>[] words = new List[SpeechPart.values().length];

	/**
	 * Immutable lists of matches for different patterns
	 */
	private final Map<Pattern, List<Match>> matchesByPattern = new HashMap<Pattern, List<Match>>();

	/**
	 * Immutable list of text nodes
	 */
	private List<Node> nodes;

	/**
	 * Array of text nodes
	 */
	private Node[] nodeArray;

	/**
	 * Метод, используемый для создания нового экземпляра текста на основе
	 * строки. Новый текст содержит разметку, полученную в результате
	 * морфологического анализа.
	 * 
	 * @param source
	 *            строка, на основе которой строится текст
	 * @return новый экзмпляр текста
	 */
	public static native Text create( String source );

	public static native Text create( String source, TextConfig config );

	/**
	 * Получить содержимое текста
	 * 
	 * @return содержимое текста
	 */
	@Override
	public String getContent() {
		return content;
	}

	@Override
	public Text getText() {
		return this;
	}

	@Override
	public int getStartOffset() {
		return 0;
	}

	@Override
	public int getEndOffset() {
		return content.length();
	}

	@Override
	public boolean containsPosition( int index ) {
		return index >= 0 && index <= content.length();
	}

	@Override
	public boolean coincidesWith( TextRange r ) {
		return r != null && r.getText() == this && r.getStartOffset() == 0 && r.getEndOffset() == content.length();
	}

	@Override
	public boolean intersectsWith( TextRange r ) {
		return r != null && r.getText() == this && r.getStartOffset() < content.length() && r.getEndOffset() > 0;
	}

	/**
	 * Получить коллекцию узлов текста
	 * 
	 * @return коллекция узлов текста
	 */
	public List<Node> getNodes() {
		return nodes;
	}

	/**
	 * Получить коллекцию слов текста
	 * 
	 * @return коллекция слов текста
	 */
	public List<Word> getWords() {
		return getWords( SpeechPart.ANY );
	}

	/**
	 * Получить коллекцию слов текста заданной части речи
	 * 
	 * @param speechPart
	 *            часть речи
	 * @return коллекция слов
	 */
	public List<Word> getWords( SpeechPart speechPart ) {
		int index = speechPart.ordinal();

		if ( words[index] == null )
			words[index] = new ImmutableArrayList<Word>( internalGetWords( index ) );

		return words[index];
	}

	public List<Transition> getTransitions() {
		ArrayList<Transition> transitions = new ArrayList<Transition>( getTransitionCount() );

		for ( Node n : nodes )
			transitions.addAll( n.getTransitions() );

		return transitions;
	}

	public Transition[] getTransitionsArray() {
		Transition[] transitions = new Transition[getTransitionCount()];

		int i = 0;
		for ( Node n : nodes )
			for ( Transition t : n.transitions )
				transitions[i++] = t;

		return transitions;
	}

	protected int getTransitionCount() {
		int size = 0;

		for ( Node n : nodes )
			size += n.getTransitionCount();

		return size;
	}

	public List<Match> getMatches( Pattern pattern ) {
		List<Match> matches = matchesByPattern.get( pattern );

		if ( matches == null )
			matchesByPattern.put( pattern, matches = new ImmutableArrayList<Match>( internalGetMatches( pattern ) ) );

		return matches;
	}

	public List<Match> getMatches( Iterable<Pattern> patterns ) {
		ArrayList<Match> matches = new ArrayList<Match>();

		for ( Pattern pattern : patterns )
			matches.addAll( getMatches( pattern ) );

		return matches;
	}

	public List<Match> getMatches( Pattern... patterns ) {
		ArrayList<Match> matches = new ArrayList<Match>();

		for ( Pattern pattern : patterns )
			matches.addAll( getMatches( pattern ) );

		return matches;
	}

	public List<MatchGroup> getMatchGroups( Pattern pattern ) {
		return MatchUtils.groupMatches( getMatches( pattern ) );
	}

	public List<MatchGroup> getMatchGroups( Pattern... patterns ) {
		return MatchUtils.groupMatches( getMatches( patterns ) );
	}

	public List<MatchGroup> getMatchGroups( Iterable<Pattern> patterns ) {
		return MatchUtils.groupMatches( getMatches( patterns ) );
	}

	public Pattern[] selectMatched( Pattern[] patterns ) {
		ArrayList<Pattern> matched = new ArrayList<Pattern>();

		for ( Pattern pattern : patterns )
			if ( getMatches( pattern ).size() > 0 )
				matched.add( pattern );

		return matched.toArray( new Pattern[matched.size()] );
	}

	/**
	 * Проверить, имеет ли хотя бы один шаблон из массива сопоставление в тексте
	 * 
	 * @param patterns
	 *            массив шаблонов
	 * @return true, если есть шаблон из массива, который имеет сопоставление
	 */
	public boolean hasAnyMatches( Pattern... patterns ) {
		for ( Pattern pattern : patterns )
			if ( getMatches( pattern ).size() > 0 )
				return true;

		return false;
	}

	/**
	 * Проверить, имеет ли хотя бы один шаблон из коллекции сопоставление в
	 * тексте
	 * 
	 * @param patterns
	 *            коллекция шаблонов
	 * @return true, если есть шаблон из коллекции, который имеет сопоставление
	 */
	public boolean hasAnyMatches( Iterable<? extends Pattern> patterns ) {
		for ( Pattern pattern : patterns )
			if ( getMatches( pattern ).size() > 0 )
				return true;

		return false;
	}

	/**
	 * Проверить, имеют ли все шаблоны из массива сопоставления в тексте
	 * 
	 * @param patterns
	 *            массив шаблонов
	 * @return true, если все шаблоны имеют сопоставления в тексте
	 */
	public boolean hasAllMatches( Pattern... patterns ) {
		for ( Pattern pattern : patterns )
			if ( getMatches( pattern ).size() <= 0 )
				return false;

		return true;
	}

	/**
	 * Проверить, имеют ли все шаблоны из коллекции сопоставления в тексте
	 * 
	 * @param patterns
	 *            коллекция шаблонов
	 * @return true, если все шаблоны имеют сопоставления в тексте
	 */
	public boolean hasAllMatches( Iterable<? extends Pattern> patterns ) {
		for ( Pattern pattern : patterns )
			if ( getMatches( pattern ).size() <= 0 )
				return false;

		return true;
	}

	public List<Word> findWordsContainingPosition( int offset ) {
		List<Node> nodes = findNodesBeforePosition( offset );

		if ( nodes.isEmpty() )
			return Collections.emptyList();

		List<Word> transitions = new ArrayList<Word>();

		for ( Node n : nodes )
			for ( Transition t : n.transitions )
				if ( t.end.startOffset >= offset && t instanceof Word )
					transitions.add( (Word) t );

		return transitions;
	}

	public List<Match> findMatchesContainingPosition( int offset ) {
		List<Node> nodes = findNodesBeforePosition( offset );

		if ( nodes.isEmpty() )
			return Collections.emptyList();

		List<Match> transitions = new ArrayList<Match>();

		for ( Node n : nodes )
			for ( Transition t : n.transitions )
				if ( t.end.startOffset >= offset && t instanceof Match )
					transitions.add( (Match) t );

		return transitions;
	}

	public List<Transition> findTransitionsContainingPosition( int offset ) {
		List<Node> nodes = findNodesBeforePosition( offset );

		if ( nodes.isEmpty() )
			return Collections.emptyList();

		List<Transition> transitions = new ArrayList<Transition>();

		for ( Node n : nodes )
			for ( Transition t : n.transitions )
				if ( t.end.startOffset >= offset )
					transitions.add( t );

		return transitions;
	}

	public List<Node> findNodesBeforePosition( int offset ) {
		for ( int i = nodeArray.length - 1; i >= 0; --i ) {
			Node n = nodeArray[i];

			if ( n.endOffset <= offset )
				return nodes.subList( 0, i + 1 );
		}

		return Collections.emptyList();
	}

	/**
	 * Сбросить отладочное представление структуры текста в строку
	 * 
	 * @return отладочное представление текста
	 */
	public native String dump();

	@Override
	protected native void finalize();

	private Text( int id, String content ) {
		super( id );

		this.content = content;
	}

	private void initialize( Node[] nodeArray ) {
		this.nodeArray = nodeArray;
		this.nodes = new ImmutableArrayList<Node>( nodeArray );
	}

	private native Word[] internalGetWords( int speechPart );

	private native Match[] internalGetMatches( Pattern pattern );

}