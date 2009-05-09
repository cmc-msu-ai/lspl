package ru.lspl.text;

public class Token extends Transition {

	public final String token;

	private Token( int id, Text text, Node start, Node end, String token ) {
		super( id, text, start, end );
		this.token = token;
	}

}
