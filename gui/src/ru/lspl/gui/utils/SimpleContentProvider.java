package ru.lspl.gui.utils;

import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.Viewer;

public class SimpleContentProvider implements IStructuredContentProvider {

	@Override
	public Object[] getElements( Object input ) {
		return (Object[]) input;
	}

	@Override
	public void dispose() {
	}

	@Override
	public void inputChanged( Viewer arg0, Object arg1, Object arg2 ) {
	}

}
