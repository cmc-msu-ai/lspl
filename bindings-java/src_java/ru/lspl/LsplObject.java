package ru.lspl;

public abstract class LsplObject {

	public final int id;

	public static native String dumpMemoryStats();

	public static native void setRml( String rml );

	private static native void initStatic();

	static {
		System.loadLibrary( "lspl" );
		System.loadLibrary( "lspl-java" );

		initStatic();
	}

	protected LsplObject( int id ) {
		this.id = id;
	}

	@Override
	public boolean equals( Object o ) {
		if ( o == this )
			return true;
		if ( o == null )
			return false;
		if ( !o.getClass().equals( getClass() ) )
			return false;

		return id == ((LsplObject) o).id;
	}

}