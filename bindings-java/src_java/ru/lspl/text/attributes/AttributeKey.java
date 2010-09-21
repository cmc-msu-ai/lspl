package ru.lspl.text.attributes;

import java.util.Arrays;

import ru.lspl.LsplObject;

/**
 * @author alno
 */
public class AttributeKey extends LsplObject {

	private static AttributeKey[] list = new AttributeKey[10];

	public static AttributeKey valueOf( int id ) {
		if ( list.length <= id )
			list = Arrays.copyOf( list, list.length + id );

		return list[id] == null ? list[id] = new AttributeKey( id ) : list[id];
	}

	private static native String getAbbrevation( int index );

	private static native String getName( int index );

	private static native String getTitle( int index );

	public final String abbrevation;

	public final String name;

	public final String title;

	public String getAbbrevation() {
		return abbrevation;
	}

	public String getName() {
		return name;
	}

	public String getTitle() {
		return title;
	}

	@Override
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
