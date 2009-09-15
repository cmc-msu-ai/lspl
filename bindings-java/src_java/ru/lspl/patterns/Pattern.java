package ru.lspl.patterns;

import java.util.AbstractList;
import java.util.List;

import ru.lspl.LsplObject;

/**
 * Класс, представлющий LSPL-шаблон.
 * 
 * @author  alno
 */
public class Pattern extends LsplObject {
	
	private class AlternativeList extends AbstractList<Alternative> {

		@Override
		public int size() {
			return getAlternativeCount();
		}

		@Override
		public Alternative[] toArray() {
			Alternative[] alts = new Alternative[getAlternativeCount()];

			for (int i = 0; i < alts.length; ++i)
				alts[i] = getAlternative(i);

			return alts;
		}

		@Override
		public Alternative get(int index) {
			return getAlternative( index );
		}
	}

	private class DependencyList extends AbstractList<Pattern> {

		@Override
		public int size() {
			return getDependencyCount();
		}

		@Override
		public Pattern[] toArray() {
			Pattern[] res = new Pattern[getDependencyCount()];

			for (int i = 0; i < res.length; ++i)
				res[i] = getDependency(i);

			return res;
		}

		@Override
		public Pattern get(int index) {
			return getDependency( index );
		}
	}
	
	/**
	 * Альтернативы шаблона
	 */
	public final List<Alternative> alternatives = new AlternativeList();

	/**
	 * Зависимости шаблона
	 */
	public final List<Pattern> dependencies = new DependencyList();
	
	/**
	 * Имя шаблона
	 */
	public final String name;
	
	/**
	 * Получить количество альтернатив шаблона
	 * 
	 * @return количество альтернатив
	 */
	public native int getAlternativeCount();
	
	/**
	 * Получить альтернативу шаблона по индексу
	 * 
	 * @param index индекс альтернативы шаблона
	 * @return альтернатива шаблона
	 */
	public native Alternative getAlternative( int index );
	
	/**
	 * Получить список альтернатив шаблона
	 * 
	 * @return список альтернатив
	 */
	public List<Alternative> getAlternatives() {
		return alternatives;
	}

	/**
	 * Получить количество зависмостей шаблона
	 * 
	 * @return количество зависимостей
	 */
	public native int getDependencyCount();

	/**
	 * Получить зависимость шаблона по индексу
	 * 
	 * @param index индекс зависимости
	 * @return зависимость шаблона
	 */
	public native Pattern getDependency( int index );

	/**
	 * Получить зависимости шаблона
	 * 
	 * @return список зависимостей шаблона
	 */
	public List<Pattern> getDependencies() {
		return dependencies;
	}
	
	/**
	 * Получить исходный код шаблона
	 */
	public native String getSource();
	
	/**
	 * Получить отладочное представление шаблона
	 */
	public native String dump();
	
	/**
	 * Удалить альтернативы-дубликаты
	 */
	public native void removeDuplicateAlternatives();
	
	protected native void finalize();	

	private Pattern( int id, String name ) {
		super( id );
		this.name = name;
	}
}