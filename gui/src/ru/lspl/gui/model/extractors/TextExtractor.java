package ru.lspl.gui.model.extractors;

import java.io.IOException;
import java.io.InputStream;

public interface TextExtractor {

	String extractText( InputStream is ) throws IOException;
	
}
