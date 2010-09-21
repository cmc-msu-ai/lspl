package ru.lspl.gui.utils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import ru.lspl.text.Match;
import ru.lspl.text.Transition;

public class RangeMatchGroupper {

	public List<RangeMatchGroup> groupMatches( List<Match> matches ) {
		if ( matches.isEmpty() )
			return Collections.emptyList();

		List<RangeMatchGroup> groups = new ArrayList<RangeMatchGroup>();

		int startIndex = 0;
		Match startMatch = matches.get( startIndex );

		for ( int curIndex = 1; curIndex < matches.size(); ++curIndex ) {
			Match curMatch = matches.get( curIndex );

			if ( !isCompatible( startMatch, curMatch ) ) {
				groups.add( createGroup( matches, startIndex, curIndex ) );

				startIndex = curIndex;
				startMatch = curMatch;
			}
		}

		groups.add( createGroup( matches, startIndex, matches.size() ) );

		return groups;
	}

	private RangeMatchGroup createGroup( List<Match> matches, int startIndex, int endIndex ) {
		Match refMatch = matches.get( startIndex );

		return new RangeMatchGroup( matches.subList( startIndex, endIndex ), refMatch.start, refMatch.end, refMatch.getContent() );
	}

	private boolean isCompatible( Transition first, Transition second ) {
		return first.start == second.start && first.end == second.end;
	}
}
