package ru.lspl.patterns;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.LsplObject;

/**
 * @author alno
 */
public class Alternative extends LsplObject {

	private class DependencyList extends AbstractList<Pattern> {

		@Override
		public int size() {
			return getDependencyCount();
		}

		@Override
		public Pattern[] toArray() {
			Pattern[] res = new Pattern[getDependencyCount()];

			for ( int i = 0; i < res.length; ++i )
				res[i] = getDependency( i );

			return res;
		}

		@Override
		public Pattern get( int index ) {
			return getDependency( index );
		}
	}

	public final List<Pattern> dependencies = new DependencyList();

	public final Pattern pattern;

	public native int getMatcherCount();

	public native String getSource();

	public native String dump();

	public native int getDependencyCount();

	public native Pattern getDependency( int index );

	public List<Pattern> getDependencies() {
		return dependencies;
	}

	@Override
	protected native void finalize();

	private Alternative( int id, Pattern pattern ) {
		super( id );
		this.pattern = pattern;
	}
}