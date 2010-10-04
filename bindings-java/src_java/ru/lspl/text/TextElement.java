package ru.lspl.text;

import ru.lspl.LsplObject;

public abstract class TextElement extends LsplObject {

	/**
	 * Текст, в котором находится узел
	 */
	public final Text text;

	protected TextElement( int id, Text text ) {
		super( id );

		this.text = text;
	}

	public Text getText() {
		return text;
	}

}
