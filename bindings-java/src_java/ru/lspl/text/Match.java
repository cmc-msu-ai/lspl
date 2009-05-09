package ru.lspl.text;

import ru.lspl.patterns.Pattern;

/**
 * Сопоставление в тексте.
 * @author  alno
 */
public class Match extends Transition {

	/**
	 * Шаблон, который был сопоставлен
	 * @uml.property  name="pattern"
	 * @uml.associationEnd  
	 */
	public final Pattern pattern;

	private Match(int id, Text text, Node start, Node end, Pattern pattern) {
		super(id, text, start, end);
		this.pattern = pattern;
	}
}