package ru.lspl.gui.text.viewers;

import org.eclipse.jface.viewers.ITableLabelProvider;

import ru.lspl.patterns.Pattern;
import ru.lspl.text.Match;

public class TextMatchesLabelProvider extends BaseTransitionsLabelProvider implements ITableLabelProvider {
		
	protected String getMainColumnText( Object obj ) {
		if ( obj instanceof Match )
			return "[" + ((Match)obj).start.endOffset + "-" + ((Match)obj).end.startOffset + "] " + ((Match)obj).getFragment();
		
		if ( obj instanceof Pattern )
			return ((Pattern)obj).name;
		
		return "<Unknown>";
	}

}
