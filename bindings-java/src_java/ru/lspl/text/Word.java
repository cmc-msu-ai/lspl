package ru.lspl.text;

import ru.lspl.text.attributes.SpeechPart;

/**
 * Слово в тексте.
 * 
 * @author alno
 */
public class Word extends Transition {

	/**
	 * Базовая форма слова
	 */
	public final String base;

	/**
	 * Форма слова
	 */
	public final String form;

	/**
	 * Часть речи слова
	 * 
	 * @uml.property name="speechPart"
	 * @uml.associationEnd
	 */
	public SpeechPart speechPart;

	private Word( int id, Text text, Node start, Node end, String base, String form, int sp ) {
		super( id, text, start, end );
		this.base = base;
		this.form = form;
		this.speechPart = SpeechPart.values()[sp];
	}

}
