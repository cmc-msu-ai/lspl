package ru.lspl.text.attributes;

import ru.lspl.LsplObject;

public class IndexedAttribute extends LsplObject {

	public IndexedAttribute( int id ) {
		super( id );
	}
	
	public String getAbbrevation() {
		return getAbbrevation( id );
	}
	
	public String getTitle() {
		return getTitle( id );
	}
	
	@Override
	public String toString() {
		return getTitle( id );
	}

	@Override
	protected void finalize() {
	}
	
	private static native String getAbbrevation( int index );

	private static native String getTitle( int index );

}
