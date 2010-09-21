package ru.lspl;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.patterns.Pattern;
import ru.lspl.patterns.PatternBuilder;
import ru.lspl.transforms.TransformBuilder;

/**
 * @author alno
 */
public class Namespace extends LsplObject {

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

	public final List<Pattern> definedPatterns = new DefinedPatternList();

	public static native Namespace create( Namespace... parents );

	public PatternBuilder createPatternBuilder() {
		return PatternBuilder.create( this );
	}

	public PatternBuilder createPatternBuilder( TransformBuilder tb ) {
		return PatternBuilder.create( this, tb );
	}

	/**
	 * Получить кол-во определенных в этом пространстве имен шаблонов
	 */
	public native int getDefinedPatternCount();

	/**
	 * Получить определенный шаблон по индексу
	 */
	public native Pattern getDefinedPattern( int i );

	/**
	 * Получить определенный шаблон по имени. При этом поиску осуществляется и
	 * по родительским пространствам имен.
	 */
	public native Pattern getDefinedPattern( String name );

	public List<Pattern> getDefinedPatterns() {
		return definedPatterns;
	}

	public Pattern[] getDefinedPatternsArray() {
		Pattern[] patterns = new Pattern[getDefinedPatternCount()];

		for ( int i = 0; i < patterns.length; ++i )
			patterns[i] = getDefinedPattern( i );

		return patterns;
	}

	@Override
	protected native void finalize();

	private Namespace( int id ) {
		super( id );
	}
}