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

	@Override
	public boolean containsPosition( int index ) {
		return index >= start.endOffset && index <= end.startOffset;
	}

	@Override
	public boolean coincidesWith( TextRange r ) {
		return r != null && r.getText() == text && r.getStartOffset() == start.endOffset && r.getEndOffset() == end.startOffset;
	}

	public boolean coincidesWith( Transition t ) {
		return t != null && t.text == text && t.start == start && t.end == end;
	}

	@Override
	public boolean intersectsWith( TextRange r ) {
		return r != null && r.getText() == text && r.getStartOffset() < end.startOffset && r.getEndOffset() > start.endOffset;
	}

}
