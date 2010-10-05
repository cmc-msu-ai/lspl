package ru.lspl.tests;

import ru.lspl.text.Text;
import ru.lspl.text.Word;
import ru.lspl.text.attributes.SpeechPart;

public class WordsTest {

	public static void main( String[] args ) {

		Text t1 = Text.create( "Дама сдавала в багаж диван, чемодан, саквояж, картину, корзину, картонку и маленькую собаченку" );

		for ( int i = 0; i < 12; ++i ) {
			System.out.println( "== " + SpeechPart.values()[i] + " ==" );
			for ( Word w : t1.getWords( SpeechPart.values()[i] ) ) {
				System.out.println( w.form + "/" + w.base + "/" + w.speechPart + ": " + w.dump() );
			}
		}
	}
}