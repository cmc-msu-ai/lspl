package ru.lspl.gui.model.extractors;

import java.io.IOException;
import java.io.InputStream;

import org.apache.poi.hwpf.extractor.WordExtractor;

public class DocExtractor implements TextExtractor {

	@Override
	public String extractText( InputStream is ) throws IOException {
		String[] pars = new WordExtractor( is ).getParagraphText();

		StringBuilder text = new StringBuilder();

		for ( String par : pars ) {
			String trimmedPar = par.trim();

			if ( trimmedPar.length() == 0 )
				continue;

			text.append( "\t" );
			text.append( trimmedPar );
			text.append( "\n\n" );
		}

		return text.toString();
	}

	@Override
	public boolean isLossless() {
		return false;
	}

}
