package ru.lspl.tests;

import ru.lspl.text.Node;
import ru.lspl.text.Text;

public class NodesTest {

	public static void main( String[] args ) {

		Text t1 = Text.create( "Дама сдавала в багаж диван, чемодан, саквояж, картину, корзину, картонку и маленькую собаченку" );

		for ( Node n : t1.getNodes() ) {
			System.out.println( n.startOffset + "-" + n.endOffset + ": " + n.dump() );
		}
	}
}