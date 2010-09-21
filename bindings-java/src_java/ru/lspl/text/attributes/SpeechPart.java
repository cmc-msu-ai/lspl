package ru.lspl.text.attributes;

import ru.lspl.LsplObject;

/**
 * @author alno
 */
public class SpeechPart extends LsplObject {
	/**
	 * @uml.property name="aNY"
	 * @uml.associationEnd
	 */
	public static final SpeechPart ANY = new SpeechPart( 0 );
	/**
	 * @uml.property name="nOUN"
	 * @uml.associationEnd
	 */
	public static final SpeechPart NOUN = new SpeechPart( 1 );
	/**
	 * @uml.property name="aDJECTIVE"
	 * @uml.associationEnd
	 */
	public static final SpeechPart ADJECTIVE = new SpeechPart( 2 );
	/**
	 * @uml.property name="vERB"
	 * @uml.associationEnd
	 */
	public static final SpeechPart VERB = new SpeechPart( 3 );
	/**
	 * @uml.property name="pARTICIPLE"
	 * @uml.associationEnd
	 */
	public static final SpeechPart PARTICIPLE = new SpeechPart( 4 );
	/**
	 * @uml.property name="aDVERBAL_PARTICIPLE"
	 * @uml.associationEnd
	 */
	public static final SpeechPart ADVERBAL_PARTICIPLE = new SpeechPart( 5 );
	/**
	 * @uml.property name="pRONOUN"
	 * @uml.associationEnd
	 */
	public static final SpeechPart PRONOUN = new SpeechPart( 6 );
	/**
	 * @uml.property name="aDVERB"
	 * @uml.associationEnd
	 */
	public static final SpeechPart ADVERB = new SpeechPart( 7 );
	/**
	 * @uml.property name="cONJUNCTION"
	 * @uml.associationEnd
	 */
	public static final SpeechPart CONJUNCTION = new SpeechPart( 8 );
	/**
	 * @uml.property name="pREPOSITION"
	 * @uml.associationEnd
	 */
	public static final SpeechPart PREPOSITION = new SpeechPart( 9 );
	/**
	 * @uml.property name="pARTICLE"
	 * @uml.associationEnd
	 */
	public static final SpeechPart PARTICLE = new SpeechPart( 10 );
	/**
	 * @uml.property name="iNTERJECTION"
	 * @uml.associationEnd
	 */
	public static final SpeechPart INTERJECTION = new SpeechPart( 11 );
	/**
	 * @uml.property name="nUMERAL"
	 * @uml.associationEnd
	 */
	public static final SpeechPart NUMERAL = new SpeechPart( 12 );

	/**
	 * @uml.property name="vALUES"
	 * @uml.associationEnd multiplicity="(0 -1)"
	 */
	public static final SpeechPart[] VALUES = new SpeechPart[] { ANY, NOUN, ADJECTIVE, VERB, PARTICIPLE, ADVERBAL_PARTICIPLE, PRONOUN, ADVERB, CONJUNCTION, PREPOSITION, PARTICLE,
			INTERJECTION, NUMERAL };

	/**
	 * @uml.property name="title"
	 */
	public final String title;
	/**
	 * @uml.property name="pluralTitle"
	 */
	public final String pluralTitle;
	/**
	 * @uml.property name="abbrevation"
	 */
	public final String abbrevation;

	public static final SpeechPart[] values() {
		return VALUES;
	}

	/**
	 * Получить наименование части речи
	 * 
	 * @return наименование части речи
	 * @uml.property name="title"
	 */
	public String getTitle() {
		return title;
	}

	/**
	 * Получить наименование части речи во множественном числе
	 * 
	 * @return наименование части речи во множественном числе
	 * @uml.property name="pluralTitle"
	 */
	public String getPluralTitle() {
		return pluralTitle;
	}

	/**
	 * Получить аббревиатуру части речи
	 * 
	 * @return аббревиатура части речи
	 * @uml.property name="abbrevation"
	 */
	public String getAbbrevation() {
		return abbrevation;
	}

	/**
	 * Получить порядковый номер части речи
	 * 
	 * @return порядковый номер части речи
	 */
	public int ordinal() {
		return id;
	}

	@Override
	public String toString() {
		return title;
	}

	@Override
	protected void finalize() {
	}

	private SpeechPart( int id ) {
		super( id );
		this.abbrevation = getAbbrevation( id );
		this.title = getTitle( id );
		this.pluralTitle = getPluralTitle( id );
	}

	private static native String getAbbrevation( int index );

	private static native String getTitle( int index );

	private static native String getPluralTitle( int index );

}
