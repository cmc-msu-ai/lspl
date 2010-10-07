package ru.lspl.gui.model.extractors;

import java.io.IOException;
import java.io.InputStream;

import org.apache.poi.extractor.ExtractorFactory;
import org.apache.poi.openxml4j.exceptions.InvalidFormatException;
import org.apache.poi.openxml4j.exceptions.OpenXML4JException;
import org.apache.xmlbeans.XmlException;

public class OoxmlExtractor implements TextExtractor {

	@Override
	public String extractText( InputStream is ) throws IOException {
		try {
			return ExtractorFactory.createExtractor( is ).getText();
		} catch ( InvalidFormatException e ) {
			throw new IOException( "Wrong format", e );
		} catch ( OpenXML4JException e ) {
			throw new IOException( "Wrong format", e );
		} catch ( XmlException e ) {
			throw new IOException( "Wrong format", e );
		}
	}

	@Override
	public boolean isLossless() {
		return false;
	}

}
