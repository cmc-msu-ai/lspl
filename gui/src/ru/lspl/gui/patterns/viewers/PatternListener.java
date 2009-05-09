package ru.lspl.gui.patterns.viewers;

import java.util.Set;

import ru.lspl.patterns.Pattern;

public class PatternListener implements IPatternListener {

	@Override
	public void patternSelect(Pattern pattern) {
		// NOP implementation
	}

	@Override
	public void patternDoubleClick(Pattern pattern) {
		// NOP implementation
	}

	@Override
	public void patternChecked( Pattern pattern, boolean checked, Set<Pattern> checkedPatterns ) {
		// NOP implementation
	}

}
