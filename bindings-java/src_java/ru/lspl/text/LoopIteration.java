package ru.lspl.text;

import ru.lspl.LsplObject;

import java.util.List;
import java.util.AbstractList;

/**
 * Итерация цикла в тексте
 * 
 * @author alno
 */
public class LoopIteration extends Transition {
		
	private class VariantList extends AbstractList<LoopIterationVariant> {
		
		@Override
		public int size() {
			return getVariantCount();
		}

		@Override
		public LoopIterationVariant[] toArray() {
			LoopIterationVariant[] ts = new LoopIterationVariant[size()];

			for (int i = 0; i < ts.length; ++i)
				ts[i] = getVariant(i);

			return ts;
		}

		@Override
		public LoopIterationVariant get(int index) {
			return getVariant( index );
		}
		
	};
	
	public final List<LoopIterationVariant> variants = new VariantList();
	
	private LoopIteration(int id, Text text, Node start, Node end) {
		super(id, text, start, end);
	}
	
	public List<LoopIterationVariant> getVariants() {
		return variants;
	}
	
	/**
	 * Получить количество вариантов сопоставления
	 */
	public native int getVariantCount();
	
	/**
	 * Получить вариант сопоставления по его индексу
	 */
	public native LoopIterationVariant getVariant( int variantIndex );
		
	public native int getVariantTransitionCount( int variantIndex );
	
	public native Transition getVariantTransition( int variantIndex, int transitionIndex );
	
	native void finalizeVariant( int variantIndex );
	
}