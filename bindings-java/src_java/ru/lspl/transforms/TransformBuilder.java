package ru.lspl.transforms;

import ru.lspl.patterns.Alternative;

public interface TransformBuilder {

	Transform build( Alternative alternative, String source );

}
