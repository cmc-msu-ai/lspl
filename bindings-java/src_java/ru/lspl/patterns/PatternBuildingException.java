package ru.lspl.patterns;

/**
 * Исключение, выбрасываемое в случае ошибки разбора шаблона
 * @author  alno
 */
public class PatternBuildingException extends Exception {

	public PatternBuildingException(String text) {
		super(text);
	}
	
}