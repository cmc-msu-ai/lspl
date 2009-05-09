package ru.lspl.gui.text.viewers;

import java.util.Map;

import org.eclipse.jface.viewers.BaseLabelProvider;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.swt.graphics.Image;

import ru.lspl.text.Transition;
import ru.lspl.text.attributes.AttributeKey;

public abstract class BaseTransitionsLabelProvider extends BaseLabelProvider implements ITableLabelProvider {	

	protected static final int COLUMN_MAIN = 0;
	protected static final int COLUMN_PARAMS = 1;

	@Override
	public Image getColumnImage( Object arg0, int arg1 ) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getColumnText( Object obj, int column ) {
		switch ( column ) {
		case COLUMN_MAIN:
			return getMainColumnText( obj );
		case COLUMN_PARAMS:
			return getParamsColumnText( obj );
		default:
			return "";
		}
	}

	protected String getParamsColumnText( Object obj ) {
		if ( obj instanceof Transition ) {
			StringBuilder builder = new StringBuilder();
			boolean first = true;
			for ( Map.Entry<Integer,Object> entry : ((Transition)obj).getAttributes().entrySet() ) {
				if ( !first )
					builder.append( ", " );
				
				first = false;
				
				builder.append( AttributeKey.valueOf( entry.getKey() ).getTitle() );
				builder.append( ": " );
				builder.append( entry.getValue().toString() );
			}
			
			return builder.toString();
		}	
		
		return "";
	}
	
	protected abstract String getMainColumnText( Object obj );

}