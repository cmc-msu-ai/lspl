package ru.lspl.tests;

import ru.lspl.text.Node;
import ru.lspl.text.Text;
import ru.lspl.text.Transition;
import ru.lspl.text.Word;

public class TransitionsTest {

	public static void main( String[] args ) {

		Text t1 = Text.create( "Дама сдавала в багаж диван, чемодан, саквояж, картину, корзину, картонку и маленькую собаченку" );

		for ( Node n : t1.getNodes() ) {
			for ( Transition tr : n.getTransitions() ) {
				if ( tr instanceof Word ) {
					System.out.print( ((Word) tr).form + "/" + ((Word) tr).base + ": " );
				}
				System.out.println( tr.dump() );
			}
		}
	}
}