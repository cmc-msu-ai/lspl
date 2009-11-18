/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <fstream>
#include <iostream>

#include "lspl/patterns/PatternBuilder.h"
#include "SynDictionary.h"
#include "Util.h"

namespace lspl {
	lspl::utils::Conversion Util::in("UTF-8", "CP1251");
	lspl::utils::Conversion Util::out("CP1251", "UTF-8");
	const std::string Util::delimiters = " \t\r\n,:.;\'\"!?()";
	const std::string Util::spaces = " \t\r\n";
	transforms::Normalization Util::normalization;
	text::readers::PlainTextReader Util::reader;

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
		return ConvertToText(text);
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
		bool last_delimiter = true;
		for(int i = 0; i < text.size(); ++i) {
			if (delimiters.find_first_of(text[i]) == std::string::npos) {
				if (last_delimiter) {
					++result;
				}
				last_delimiter = false;
			} else {
				last_delimiter = true;
			}
		}
		if (!last_delimiter) {
			++result;
		}
		return result;
	}

	void Util::ExtractWords(const std::string &text,
			std::vector<std::string> &words) {
		words.clear();
		bool last_delimiter = true;
		int word_start = 0;
		for(int i = 0; i < text.size(); ++i) {
			if (delimiters.find_first_of(text[i]) == std::string::npos) {
				if (last_delimiter) {
					word_start = i;
				}
				last_delimiter = false;
			} else {
				if (!last_delimiter) {
					std::string word(text, word_start, i - word_start);
					words.push_back(word);
				}
				last_delimiter = true;
			}
		}
		if (!last_delimiter) {
			std::string word(text, word_start, text.size() - word_start);
			words.push_back(word);
		}
	}

	void Util::ConvertToText(const std::vector<std::string> &terms,
			std::vector<text::TextRef> &terms_text) {
		for(int i = 0; i < terms.size(); ++i) {
			terms_text.push_back(ConvertToText(terms[i]));
		}
	}

	text::TextRef Util::ConvertToText(const std::string &term) {
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

	std::string Util::NormalizePattern(const std::string &pattern) {
		std::string result = "";
		int i = 0;
		bool last_delimiter = true;
		bool condition = false;
		while (i < pattern.size()) {
			if (delimiters.find_first_of(pattern[i]) != std::string::npos) {
				last_delimiter = true;
			} else if (pattern[i] == '<' && !condition) {
				condition = true;
			} else if (pattern[i] == '>' && condition) {
				condition = false;
				last_delimiter = true;
			} else if (!condition) {
				if (last_delimiter && result != "") {
					result += " ";
				}
				result += pattern[i];
				last_delimiter = false;
			}
			++i;
		}
		return result;
	}

	bool Util::BuildWordsByPattern(const std::string &text,
			const patterns::PatternRef &pattern,
			std::map<std::string, std::string> &words) {
		words.clear();
		std::string normalized_pattern = NormalizePattern(pattern->getSource());
		//std::cout << Util::out.convert(normalized_pattern) << std::endl;
		std::vector<std::string> text_words;
		ExtractWords(text, text_words);
		std::vector<std::string> pattern_words;
		ExtractWords(normalized_pattern, pattern_words);
		if (text_words.size() != pattern_words.size()) {
			return false;
		}
		for(int i = 0; i < text_words.size(); ++i) {
			//std::cout << pattern_words[i] << " " << out.convert(text_words[i]) << std::endl;
			words[pattern_words[i]] = text_words[i];
		}
		//std::cout << std::endl;
		return true;
	}

	std::string Util::GetNormalizedMatch(text::TextRef text,
			patterns::PatternRef pattern) {
		text::MatchList matches = text->getMatches(pattern);
		if (!matches.size()) {
			return "";
		}

		int text_words_count = Util::CountWords(text->getContent());
		int match_words_count =
				Util::CountWords(matches[0]->getFragment(0).getText()); 
		if (matches.size() != 1  || text_words_count != match_words_count ) {
			return "";
		}
		std::string normalized_match =
				normalization.normalize(matches[0]->getVariants().at(0)); 
		return normalized_match;
	}

	std::string Util::GetNormalizedMatch(std::string text,
			patterns::PatternRef pattern) {
		text::TextRef textRef = ConvertToText(text);
		return GetNormalizedMatch(textRef, pattern);
	}

	std::string Util::BuildPattern(const std::string &pattern,
			const std::map<std::string, std::string> &words) {
		std::string result = pattern;
		for(std::map<std::string, std::string>::const_iterator i = words.begin();
				i != words.end(); ++i) {
			std::string temp = result;
			result = "";
			std::string pattern_word = i->first;
			std::string word = i->second;
			bool is_condition = false;
			bool is_bracket = false;
			int j;
			for(j = 0; j < temp.size() - pattern_word.size() + 1; ++j) {
				if (temp[j] == '<') {
					is_condition = true;
					result += '<';
					continue;
				} else if (temp[j] == '>') {
					is_condition = false;
					result += '>';
					continue;
				} else if (temp[j] == '(') {
					is_bracket = true;
					result += '(';
					continue;
				} else if (temp[j] == ')') {
					is_bracket = false;
					result += ')';
					continue;
				}
				std::string part(temp, j, pattern_word.size());
				if (part != pattern_word) {
					result += temp[j];
					continue;
				}
				j += pattern_word.size() - 1;
				if (is_condition && is_bracket) {
					result += word;
				} else if (!is_condition && !is_bracket) { 
					result += pattern_word;
					if (j + 1 < temp.size() && temp[j + 1] == '<') {
						result += "<" + word + ",";
						++j;
					} else {
						result += "<" + word + ">";
					}
				} else {
					result += pattern_word;
				}
			}
			for(; j < temp.size(); ++j) {
				result += temp[j];
			}
		}
		return result;
	}

	void Util::LoadSimilarPatterns(
			const std::string &text, std::vector<std::string> &patterns,
			std::vector<std::vector<std::string> > &similar_patterns) {
		LoadSimilarPatternsState state = LOADSTATE_NEW;
		std::vector<std::string> temp;
		for(int i = 0; i < text.size(); ++i) {
			bool is_space = spaces.find_first_of(text[i]) != std::string::npos;
			if (is_space) {
				continue;
			}
			std::string pattern;
			switch(state) {
				case LOADSTATE_NEW:
					while (i < text.size() && text[i] != '#' && text[i] != '\n' &&
							text[i] != '\r') {
						pattern += text[i];
						++i;
					}
					patterns.push_back(Trim(pattern));
					similar_patterns.push_back(temp);
					if (i == text.size()) {
						break;
					}
					if (text[i] == '#') {
						std::cout << pattern << std::endl;
						state = LOADSTATE_SIMILARS;
					}
					break;
				case LOADSTATE_SIMILARS:
					std::cout<<"!";
					bool is_condition = false;
					bool is_string = false;
					while (i < text.size() && text[i] != '\n' &&
							text[i] != '\r' &&
							(text[i] != ',' || is_condition || is_string)) {
						pattern += text[i];
						switch (text[i]) {
							case '\"':
								is_string = !is_string;
								break;
							case '<':
								is_condition = true;
								break;
							case '>':
								is_condition = false;
								break;
							default:
								break;
						}
						++i;
					}
						std::cout << pattern << std::endl;
					similar_patterns[similar_patterns.size() - 1].push_back(
							Trim(pattern));
					if (i == text.size()) {
						break;
					}
					if (text[i] != ',') {
						state = LOADSTATE_NEW;
					}
					break;
			}
		}
		assert(patterns.size() == similar_patterns.size());
	}

	NamespaceRef Util::BuildPatterns(const std::vector<std::string> &patterns) {
		NamespaceRef result = new Namespace();
		result->addDictionary(
				new dictionaries::SynDictionary("Syn", "syn_dictionary.txt"));
		patterns::PatternBuilderRef builder =
				new patterns::PatternBuilder(result);
		for(int i = 0; i < patterns.size(); ++i) {
			try {
				builder->build(patterns[i]);
				std::cout << "Successfully build pattern \"" << out.convert(patterns[i])
						<< "\"" << std::endl;
			} catch (...) {
				std::cout << "Cannot build pattern \"" << out.convert(patterns[i])
						<< "\"" << std::endl;
			}
		}
		return result;
	}
} // namespace lspl.
