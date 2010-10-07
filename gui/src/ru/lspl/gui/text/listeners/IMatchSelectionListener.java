package ru.lspl.gui.text.listeners;

import ru.lspl.patterns.Pattern;
import ru.lspl.text.Match;
import ru.lspl.text.Text;

public interface IMatchSelectionListener {

	void patternSelect( Pattern pattern );

	void patternDoubleClick( Pattern pattern );

	void matchSelect( Match match );

	void matchDoubleClick( Match match );

	void textSelect( Text obj );

	void textDoubleClick( Text obj );
}
