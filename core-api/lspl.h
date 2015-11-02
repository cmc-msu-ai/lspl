// Author: Anton Todua
// Description: интерфейс использования библиотеки lspl.

#ifndef _LSPL_LSPL_H_
#define _LSPL_LSPL_H_

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// База шаблонов.
typedef struct lspl_grammar lspl_grammar; 

// Создаёт базу шаблонов из текстового описания шаблонов.
// Возвращает NULL в случае ошибки или новый объект в динамической памяти,
// для освобождения, которого обязательно использование lspl_free_grammar.
lspl_grammar* lspl_create_grammar( const char* patterns_text );

// Удаляет ранее созданный объект lspl_grammar.
void lspl_free_grammar( lspl_grammar* grammar );

//------------------------------------------------------------------------------
// Шаблон.
typedef struct lspl_pattern lspl_pattern; 

// Возвращает шаблон по его имени или NULL в случае если такого шаблона нет.
const lspl_pattern* lspl_get_pattern_by_name( const lspl_grammar* grammar,
	const char* pattern_name );

//------------------------------------------------------------------------------
// Разметка.
typedef struct lspl_markup lspl_markup; 

// Создаёт разметку из исходного текста с помощью aot.
// Возвращает NULL в случае ошибки или новый объект в динамической памяти,
// для освобождения, которого обязательно использование lspl_free_grammar.
lspl_markup* lspl_create_aot_markup( const char* text );

// Удаляет ранее созданный объект lspl_markup.
void lspl_free_markup( lspl_markup* markup );

// Накладывает шаблон на текст.
// Возврщает новую разметку, с наложенным шаблоном pattern.
// Возвращаемое значение NULL свидетельствует об ошибке.
lspl_markup* lspl_apply( const lspl_pattern* pattern, const lspl_markup* markup );

//------------------------------------------------------------------------------
// Наложение шаблона на текст.
typedef struct lspl_annotation lspl_annotation;

// Возвращают смещение в байтах от начала текста.
// Начало наложения.
int lspl_annotation_start( const lspl_annotation* annotation );
// Конец наложения.
int lspl_annotation_end( const lspl_annotation* annotation );

// Возвращают длину текста аннотации в байтах.
int lspl_annotation_text_length( const lspl_annotation* annotation );

// Записывает текст аннотации в buffer (размер которого передан в buffer_size).
// Возвращает число записанных байт или 0 в случае ошибки.
int lspl_annotation_text( const lspl_annotation* annotation,
	char* buffer, int buffer_size );

// Возвращает не 0 если атрибут с именем attribute_name есть у аннотации.
int lspl_annotation_has_attribute( const char* attribute_name );

// Возращает строковое значение атрибута аннотации или NULL если такого атрибута нет.
const char* lspl_annotation_attribute_value( const char* attribute_name );

//------------------------------------------------------------------------------
// Итератор для обхода аннотаций.
typedef struct lspl_annotation_iterator lspl_annotation_iterator;

// Создаёт итератор для заданной разметки. Возвращает NULL в случае ошибки.
lspl_annotation_iterator* lspl_create_annotation_iterator( lspl_markup* markup );

// Удаляет ранее созданный annotation_iterator.
void lspl_free_annotation_iterator( lspl_annotation_iterator* annotation_iterator );

// Возврщает аннотацию по интератору или NULL если все аннотации пройдены.
const lspl_annotation* lspl_annotation_iterator_get(
	const lspl_annotation_iterator* annotation_iterator );

// Сдвигает итератор на следующую аннотацию.
void lspl_annotation_iterator_next( lspl_annotation_iterator* annotation_iterator );

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // _LSPL_LSPL_H_
