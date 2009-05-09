package ru.lspl.patterns;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.LsplObject;

/**
 * @author  alno
 */
public class PatternBuilder extends LsplObject {

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
	
	/**
	 * @uml.property  name="definedPatterns"
	 */
	public final List<Pattern> definedPatterns = new DefinedPatternList();
	
	public static native PatternBuilder create();
				
	public native int getDefinedPatternCount();
	
	public native Pattern getDefinedPattern( int i );
	
	public native Pattern getDefinedPattern( String name );
	
	/**
	 * @return
	 * @uml.property  name="definedPatterns"
	 */
	public List<Pattern> getDefinedPatterns() {
		return definedPatterns;
	}
	
	public Pattern[] getDefinedPatternsArray() {
		Pattern[] patterns = new Pattern[ getDefinedPatternCount() ];
		
		for ( int i = 0; i < patterns.length; ++ i )
			patterns[ i ] = getDefinedPattern( i );
		
		return patterns;
	}
		
	public native void build( String s ) throws PatternBuildingException;

	protected native void finalize();	

	private PatternBuilder( int id ) {
		super( id );
	}
}