package ru.lspl;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.LsplObject;
import ru.lspl.patterns.Pattern;

/**
 * @author  alno
 */
public class Namespace extends LsplObject {

	private class DefinedPatternList extends AbstractList<Pattern> {

		public int size() {
			return getDefinedPatternCount();
		}

		public Pattern[] toArray() {
			return getDefinedPatternsArray();
		}

		@Override
		public Pattern get(int index) {
			return getDefinedPattern( index );
		}
	}
	
	public final List<Pattern> definedPatterns = new DefinedPatternList();
	
	public static native Namespace create( Namespace[] parents );
	
	public static Namespace create() {
		return create( new Namespace[0] );
	}
	
	public static Namespace create( Namespace parent ) {
		return create( new Namespace[] { parent } );
	}
				
	public native int getDefinedPatternCount();
	
	public native Pattern getDefinedPattern( int i );
	
	public native Pattern getDefinedPattern( String name );
	
	public List<Pattern> getDefinedPatterns() {
		return definedPatterns;
	}
	
	public Pattern[] getDefinedPatternsArray() {
		Pattern[] patterns = new Pattern[ getDefinedPatternCount() ];
		
		for ( int i = 0; i < patterns.length; ++ i )
			patterns[ i ] = getDefinedPattern( i );
		
		return patterns;
	}
	
	protected native void finalize();	

	private Namespace( int id ) {
		super( id );
	}
}