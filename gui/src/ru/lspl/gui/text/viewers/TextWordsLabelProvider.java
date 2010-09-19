package ru.lspl.gui.text.viewers;

import ru.lspl.text.Node;
import ru.lspl.text.Transition;
import ru.lspl.text.Word;

public class TextWordsLabelProvider extends BaseTransitionsLabelProvider {

	@Override
	protected String getMainColumnText( Object obj ) {
		if ( obj instanceof Word )
			return ((Word) obj).speechPart + ": " + ((Word) obj).base;

		if ( obj instanceof Node )
			for ( Transition t : ((Node) obj).getTransitions() )
				if ( t instanceof Word )
					return ((Word) t).form;

		return "<Unknown>";
	}
}
