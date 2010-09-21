package ru.lspl.utils;

import java.io.Serializable;
import java.util.AbstractList;
import java.util.Arrays;
import java.util.Collection;
import java.util.RandomAccess;

public class ImmutableArrayList<E> extends AbstractList<E> implements RandomAccess, Serializable {

	private static final long serialVersionUID = 1L;

	private final E[] array;

	public ImmutableArrayList( E... array ) {
		if ( array == null )
			throw new NullPointerException();

		this.array = array;
	}

	@SuppressWarnings( "unchecked" )
	public ImmutableArrayList( Collection<E> collection ) {
		if ( collection == null )
			throw new NullPointerException();

		this.array = (E[]) collection.toArray();
	}

	@Override
	public int size() {
		return array.length;
	}

	@Override
	public Object[] toArray() {
		return array.clone();
	}

	@SuppressWarnings( "unchecked" )
	@Override
	public <T> T[] toArray( T[] a ) {
		int size = size();
		if ( a.length < size )
			return Arrays.copyOf( this.array, size, (Class<? extends T[]>) a.getClass() );
		System.arraycopy( this.array, 0, a, 0, size );
		if ( a.length > size )
			a[size] = null;
		return a;
	}

	@Override
	public E get( int index ) {
		return array[index];
	}

	@Override
	public int indexOf( Object o ) {
		if ( o == null ) {
			for ( int i = 0; i < array.length; i++ )
				if ( array[i] == null )
					return i;
		} else {
			for ( int i = 0; i < array.length; i++ )
				if ( o.equals( array[i] ) )
					return i;
		}
		return -1;
	}

	@Override
	public boolean contains( Object o ) {
		return indexOf( o ) != -1;
	}

}
