package ru.lspl.tests;

import ru.lspl.patterns.Pattern;
import ru.lspl.patterns.PatternBuilder;
import ru.lspl.text.Match;
import ru.lspl.text.Loop;
import ru.lspl.text.MatchVariant;
import ru.lspl.text.Transition;
import ru.lspl.text.Text;

import java.util.List;

public class MatchingTest {
	
	public static void main(String[] args) throws Exception {		
		PatternBuilder b = PatternBuilder.create();
		b.build( "Act = N V");
		b.build( "AP = A (A) | Pa (Pa)");
		b.build( "AN = {AP} N <AP=N> (N)" );
		b.build( "Act = AN V <AN=V>" );
		b.build( "Sng = AN1 {AN2}<1> [\"и\" AN3] <AN1.c=AN2.c,AN1.c=AN3.c> (AN1)" );
		b.build( "Tst = Act \"в\" \"багаж\" Sng" );
		b.build( "Sn = N1 \"за\" N2<c=acc> {N3 \"за\" N4<c=acc>}<1> <N1.c=N3.c>" );
		b.build( "NP = AN1 {AN2<c=gen>} (AN1)" );
		b.build( "DE = NP1<c=acc> [\"мы\"] \"назовем\" NP2<c=ins>" );
		
		Text t0 = Text.create( "Мама мыла рму" );
		Text t1 = Text.create( "Дама сдавала в багаж диван, чемодан, саквояж, картину, корзину, картонку и маленькую собаченку" );		
		Text t2 = Text.create( "Мышка за кошку, кошка за Жучку, Жучка за внучку, внучка за бабку, бабка за дедку, дедка за репку" );		
		Text t3 = Text.create( "Указанную операцию назовем операцией поиска примеров, фразу Поддержку динамичности изменения доступного информационного пространства мы назовем динамичностью информационной модели" );
				
		for ( Pattern p : b.getDefinedPatterns() ) {
			System.out.println( p.dump() );
			t0.getMatches( p );
			t1.getMatches( p );
			t2.getMatches( p );
			t3.getMatches( p );			

			printTransitions( t0.getMatches( p ) );
		}
		
		System.out.println( t0.dump() );
		System.out.println( t1.dump() );
		System.out.println( t2.dump() );
		System.out.println( t3.dump() );
	}
	
	public static void printTransitions( List<? extends Transition> trans ) {
		for ( Transition t : trans ) {
			System.out.println( t.dump() );
			
			if ( t instanceof Match ) {
				for ( MatchVariant v : ((Match)t).getVariants() )
					printTransitions( v.getTransitions() );
			} else if ( t instanceof Loop ) {
				printTransitions( ((Loop)t).getTransitions() );
			}
		}
	}
}