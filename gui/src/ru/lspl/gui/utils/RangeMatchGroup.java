package ru.lspl.gui.utils;

import java.util.List;

import ru.lspl.text.Match;
import ru.lspl.text.Node;

public class RangeMatchGroup {

	public final List<Match> matches;

	public final Node start;

	public final Node end;

	public final String text;

	public RangeMatchGroup( List<Match> matches, Node start, Node end, String text ) {
		this.matches = matches;
		this.start = start;
		this.end = end;
		this.text = text;
	}

	public String getFragment() {
		return text;
	}

}
