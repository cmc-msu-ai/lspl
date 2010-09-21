package ru.lspl.patterns;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.LsplObject;
import ru.lspl.Namespace;
import ru.lspl.transforms.TransformBuilder;

/**
 * @author alno
 */
public class PatternBuilder extends LsplObject {

	private class DefinedPatternList extends AbstractList<Pattern> {

		@Override
		public int size() {
			return getDefinedPatternCount();
		}

		@Override
		public Pattern[] toArray() {
			return getDefinedPatternsArray();
		}

		@Override
		public Pattern get( int index ) {
			return getDefinedPattern( index );
		}
	}

	/**
	 * @uml.property name="definedPatterns"
	 */
	public final List<Pattern> definedPatterns = new DefinedPatternList();

	public static native PatternBuilder create( Namespace ns, TransformBuilder tb );

	public static PatternBuilder create( Namespace ns ) {
		return create( ns, null );
	}

	public static PatternBuilder create( TransformBuilder tb ) {
		return create( Namespace.create( new Namespace[0] ), tb );
	}

	public static PatternBuilder create() {
		return create( Namespace.create( new Namespace[0] ), null );
	}

	public native int getDefinedPatternCount();

	public native Pattern getDefinedPattern( int i );

	public native Pattern getDefinedPattern( String name );

	public native Namespace getNamespace();

	public List<Pattern> getDefinedPatterns() {
		return definedPatterns;
	}

	public Pattern[] getDefinedPatternsArray() {
		Pattern[] patterns = new Pattern[getDefinedPatternCount()];

		for ( int i = 0; i < patterns.length; ++i )
			patterns[i] = getDefinedPattern( i );

		return patterns;
	}

	public native void build( String s ) throws PatternBuildingException;

	@Override
	protected native void finalize();

	private PatternBuilder( int id ) {
		super( id );
	}
}