package ru.lspl.text;

import java.util.List;

import ru.lspl.utils.ImmutableArrayList;

public class MatchGroup implements TextRange {

	public final List<Match> matches;

	public final Node start;

	public final Node end;

	public final Text text;

	public final String fragment;

	public MatchGroup( Text text, List<Match> matches, Node start, Node end ) {
		this.text = text;
		this.matches = new ImmutableArrayList<Match>( matches );
		this.start = start;
		this.end = end;
		this.fragment = text.getContent().substring( start.startOffset, end.endOffset );
	}

	public MatchGroup( List<Match> matches ) {
		this( matches.get( 0 ).text, matches, matches.get( 0 ).start, matches.get( 0 ).end );
	}

	public List<Match> getMatches() {
		return matches;
	}

	public Node getStart() {
		return start;
	}

	public Node getEnd() {
		return end;
	}

	@Override
	public Text getText() {
		return text;
	}

	@Override
	public int getStartOffset() {
		return start.endOffset;
	}

	@Override
	public int getEndOffset() {
		return end.startOffset;
	}

	@Override
	public String getContent() {
		return text.getContent().substring( start.endOffset, end.startOffset );
	}

}
