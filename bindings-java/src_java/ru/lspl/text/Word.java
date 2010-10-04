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
	 */
	public SpeechPart speechPart;

	private Word( int id, Text text, int start, int end, String base, Token token, int sp ) {
		super( id, text, start, end );
		this.base = base;
		this.form = token.token;
		this.speechPart = SpeechPart.values()[sp];
	}

}
