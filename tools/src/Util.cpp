/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <fstream>
#include <iostream>

#include "lspl/text/readers/PlainTextReader.h"
#include "Util.h"

namespace lspl {
	lspl::utils::Conversion Util::in("UTF-8", "CP1251");
	lspl::utils::Conversion Util::out("CP1251", "UTF-8");

	std::string Util::LoadTextFromFile(const char* filename) {
		std::ifstream input_stream(filename);
		input_stream.seekg(0, std::ios_base::end);
		int size = input_stream.tellg();
		input_stream.seekg(0, std::ios_base::beg);
		char* buffer = new char[size];
		input_stream.read(buffer, size);
		std::string result = in.convert(buffer, size);
		delete [] buffer;
		return result;
	}

	text::TextRef Util::LoadText(const char* text_filename) {
		std::string text = Util::LoadTextFromFile(text_filename);
		text::readers::PlainTextReader reader;
		return reader.readFromString(text);
	}

	std::string Util::Trim(const std::string &text) {
		size_t startpos = text.find_first_not_of(" \t\r\n");
		size_t endpos = text.find_last_not_of(" \t\r\n");
		if (startpos == std::string::npos || endpos == std::string::npos) {
			return "";
		} else {
			return text.substr(startpos, endpos - startpos + 1);
		}
	}

	int Util::CountWords(const std::string &text) {
		int result = 0;
		std::string delimiters = " \t\r\n,:.;\'\"!?()";
		bool is_delimiter = true;
		for(int i = 0; i < text.size(); ++i) {
			if (delimiters.find_first_of(text[i]) == std::string::npos) {
				if (is_delimiter) {
					++result;
				}
				is_delimiter = false;
			} else {
				is_delimiter = true;
			}
		}
		return result;
	}
} // namespace lspl.
