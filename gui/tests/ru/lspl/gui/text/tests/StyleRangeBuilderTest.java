package ru.lspl.gui.text.tests;

import org.eclipse.swt.custom.StyleRange;
import org.junit.Test;

import ru.lspl.gui.text.StyleRangeBuilder;

public class StyleRangeBuilderTest {

	@Test
	public void testRangeBuilding() {
		StyleRangeBuilder rangeBuilder = new StyleRangeBuilder();
		
		StyleRange[] initialRanges = new StyleRange[ 2 ];
		
		initialRanges[0] = newRange( rangeBuilder, 2, 4 );
		//initialRanges[1] = newRange( rangeBuilder, 1, 3 );		
		initialRanges[1] = newRange( rangeBuilder, 5, 7 );
		
		StyleRange[] ranges = rangeBuilder.buildRanges( initialRanges );
		
		for ( StyleRange range : ranges )
			System.out.println( range.start + " - " + ( range.start + range.length ) + " : " + ( range.background == rangeBuilder.multiMatchColor ) );
	}

	private StyleRange newRange(StyleRangeBuilder rangeBuilder, int i, int j) {
		StyleRange rng = new StyleRange();
		rng.start = i;
		rng.length = j - i;
		rng.background = rangeBuilder.singleMatchColor;
		return rng;
	}
}
