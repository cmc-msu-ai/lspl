package ru.lspl.gui.text.viewers;

import org.eclipse.jface.viewers.ITableLabelProvider;

import ru.lspl.gui.utils.RangeMatchGroup;
import ru.lspl.patterns.Pattern;
import ru.lspl.text.Match;

public class TextMatchesLabelProvider extends BaseTransitionsLabelProvider implements ITableLabelProvider {

	@Override
	protected String getMainColumnText( Object obj ) {
		if ( obj instanceof Match )
			return "[" + ((Match) obj).start.endOffset + "-" + ((Match) obj).end.startOffset + "] " + ((Match) obj).getContent();

		if ( obj instanceof RangeMatchGroup )
			return "[" + ((RangeMatchGroup) obj).start.endOffset + "-" + ((RangeMatchGroup) obj).end.startOffset + "] " + ((RangeMatchGroup) obj).getFragment();

		if ( obj instanceof Pattern )
			return ((Pattern) obj).name;

		return "<Unknown>";
	}

	@Override
	protected String getParamsColumnText( Object obj ) {
		if ( obj instanceof RangeMatchGroup ) {
			RangeMatchGroup group = (RangeMatchGroup) obj;

			if ( group.matches.size() == 1 )
				return super.getParamsColumnText( group.matches.get( 0 ) );
			else
				return "";
		}

		return super.getParamsColumnText( obj );
	}

}
