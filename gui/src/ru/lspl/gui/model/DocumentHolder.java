package ru.lspl.gui.model;

/**
 * @author alno
 */
public interface DocumentHolder {

	/**
	 * @param doc
	 * @uml.property name="document"
	 */
	void setDocument( Document doc );

	/**
	 * @uml.property name="document"
	 * @uml.associationEnd
	 */
	Document getDocument();
}
