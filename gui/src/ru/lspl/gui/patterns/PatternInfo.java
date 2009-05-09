package ru.lspl.gui.patterns;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import ru.lspl.patterns.Pattern;

/**
 * @author  alno
 */
public class PatternInfo extends Composite {
	
	/**
	 * @uml.property  name="pattern"
	 * @uml.associationEnd  
	 */
	private Pattern pattern = null;  //  @jve:decl-index=0:
	private Text infoArea = null;

	public PatternInfo(Composite parent, int style) {
		super(parent, style);
		initialize();
	}
	
	private void initialize() {
		infoArea = new Text(this, SWT.MULTI | SWT.WRAP | SWT.V_SCROLL );
		infoArea.setEditable( false );
		setSize(new Point(300, 200));
		setLayout(new FillLayout());
	}

	/**
	 * @return
	 * @uml.property  name="pattern"
	 */
	public Pattern getPattern() {
		return pattern;
	}

	/**
	 * @param pattern
	 * @uml.property  name="pattern"
	 */
	public void setPattern(Pattern pattern) {
		this.pattern = pattern;
		
		if ( pattern == null ) {
			infoArea.setText("");
		} else {			
			infoArea.setText(pattern.getSource() + "\n\n" + pattern.dump());
		}
	}

}
