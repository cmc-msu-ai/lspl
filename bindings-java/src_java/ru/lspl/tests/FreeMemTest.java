package ru.lspl.tests;

import ru.lspl.text.Node;
import ru.lspl.text.Text;
import ru.lspl.text.Transition;

public class FreeMemTest {

	public static void main( String[] args ) throws InterruptedException {

		for ( int i = 0; i < 10000; ++i )
			doTest();

		System.gc();

		Thread.sleep( 100000 );
	}

	private static void doTest() {
		Text t1 = Text.create( "Дама сдавала в багаж диван, чемодан, саквояж, картину, корзину, картонку и маленькую собаченку" );

		for ( Node n : t1.getNodes() )
			for ( Transition t : n.getTransitions() )
				t.dump();
	}
}