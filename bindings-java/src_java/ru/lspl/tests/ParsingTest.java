package ru.lspl.tests;

import ru.lspl.patterns.Pattern;
import ru.lspl.patterns.PatternBuilder;

public class ParsingTest {

	public static void main( String[] args ) throws Exception {
		PatternBuilder b = PatternBuilder.create();
		b.build( "AP = A (A) | Pa (Pa)" );
		b.build( "AN = {AP} N <AP=N> (N)" );
		b.build( "Act = AN V <AN=V>" );
		b.build( "Sng = AN1 {AN2}<1> [\"и\" AN3] <AN1.c=AN2.c,AN1.c=AN3.c> (AN1)" );
		b.build( "Tst = Act \"в\" \"багаж\" Sng" );
		b.build( "Sn = N1 \"за\" N2<c=acc> {N3 \"за\" N4<c=acc>}<1> <N1.c=N3.c>" );
		b.build( "NP = AN1 {AN2<c=gen>} (AN1)" );
		b.build( "DE = NP1<c=acc> [\"мы\"] \"назовем\" NP2<c=ins>" );

		for ( Pattern p : b.getDefinedPatterns() ) {
			System.out.println( p.dump() );
		}
	}
}