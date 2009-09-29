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

	void Util::ConvertToText(const std::vector<std::string> &terms,
			std::vector<text::TextRef> &terms_text) {
		for(int i = 0; i < terms.size(); ++i) {
			terms_text.push_back(ConvertToText(terms[i]));
		}
	}

	text::TextRef Util::ConvertToText(const std::string &term) {
		static text::readers::PlainTextReader reader;
		return reader.readFromString(term);
	}

	std::string Util::Normalize(const std::string &term) {
		std::cout << out.convert(term) << std::endl;
		text::TextRef text = ConvertToText(term);
		std::string result;
		for(int i = 0; i < text->getWords().size(); ++i) {
			/*std::cout << out.convert(text->getContent()) << " " <<
				 out.convert((text->getWords()[i]->getBase())) << std::endl;*/
			if (i > 0) {
				result += " ";
			}
			result += text->getWords()[i]->getBase();
		}
		return result;
	}
} // namespace lspl.
