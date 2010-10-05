package ru.lspl.utils;

import java.util.AbstractMap;
import java.util.AbstractSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public class ArrayMap<Value> extends AbstractMap<Integer, Value> {

	private static final class Entry<Value> implements Map.Entry<Integer, Value> {

		public final Integer index;
		public Value value;

		public Entry( Integer index, Value value ) {
			this.index = index;
			this.value = value;
		}

		@Override
		public Integer getKey() {
			return index;
		}

		@Override
		public Value getValue() {
			return value;
		}

		@Override
		public Value setValue( Value value ) {
			Value old = this.value;
			this.value = value;
			return old;
		}
	}

	private static final class EntryIterator<Value> implements Iterator<Map.Entry<Integer, Value>> {

		private final Map.Entry<Integer, Value>[] entries;

		private int index = 0;

		public EntryIterator( Map.Entry<Integer, Value>[] entries ) {
			this.entries = entries;
		}

		@Override
		public boolean hasNext() {
			while ( index < entries.length && entries[index] == null )
				++index;

			return index < entries.length;
		}

		@Override
		public Map.Entry<Integer, Value> next() {
			while ( index < entries.length && entries[index] == null )
				++index;

			return entries[index];
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}

	}

	private final class EntrySet extends AbstractSet<Map.Entry<Integer, Value>> {

		private static final long serialVersionUID = 1L;

		@Override
		public Iterator<Map.Entry<Integer, Value>> iterator() {
			return new EntryIterator<Value>( entries );
		}

		@Override
		public int size() {
			return size;
		}
	}

	private final Entry<Value>[] entries;

	private final EntrySet entrySet;

	private int size = 0;

	@SuppressWarnings( "unchecked" )
	public ArrayMap( int size ) {
		entries = new Entry[size];
		entrySet = new EntrySet();
	}

	@Override
	public int size() {
		return size;
	}

	@Override
	public Value get( Object key ) {
		if ( key instanceof Integer ) {
			Entry<Value> entry = entries[(Integer) key];

			if ( entry != null )
				return entry.value;
		}

		return null;
	}

	@Override
	public boolean containsKey( Object key ) {
		return key instanceof Integer && entries[(Integer) key] != null;
	}

	@Override
	public Value put( Integer key, Value value ) {
		Entry<Value> entry = entries[key];

		if ( entry == null ) {
			entries[key] = new Entry<Value>( key, value );

			++size;

			return null;
		} else {
			return entry.setValue( value );
		}
	}

	@Override
	public Set<Map.Entry<Integer, Value>> entrySet() {
		return entrySet;
	}

}
