package ru.lspl.text;

import ru.lspl.patterns.Pattern;

/**
 * Сопоставление в тексте.
 * 
 * @author alno
 */
public class Match extends Transition {

	/**
	 * Шаблон, который был сопоставлен
	 */
	public final Pattern pattern;

	private Match(int id, Text text, Node start, Node end, Pattern pattern) {
		super(id, text, start, end);
		this.pattern = pattern;
	}
	
	/**
	 * Получить количество вариантов сопоставления
	 */
	public native int getVariantCount();
	
	/**
	 * Получить вариант сопоставления по его индексу
	 */
	public native MatchVariant getVariant(int index);
}