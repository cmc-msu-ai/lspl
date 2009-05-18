package ru.lspl.gui.model.extractors;

import java.io.IOException;
import java.io.InputStream;

public class PlainTextExtractor implements TextExtractor {

	@Override
	public String extractText( InputStream is ) throws IOException {
		byte[] data = new byte[ is.available() ];
		is.read( data );
		
		return new String( data );
	}
	
	@Override
	public boolean isLossless() {
		return true;
	}

}
