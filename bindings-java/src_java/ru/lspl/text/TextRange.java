package ru.lspl.text;

public interface TextRange {

	Text getText();

	int getStartOffset();

	int getEndOffset();

	String getContent();

}
