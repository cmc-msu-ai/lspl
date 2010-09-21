package ru.lspl.text.attributes;

import java.util.Map;

public interface AttributeContainer {

	Object getAttribute( int key );

	Map<Integer, Object> getAttributes();
}
