package ru.lspl.gui.model.extractors;

import java.io.IOException;
import java.io.InputStream;

import org.apache.poi.hssf.extractor.ExcelExtractor;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

public class XlsExtractor implements TextExtractor {

	@Override
	public String extractText( InputStream is ) throws IOException {
		ExcelExtractor extractor = new ExcelExtractor(new POIFSFileSystem(is));

		extractor.setFormulasNotResults(true);
		extractor.setIncludeSheetNames(false);
		
		return extractor.getText();
	}

}
