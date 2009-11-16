package ru.lspl.text;

import ru.lspl.patterns.Pattern;

import java.util.List;
import java.util.AbstractList;

/**
 * Цикл в тексте
 * 
 * @author alno
 */
public class Loop extends Transition {
	
	private class IterationList extends AbstractList<LoopIteration> {
		
		@Override
		public int size() {
			return getIterationCount();
		}

		@Override
		public LoopIteration[] toArray() {
			LoopIteration[] ts = new LoopIteration[size()];

			for (int i = 0; i < ts.length; ++i)
				ts[i] = get(i);

			return ts;
		}

		@Override
		public LoopIteration get(int index) {
			return getIteration( index );
		}
		
	};
	
	public final List<LoopIteration> iterations = new IterationList();
	
	/**
	 * Количество повторений, произведенных в цикле
	 */
	public final int repeatCount;

	private Loop(int id, Text text, Node start, Node end, int repeatCount) {
		super(id, text, start, end);
		this.repeatCount = repeatCount;
	}

	public List<LoopIteration> getIterations() {
		return iterations;
	}
	
	public native int getIterationCount();
	
	public native LoopIteration getIteration(int index);
	
}