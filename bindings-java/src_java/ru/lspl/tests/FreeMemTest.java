package ru.lspl.tests;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import ru.lspl.LsplObject;
import ru.lspl.patterns.Pattern;
import ru.lspl.patterns.PatternBuilder;
import ru.lspl.patterns.PatternBuildingException;
import ru.lspl.text.Node;
import ru.lspl.text.Text;
import ru.lspl.text.Transition;

public class FreeMemTest {

	public static void main( String[] args ) throws Exception {
		String t1 = "Дама сдавала в багаж диван, чемодан, саквояж, картину, корзину, картонку и маленькую собаченку";
		String text = extractText( new FileInputStream( "/home/alno/test1000.txt" ) );

		for ( int i = 0; i < 1000; ++i ) {
			doTest( text );
			System.gc();
			Thread.sleep( 1000 );
			System.err.println( LsplObject.dumpMemoryStats() );
		}
	}

	private static void doTest( String text ) throws PatternBuildingException, InterruptedException {
		Text t1 = Text.create( text );
		PatternBuilder b = PatternBuilder.create();

		b.build( "Act = N V (N)" );

		for ( Pattern p : b.getDefinedPatterns() )
			t1.getMatches( p ).size();

		for ( int i = 0; i < 15; ++i ) {
			for ( Node n : t1.getNodes() )
				for ( Transition t : n.getTransitions() ) {
					Object o = t.getAttribute( 1 );
					if ( o != null )
						o.toString();
				}

			//t.dump();
			System.out.println( "C" + i );
			Thread.sleep( 1000 );
		}
	}

	private static String extractText( InputStream is ) throws IOException {
		byte[] data = new byte[is.available()];
		is.read( data );
		is.close();

		return new String( data );
	}
}