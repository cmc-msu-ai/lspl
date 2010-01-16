/*
 * Author: Antonov Vadim (avadim@gmail.com)
 */

#include <algorithm>
#include <fstream>
#include <iostream>

#include "lspl/morphology/Morphology.h"
#include "lspl/morphology/WordForm.h"
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

	void Util::ExtractWordsAndDelimiters(const std::string &text,
			std::vector<std::string> &words) {
		words.clear();
		bool last_delimiter = true;
		int word_start = 0;
		for(int i = 0; i < text.size(); ++i) {
			if (delimiters.find_first_of(text[i]) == std::string::npos) {
				if (last_delimiter) {
					if (i > 0) {
						std::string delimiter(text, word_start, i - word_start);
						words.push_back(delimiter);
					}
					word_start = i;
				}
				last_delimiter = false;
			} else {
				if (!last_delimiter) {
					std::string word(text, word_start, i - word_start);
					words.push_back(word);
					word_start = i;
				}
				last_delimiter = true;
			}
		}
		if (text.size() > 0) {
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
		//std::cout << "Normalize " << out.convert(term) << std::endl;
		std::vector<std::string> words;
		ExtractWordsAndDelimiters(term, words);
		std::string result;
		// std::cout << "Normalized: ";
		for(int i = 0; i < words.size(); ++i) {
			//std::cout << "'" << words[i] << "' " << words[i].size() << std::endl; 
			boost::ptr_vector<morphology::WordForm> forms;
			morphology::Morphology::instance().appendWordForms(words[i], forms);
			if (forms.size()) {
				result += forms[0].getBase();
			} else {
				result += words[i];
			}
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
		patterns::restrictions::AndRestriction restriction;
		return GetNormalizedMatch(text, pattern, restriction);
	}

	std::string Util::GetNormalizedMatch(std::string text,
			patterns::PatternRef pattern) {
		text::TextRef textRef = ConvertToText(text);
		return GetNormalizedMatch(textRef, pattern);
	}

	std::string Util::GetNormalizedMatch(text::TextRef text,
			patterns::PatternRef pattern,
			patterns::restrictions::AndRestriction &restriction) {
		text::RestrictedMatchList matches =
				text->getRestrictedMatches(pattern, restriction);
		if (!matches.size()) {
			//std::cout << "No matches" << std::endl;
			return "";
		}

		int text_words_count = Util::CountWords(text->getContent());
		int match_words_count =
				Util::CountWords(matches[0]->getRangeString());
		if (matches.size() != 1  || text_words_count != match_words_count ) {
			return "";
		}
		std::string normalized_match =
				normalization.normalize(*matches[0]->getVariants().at(0));
		return normalized_match;
	}

	std::string Util::GetNormalizedMatch(std::string text,
			patterns::PatternRef pattern,
			patterns::restrictions::AndRestriction &restriction) {
		text::TextRef textRef = ConvertToText(text);
		return GetNormalizedMatch(textRef, pattern, restriction);
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
					result += "'" + word + "'";
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
					//std::cout << out.convert(pattern) << std::endl;
					if (text[i] == '#') {
						state = LOADSTATE_SIMILARS;
					}
					break;
				case LOADSTATE_SIMILARS:
					//std::cout<<"!";
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
					//std::cout << out.convert(pattern) << std::endl;
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

	NamespaceRef Util::BuildPatterns(const std::vector<std::string> &patterns,
			bool use_dictionary) {
		NamespaceRef result = new Namespace();
		if (use_dictionary) {
			result->addDictionary(
					new dictionaries::SynDictionary("Syn", "syn_dictionary.txt"));
		}
		patterns::PatternBuilderRef builder =
				new patterns::PatternBuilder(result);
		for(int i = 0; i < patterns.size(); ++i) {
			try {
				builder->build(patterns[i]);
#ifdef DEBUG
				std::cout << "Successfully build pattern \"" << out.convert(patterns[i])
						<< "\"" << std::endl;
#endif
			} catch (...) {
				std::cout << "Cannot build pattern \"" << out.convert(patterns[i])
						<< "\"" << std::endl;
			}
		}
		return result;
	}

	bool Util::IsPretext(const std::string &word) {
		return word.size() <= 3;
	}

	bool Util::IsDelimiter(char symbol) {
		return delimiters.find(symbol) != std::string::npos;
	}

	void Util::ToUpper(std::string &word) {
		std::string::iterator i = word.begin();
		std::string::iterator end = word.end();
		std::string russian_small("абвгдеёжзийклмнопрстуфхцчшщьыъэюя");
		std::string russian_big("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯ");

		while (i != end) {
			*i = toupper((unsigned char)*i);
			size_t number_small = russian_small.find((unsigned char)*i);
			size_t number_big = russian_big.find((unsigned char)*i);
			if ((number_small != std::string::npos) &&
					(number_big == std::string::npos)) {
				*i = russian_big[(int)number_small];
			}
			++i;
		}
	}

	int Util::LevinshteinDistance(const std::string &word1,
			const std::string &word2) {
		std::vector<std::vector<int> >
				D(2, std::vector<int>(word2.size() + 1, 0));
		int current = 0;
		for(int j = 0; j <= word2.size(); ++j) {
			D[0][j] = j;
		}
		for(int i = 1; i <= word1.size(); ++i) {
			int previous = current;
			current = 1 - current;
			D[current][0] = i;
			for(int j = 1; j <= word2.size(); ++j) {
				D[current][j] =
						std::min(D[current][j - 1] + 1, D[previous][j] + 1);
				int d = !(word1[i - 1] == word2[j - 1]);
				//std::cout << d << " " << word1[i - 1] << word2[j - 1] << std::endl;
				D[current][j] = std::min(D[current][j], D[previous][j - 1] + d);
				//std::cout << D[current][j] << " ";
			}
			//std::cout << std::endl;
		}
		return D[current][word2.size()];
	}

	bool Util::IsOrphologicalVariant(const std::string &term1,
			const std::string &term2) {
		std::vector<std::string> words1;
		ExtractWords(Normalize(term1), words1);
		std::vector<std::string> words2;
		ExtractWords(Normalize(term2), words2);
		if (words1.size() != words2.size()) {
			return false;
		}
		bool result = true;
		//std::cout << term1 << "!" << term2 << std::endl;
		int unsimiliarWords = 0;
		for(int i = 0; i < words1.size(); ++i) {
			int minlen = std::min(words1[i].size(), words2[i].size());
			result &= (words1[i] == words2[i]) ||
					(std::abs((int)words1[i].size() - (int)words2[i].size())
							<= 2) &&
					(LevinshteinDistance(words1[i], words2[i]) <=
							std::min(2, minlen - 2));
			unsimiliarWords += !!(words1[i] != words2[i]);
			//std::cout << result <<std::endl;
		}
		if (unsimiliarWords > words1.size() / 2 + words1.size() % 2) {
			result = false;
		}
		return result;
	}
} // namespace lspl.
