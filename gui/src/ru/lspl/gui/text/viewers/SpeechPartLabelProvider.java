package ru.lspl.gui.text.viewers;

import org.eclipse.jface.viewers.LabelProvider;

import ru.lspl.text.attributes.SpeechPart;

public class SpeechPartLabelProvider extends LabelProvider {

	@Override
	public String getText(Object obj) {		
		return ((SpeechPart)obj).getPluralTitle();
	}
}
