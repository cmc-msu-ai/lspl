package ru.lspl.text;

import java.util.AbstractList;
import java.util.ArrayList;
import java.util.List;

import ru.lspl.LsplObject;

/**
 * Узел текста. Обозначает некоторую позицию в тексте.
 * @author  alno
 */
public class Node extends LsplObject {

	/**
	 * Список переходов, начинающихся в узле.
	 *
	 * @author alno
	 *
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
		public Transition get(int index) {
			return getTransition( index );
		}
	}

	/**
	 * Набор переходов текста, начинающихся в этом узле
	 * @uml.property  name="transitions"
	 */
	public final List<Transition> transitions = new TransitionList();

	/**
	 * Текст, в котором находится узел
	 * @uml.property  name="text"
	 * @uml.associationEnd
	 */
	public final Text text;

	/**
	 * Смещение начала узла в тексте (в символах)
	 */
	public final int startOffset;

	/**
	 * Смещение конца узла в тексте (в символах)
	 */
	public final int endOffset;

	private Node(int id, Text text, int startOffset,int endOffset) {
		super(id);
		this.text = text;
		this.startOffset = startOffset;
		this.endOffset = endOffset;
	}

	/**
	 * Получить коллекцию переходов, начинающихся в узле.
	 * @return  коллекция переходов, начинающих в узле.
	 * @uml.property  name="transitions"
	 */
	public List<Transition> getTransitions() {
		return transitions;
	}

	public Transition[] getWordsArray() {
		ArrayList<Transition> words = new ArrayList<Transition>();

		for ( Transition t : transitions )
			if ( t instanceof Word )
				words.add( t );

		return words.toArray( new Transition[ words.size() ] );
	}

	/**
	 * Получить переход, начинающийся в узле по его индексу
	 *
	 * @param index
	 *            индекс перехода, начинающегося в узле
	 * @return переход
	 */
	public native Transition getTransition(int index);

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

	protected native void finalize();

}