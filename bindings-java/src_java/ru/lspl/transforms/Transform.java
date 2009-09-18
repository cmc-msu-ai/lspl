package ru.lspl.transforms;

import ru.lspl.text.MatchVariant;

public interface Transform {
	
	Object apply( MatchVariant matchVariant );
	
}
