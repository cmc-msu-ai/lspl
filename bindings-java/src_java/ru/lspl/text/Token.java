package ru.lspl.text;

public class Token extends Transition {

	public final String token;

	private Token( int id, Text text, int start, int end ) {
		super( id, text, start, end );
		this.token = getContent();
	}

}
