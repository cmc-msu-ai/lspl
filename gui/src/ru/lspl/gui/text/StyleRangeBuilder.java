package ru.lspl.gui.text;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Comparator;

import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.widgets.Display;

import ru.lspl.text.Transition;

public class StyleRangeBuilder {
	
	public final Color singleMatchColor;
	public final Color multiMatchColor;
	
	/**
	 * @author  alno
	 */
	private static class RangeStartComparator implements Comparator<StyleRange> {
		
		/**
		 * @uml.property  name="iNSTANCE"
		 * @uml.associationEnd  
		 */
		public static final RangeStartComparator INSTANCE = new RangeStartComparator();
		
		private RangeStartComparator() {}

		@Override
		public int compare(StyleRange r1, StyleRange r2) {
			int diff = r1.start - r2.start;
			
			if ( diff < 0 )
				return -1;
			else if ( diff > 0 )
				return 1;
			else
				return 0;
		}
	};
	
	public StyleRangeBuilder(Color singleMatchColor, Color multiMatchColor) {
		this.singleMatchColor = singleMatchColor;
		this.multiMatchColor = multiMatchColor;
	}

	public StyleRangeBuilder(Display display) {
		this.singleMatchColor = new Color( display, 255, 255, 150 );
		this.multiMatchColor = new Color( display, 220, 220, 120 );
	}
	
	public StyleRangeBuilder() {
		this( Display.getCurrent() );
	}

	public StyleRange[] buildRanges(Collection<? extends Transition> transitions) {
		StyleRange[] ranges = new StyleRange[ transitions.size() ];
		
		int i = 0;
		for( Transition transition : transitions ) {
			StyleRange rng = new StyleRange();
			rng.start = transition.start.endOffset;
			rng.length = transition.end.startOffset - transition.start.endOffset;
			rng.background = singleMatchColor;
			ranges[i++] = rng;
		}
		
		return buildRanges( ranges );
	}
	
	public StyleRange[] buildRanges( Transition[] transitions ) {
		StyleRange[] ranges = new StyleRange[ transitions.length ];
		
		for ( int i = 0; i < transitions.length; ++ i ) {
			Transition transition = transitions[i];
			StyleRange rng = new StyleRange();
			rng.start = transition.start.endOffset;
			rng.length = transition.end.startOffset - rng.start;
			rng.background = singleMatchColor;
			ranges[i] = rng;
		}
		
		return buildRanges( ranges );
	}
	
	public StyleRange[] buildRanges( StyleRange[] rangesArr ) {
		Arrays.sort(rangesArr, RangeStartComparator.INSTANCE);
		
		ArrayList<StyleRange> ranges = new ArrayList<StyleRange>();
		
		int start = -1;
		int singleEnd = -1;
		int multiEnd = -1;
		for (StyleRange range : rangesArr) {
			int rangeEnd = range.start + range.length;
			
			if ( start < 0 ) {
				start = range.start;
				singleEnd = rangeEnd;
			} else if (range.start <= multiEnd) { // ==|==--
				multiEnd = Math.min( singleEnd, rangeEnd );
				singleEnd = Math.max( singleEnd, rangeEnd );
			} else if (range.start <= singleEnd) { // ==--| //--|
				addRanges(ranges, start, multiEnd, range.start);				
				start = range.start;
				multiEnd = Math.min( singleEnd, rangeEnd );
				singleEnd = Math.max( singleEnd, rangeEnd );
			} else { // ==-- |
				addRanges(ranges, start, multiEnd, singleEnd);				
				start = range.start;
				multiEnd = -1;
				singleEnd = rangeEnd;
			}
		}
		
		addRanges(ranges, start, multiEnd, singleEnd);
		
		return ranges.toArray(new StyleRange[ ranges.size() ] );
	}

	private void addRanges(ArrayList<StyleRange> ranges, int start,
			int multiEnd, int singleEnd) {
		if ( multiEnd >= 0 ) {
			addMultiRange( ranges, start, multiEnd ); // Добавляем новый промежуток множественного наложения
			addSingleRange( ranges, multiEnd, singleEnd ); // Добавляем новый промежуток одиночного наложения
		} else {
			addSingleRange( ranges, start, singleEnd ); // Добавляем новый промежуток одиночного наложения
		}
	}
	
	private void addSingleRange(ArrayList<StyleRange> ranges, int singleStart,
			int singleEnd) {
		if ( singleStart >= singleEnd )
			return;
		
		StyleRange rng = new StyleRange();
		rng.start = singleStart;
		rng.length = singleEnd - singleStart;
		rng.background = singleMatchColor;
		ranges.add(rng);
	}

	private void addMultiRange(ArrayList<StyleRange> ranges, int multiStart,
			int multiEnd) {
		if ( multiStart >= multiEnd )
			return;
		
		StyleRange rng = new StyleRange();
		rng.start = multiStart;
		rng.length = multiEnd - multiStart;
		rng.background = multiMatchColor;		
		ranges.add(rng);
	}
}
