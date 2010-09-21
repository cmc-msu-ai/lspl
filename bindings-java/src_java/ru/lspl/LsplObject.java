package ru.lspl;

public abstract class LsplObject {

	public final int id;

	private static native void initStatic();

	protected LsplObject( int id ) {
		this.id = id;
	}

	@Override
	protected abstract void finalize();

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

	public static native void setRml( String rml );

	static {
		System.loadLibrary( "lspl" );
		System.loadLibrary( "lspl-java" );

		initStatic();
	}
}