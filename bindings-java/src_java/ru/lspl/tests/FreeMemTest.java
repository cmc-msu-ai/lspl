package ru.lspl.tests;

import ru.lspl.LsplObject;
import ru.lspl.patterns.Pattern;
import ru.lspl.patterns.PatternBuilder;
import ru.lspl.patterns.PatternBuildingException;
import ru.lspl.text.Node;
import ru.lspl.text.Text;
import ru.lspl.text.Transition;

public class FreeMemTest {

	public static void main( String[] args ) throws Exception {

		for ( int i = 0; i < 1000; ++i ) {
			doTest();
			System.gc();
			Thread.sleep( 1000 );
			System.err.println( LsplObject.dumpMemoryStats() );
		}
	}

	private static void doTest() throws PatternBuildingException {
		Text t1 = Text.create( "Дама сдавала в багаж диван, чемодан, саквояж, картину, корзину, картонку и маленькую собаченку" );
		PatternBuilder b = PatternBuilder.create();
		
		b.build( "Act = N V (N)" );
		
		for ( Pattern p : b.getDefinedPatterns() )
			t1.getMatches( p ).size();

		for ( Node n : t1.getNodes() )
			for ( Transition t : n.getTransitions() )
				t.dump();
	}
}