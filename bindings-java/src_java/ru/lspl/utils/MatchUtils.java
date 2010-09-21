package ru.lspl.utils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import ru.lspl.text.Match;
import ru.lspl.text.MatchGroup;

public class MatchUtils {

	public static List<MatchGroup> groupMatches( List<Match> matches ) {
		if ( matches.isEmpty() )
			return Collections.emptyList();

		List<MatchGroup> groups = new ArrayList<MatchGroup>();

		int startIndex = 0;
		Match startMatch = matches.get( startIndex );

		for ( int curIndex = 1; curIndex < matches.size(); ++curIndex ) {
			Match curMatch = matches.get( curIndex );

			if ( !startMatch.coincidesWith( curMatch ) ) {
				groups.add( new MatchGroup( matches.subList( startIndex, curIndex ) ) );

				startIndex = curIndex;
				startMatch = curMatch;
			}
		}

		groups.add( new MatchGroup( matches.subList( startIndex, matches.size() ) ) );

		return groups;
	}

}
