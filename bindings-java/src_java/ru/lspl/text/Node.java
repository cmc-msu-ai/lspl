package ru.lspl.text;

import java.util.AbstractList;
import java.util.ArrayList;
import java.util.List;

/**
 * Узел текста. Обозначает некоторую позицию в тексте.
 * 
 * @author alno
 */
public class Node extends TextElement implements TextRange {

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
			return getTransition( index );
		}
	}

	/**
	 * Набор переходов текста, начинающихся в этом узле
	 */
	public final List<Transition> transitions = new TransitionList();

	/**
	 * Смещение начала узла в тексте (в символах)
	 */
	public final int startOffset;

	/**
	 * Смещение конца узла в тексте (в символах)
	 */
	public final int endOffset;

	private Node( int id, Text text, int startOffset, int endOffset ) {
		super( id, text );

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

	public Transition[] getWordsArray() {
		ArrayList<Transition> words = new ArrayList<Transition>();

		for ( Transition t : transitions )
			if ( t instanceof Word )
				words.add( t );

		return words.toArray( new Transition[words.size()] );
	}

	/**
	 * Получить переход, начинающийся в узле по его индексу
	 * 
	 * @param index
	 *            индекс перехода, начинающегося в узле
	 * @return переход
	 */
	public native Transition getTransition( int index );

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