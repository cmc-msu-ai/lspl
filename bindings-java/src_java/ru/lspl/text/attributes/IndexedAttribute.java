package ru.lspl.text.attributes;

import java.util.Arrays;

import ru.lspl.LsplObject;

public class IndexedAttribute extends LsplObject {

	public static final int COUNT = 32;

	private static IndexedAttribute[] list = new IndexedAttribute[COUNT];

	public static IndexedAttribute valueOf( int id ) {
		if ( list.length <= id )
			list = Arrays.copyOf( list, list.length + id );

		return list[id] == null ? list[id] = new IndexedAttribute( id ) : list[id];
	}

	private static native String getAbbrevation( int index );

	private static native String getTitle( int index );

	public final String title;
	public final String abbrevation;

	private IndexedAttribute( int id ) {
		super( id );

		this.title = getTitle( id );
		this.abbrevation = getAbbrevation( id );
	}

	public String getAbbrevation() {
		return abbrevation;
	}

	public String getTitle() {
		return title;
	}

	@Override
	public String toString() {
		return title;
	}

}
