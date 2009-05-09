package ru.lspl.text;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import ru.lspl.LsplObject;
import ru.lspl.text.attributes.AttributeContainer;

/**
 * Переход в тексте.
 * @author  alno
 */
public class Transition extends LsplObject implements AttributeContainer {

	/**
	 * Текст, содержащий переход
	 * @uml.property  name="text"
	 * @uml.associationEnd  
	 */
	public final Text text;

	/**
	 * Узел, в котором начинается переход
	 * @uml.property  name="start"
	 * @uml.associationEnd  
	 */
	public final Node start;

	/**
	 * Узел, в котором заканчивается переход
	 * @uml.property  name="end"
	 * @uml.associationEnd  
	 */
	public final Node end;

	/**
	 * Выбрать из массива аннотаций те, которые включают позицию
	 * 
	 * @param transitions
	 *            массив аннотаций
	 * @param position
	 *            позиция
	 * @return массив выбранных аннотаций
	 */
	public static Transition[] selectIncludesPosition(Transition[] transitions, int position) {
		ArrayList<Transition> selectedTransitions = new ArrayList<Transition>();

		for (Transition t : transitions)
			if (t.includesPosition( position ))
				selectedTransitions.add( t );

		return selectedTransitions.toArray( new Transition[selectedTransitions.size()] );
	}

	protected Transition(int id, Text text, Node start, Node end) {
		super( id );
		this.text = text;
		this.start = start;
		this.end = end;
	}

	/**
	 * Проверить, содержит ли аннотация заданную позицию текста позицию
	 * 
	 * @param position
	 *            позиция в тексте
	 * @return true, если содержит
	 */
	public boolean includesPosition(int position) {
		return start.endOffset < position && end.startOffset > position;
	}

	/**
	 * Получить фрагмент текста, соответствующий переходу
	 * 
	 * @return фрагмент текста
	 */
	public native String getFragment();

	/**
	 * Сбросить отладочное представление перехода в строку.
	 * 
	 * @return строка, содержащее отладочное представление перехода
	 */
	public native String dump();

	protected native void finalize();

	@Override
	public native Object getAttribute( int key );
	
	/**
	 * @uml.property  name="attributes"
	 */
	private Map<Integer, Object> attributes;

	/**
	 * @return
	 * @uml.property  name="attributes"
	 */
	@Override
	public Map<Integer, Object> getAttributes() {
		if ( attributes == null ) {
			attributes = new HashMap<Integer, Object>();
			
			for ( int i = 1; i < 20; ++ i ) {
				Object value = getAttribute( i );
				
				if ( value != null )
					attributes.put( i, value );
			}
		}
		
		return attributes;
	}
}