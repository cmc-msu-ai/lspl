package ru.lspl.gui.patterns;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import ru.lspl.patterns.Pattern;

/**
 * @author alno
 */
public class PatternInfo extends Composite {

	private Pattern pattern = null;
	private Text infoArea = null;

	public PatternInfo( Composite parent, int style ) {
		super( parent, style );
		initialize();
	}

	private void initialize() {
		infoArea = new Text( this, SWT.MULTI | SWT.WRAP | SWT.V_SCROLL );
		infoArea.setEditable( false );
		setSize( new Point( 300, 200 ) );
		setLayout( new FillLayout() );
	}

	public Pattern getPattern() {
		return pattern;
	}

	public void setPattern( Pattern pattern ) {
		this.pattern = pattern;

		if ( pattern == null ) {
			infoArea.setText( "" );
		} else {
			infoArea.setText( pattern.getSource() + "\n\n" + pattern.dump() );
		}
	}

}
