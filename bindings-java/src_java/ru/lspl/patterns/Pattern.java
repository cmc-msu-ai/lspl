package ru.lspl.patterns;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.LsplObject;

/**
 * @author  alno
 */
public class Pattern extends LsplObject {
	
	private class AlternativeList extends AbstractList<Alternative> {

		@Override
		public int size() {
			return getAlternativeCount();
		}

		@Override
		public Alternative[] toArray() {
			Alternative[] alts = new Alternative[getAlternativeCount()];

			for (int i = 0; i < alts.length; ++i)
				alts[i] = getAlternative(i);

			return alts;
		}

		@Override
		public Alternative get(int index) {
			return getAlternative( index );
		}
	}

	private class DependencyList extends AbstractList<Pattern> {

		@Override
		public int size() {
			return getDependencyCount();
		}

		@Override
		public Pattern[] toArray() {
			Pattern[] res = new Pattern[getDependencyCount()];

			for (int i = 0; i < res.length; ++i)
				res[i] = getDependency(i);

			return res;
		}

		@Override
		public Pattern get(int index) {
			return getDependency( index );
		}
	}
	
	public final List<Alternative> alternatives = new AlternativeList();

	public final List<Pattern> dependencies = new DependencyList();
	
	public final String name;
	
	public native int getAlternativeCount();
	
	public native Alternative getAlternative( int index );
	
	/**
	 * @return
	 * @uml.property  name="alternatives"
	 */
	public List<Alternative> getAlternatives() {
		return alternatives;
	}

	public native int getDependencyCount();

	public native Pattern getDependency( int index );

	public List<Pattern> getDependencies() {
		return dependencies;
	}
	
	public native String getSource();
	
	public native String dump();
	
	protected native void finalize();	

	private Pattern( int id, String name ) {
		super( id );
		this.name = name;
	}
}