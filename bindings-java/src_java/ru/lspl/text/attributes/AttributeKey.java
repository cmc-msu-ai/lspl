package ru.lspl.text.attributes;

import java.util.Arrays;

import ru.lspl.LsplObject;

/**
 * @author  alno
 */
public class AttributeKey extends LsplObject {

	/**
	 * @uml.property  name="list"
	 * @uml.associationEnd  multiplicity="(0 -1)"
	 */
	private static AttributeKey[] list = new AttributeKey[10];

	public static AttributeKey valueOf( int id ) {
		if ( list.length <= id )
			list = Arrays.copyOf( list, list.length + id );

		return list[id] == null ? list[id] = new AttributeKey( id ) : list[id];
	}

	private static native String getAbbrevation( int index );

	private static native String getName( int index );

	private static native String getTitle( int index );

	/**
	 * @uml.property  name="abbrevation"
	 */
	public final String abbrevation;

	/**
	 * @uml.property  name="name"
	 */
	public final String name;

	/**
	 * @uml.property  name="title"
	 */
	public final String title;

	/**
	 * @return
	 * @uml.property  name="abbrevation"
	 */
	public String getAbbrevation() {
		return abbrevation;
	}

	/**
	 * @return
	 * @uml.property  name="name"
	 */
	public String getName() {
		return name;
	}

	/**
	 * @return
	 * @uml.property  name="title"
	 */
	public String getTitle() {
		return title;
	}

	public String toString() {
		return title;
	}

	@Override
	protected void finalize() {
	}

	private AttributeKey( int id ) {
		super( id );

		name = getName( id );
		title = getTitle( id );
		abbrevation = getAbbrevation( id );
	}

}
