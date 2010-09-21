package ru.lspl.patterns;

/**
 * Исключение, выбрасываемое в случае ошибки разбора шаблона
 * 
 * @author alno
 */
public class PatternBuildingException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public PatternBuildingException( String text ) {
		super( text );
	}

}