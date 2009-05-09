package ru.lspl.gui.text.listeners;

import ru.lspl.text.Node;
import ru.lspl.text.Word;

public interface IWordSelectionListener {
	
	void wordSelect(Word word);

	void wordDoubleClick(Word word);
	
	void nodeSelect(Node node);
	
	void nodeDoubleClick(Node node);
}
