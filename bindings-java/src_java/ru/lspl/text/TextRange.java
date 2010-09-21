package ru.lspl.text;

public interface TextRange {

	Text getText();

	int getStartOffset();

	int getEndOffset();

	String getContent();

	boolean containsPosition( int index );

	boolean coincidesWith( TextRange r );

	boolean intersectsWith( TextRange r );

}
