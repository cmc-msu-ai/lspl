package ru.lspl.text;

import java.util.AbstractList;
import java.util.ArrayList;
import java.util.List;

import ru.lspl.LsplObject;
import ru.lspl.text.attributes.SpeechPart;
import ru.lspl.utils.ImmutableArrayList;

/**
 * Узел текста. Обозначает некоторую позицию в тексте.
 * 
 * @author alno
 */
public class Node extends LsplObject implements TextRange {

	/**
	 * Список переходов, начинающихся в узле.
	 * 
	 * @author alno
	 */
	private class TransitionList extends AbstractList<Transition> {

		@Override
		public int size() {
			return getTransitionCount();
		}

		@Override
		public Transition[] toArray() {
			return toArray( new Transition[getTransitionCount()] );
		}

		@Override
		public Transition get( int index ) {
			if ( index >= cachedTransitions.size() ) {
				int count = getTransitionCount();

				if ( index < count ) {
					cachedTransitions.ensureCapacity( count );

					for ( int i = cachedTransitions.size(); i < count; ++i )
						cachedTransitions.add( getTransition( i ) );
				}
			}

			return cachedTransitions.get( index );
		}
	}

	/**
	 * Набор переходов текста, начинающихся в этом узле
	 */
	public final List<Transition> transitions = new TransitionList();

	public final Text text;

	/**
	 * Смещение начала узла в тексте (в символах)
	 */
	public final int startOffset;

	/**
	 * Смещение конца узла в тексте (в символах)
	 */
	public final int endOffset;

	@SuppressWarnings( "unchecked" )
	private final List<Word>[] words = new List[SpeechPart.values().length];

	private final ArrayList<Transition> cachedTransitions = new ArrayList<Transition>();

	private Node( int id, Text text, int startOffset, int endOffset ) {
		super( id );

		this.text = text;
		this.startOffset = startOffset;
		this.endOffset = endOffset;
	}

	/**
	 * Получить коллекцию переходов, начинающихся в узле.
	 * 
	 * @return коллекция переходов, начинающих в узле.
	 */
	public List<Transition> getTransitions() {
		return transitions;
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
			words[index] = new ImmutableArrayList<Word>( internalGetWords( speechPart ) );

		return words[index];
	}

	private List<Word> internalGetWords( SpeechPart speechPart ) {
		List<Word> words = new ArrayList<Word>();

		if ( speechPart == SpeechPart.ANY ) {
			for ( Transition t : transitions ) {
				if ( t instanceof Word )
					words.add( (Word) t );
				else if ( t instanceof Match )
					break;
			}
		} else {
			for ( Transition t : transitions ) {
				if ( t instanceof Word && ((Word) t).speechPart == speechPart )
					words.add( (Word) t );
				else if ( t instanceof Match )
					break;
			}
		}

		return words;
	}

	/**
	 * Получить переход, начинающийся в узле по его индексу
	 * 
	 * @param index
	 *            индекс перехода, начинающегося в узле
	 * @return переход
	 */
	protected native Transition getTransition( int index );

	/**
	 * Получить количество переходов, начинающихся в узле.
	 * 
	 * @return кол-во переходов
	 */
	public native int getTransitionCount();

	/**
	 * Сбросить отладочное предстваление узла в строку
	 * 
	 * @return строка, содержащая отладочное представление узла
	 */
	public native String dump();

	@Override
	public Text getText() {
		return text;
	}

	@Override
	public int getStartOffset() {
		return startOffset;
	}

	@Override
	public int getEndOffset() {
		return endOffset;
	}

	@Override
	public String getContent() {
		return text.getContent().substring( startOffset, endOffset );
	}

	@Override
	public boolean containsPosition( int index ) {
		return index >= startOffset && index <= endOffset;
	}

	@Override
	public boolean coincidesWith( TextRange r ) {
		return r != null && r.getText() == text && r.getStartOffset() == startOffset && r.getEndOffset() == endOffset;
	}

	@Override
	public boolean intersectsWith( TextRange r ) {
		return r != null && r.getText() == text && r.getStartOffset() < endOffset && r.getEndOffset() > startOffset;
	}

}