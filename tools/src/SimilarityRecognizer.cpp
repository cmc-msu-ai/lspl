 /*
  * Author: Antonov Vadim (avadim@gmail.com)
  */

#include <iostream>
#include <boost/scoped_ptr.hpp>

#include "lspl/patterns/Pattern.h"
#include "AbbrAnalyzer.h"
#include "SimilarityRecognizer.h"
#include "Util.h"

namespace lspl {

SimilarityRecognizer::SimilarFinder::SimilarFinder(
		const std::vector<text::TextRef> &terms1,
		const std::vector<text::TextRef> &terms2,
		NamespaceRef patterns_namespace,
		std::vector<std::vector<std::string> > &similar_patterns) :
		_terms1(terms1),
		_terms2(terms2),
		_patterns_namespace(patterns_namespace),
		_similar_patterns(similar_patterns),
		_synonim_dictionary("Synonim", "synonim_dictionary.txt") {
}

const std::vector<text::TextRef>
		&SimilarityRecognizer::SimilarFinder::terms1() const {
	return _terms1;
}

const std::vector<text::TextRef>
		&SimilarityRecognizer::SimilarFinder::terms2() const {
	return _terms2;
}

NamespaceRef SimilarityRecognizer::SimilarFinder::patterns_namespace() const {
	return _patterns_namespace;
}

std::vector<std::vector<std::string> >
		&SimilarityRecognizer::SimilarFinder::similar_patterns() const {
	return _similar_patterns;
}

const dictionaries::SynDictionary &
		SimilarityRecognizer::SimilarFinder::synonim_dictionary() const {
	return _synonim_dictionary;
}

std::vector<std::vector<int> *> *
		SimilarityRecognizer::SimilarFinder::FindSimilars() {
	std::vector<std::vector<int> *> *result =
			new std::vector<std::vector<int> *>();
	/// terms1 && main patterns
	for(int j = 0; j < patterns_namespace()->getPatternCount(); ++j) {
		patterns::PatternRef pattern =
				patterns_namespace()->getPatternByIndex(j);

		for(int i = 0; i < terms1().size(); ++i) {
			std::string normalized_match =
					Util::GetNormalizedMatch(terms1()[i], pattern); 

			if (normalized_match == "") {
				continue;
			}

			std::map<std::string, std::string> pattern_words;
			//std::cout << Util::out.convert(terms1[i]) << std::endl;
			//std::cout << Util::out.convert(pattern->getSource()) << std::endl;
			if (!Util::BuildWordsByPattern(normalized_match, pattern,
					pattern_words)) {
				continue;
			}

			result->push_back(FindSimilars(terms1()[i], pattern_words,
					similar_patterns()[j]));
		}
	}
	return result;
}

std::vector<int> *SimilarityRecognizer::SimilarFinder::FindSimilars(
		const text::TextRef term1,
		std::map<std::string, std::string> &pattern_words,
		const std::vector<std::string> &similar_patterns) {
	for(std::map<std::string, std::string>::iterator i = pattern_words.begin();
			i != pattern_words.end(); ++i) {
		std::cout << i->first << ":\"" << Util::out.convert(i->second) <<"\"";
	}
	std::cout << std::endl;
	std::vector<int> *result = new std::vector<int>();

	std::vector<std::string> new_similar_patterns;
	for(int i = 0; i < similar_patterns.size(); ++i) {
		std::string similar_pattern =
				Util::BuildPattern(similar_patterns[i], pattern_words);
		new_similar_patterns.push_back(similar_pattern);
	}
	NamespaceRef similar_patterns_namespace =
			Util::BuildPatterns(new_similar_patterns);
	std::vector<bool> is_similar(terms2().size(), false);

	for(int l = 0; l < similar_patterns_namespace->getPatternCount(); ++l) {
		patterns::PatternRef similar_pattern =
				similar_patterns_namespace->getPatternByIndex(l);
		for(int k = 0; k < terms2().size(); ++k) {
			if (is_similar[k]) {
				continue;
			}
			std::string normalized_match =
					Util::GetNormalizedMatch(terms2()[l], similar_pattern); 

			if (normalized_match == "") {
				continue;
			}

			std::map<std::string, std::string> similar_pattern_words;
			if (!Util::BuildWordsByPattern(normalized_match, similar_pattern,
					similar_pattern_words)) {
				continue;
			}

			if (IsSimilar(pattern_words, similar_pattern_words)) {
				std::cout << "Similar by patternds: " <<
						Util::out.convert(term1->getContent()) << " && " <<
						Util::out.convert(terms2()[k]->getContent()) << std::endl;
				is_similar[k] = true;
			}
		}
	}
	for(int i = 0; i < terms2().size(); ++i) {
		if (!is_similar[i]) {
			if (synonim_dictionary().acceptsWords(term1->getContent(),
					terms2()[i]->getContent())) {
				std::cout << "Similar as synonims: " <<
						Util::out.convert(term1->getContent()) << " && " <<
						Util::out.convert(terms2()[i]->getContent()) << std::endl;
				is_similar[i] = true;
				continue;
			}
			if (AbbrAnalyzer::Analyze(term1->getContent(),
					terms2()[i]->getContent()) == 1) {
				std::cout << "Similar as abbr: " <<
						Util::out.convert(term1->getContent()) << " && " <<
						Util::out.convert(terms2()[i]->getContent()) << std::endl;
				is_similar[i] = true;
				continue;
			}
		}
	}
	for(int i = 0; i < terms2().size(); ++i) {
		if (is_similar[i]) {
			result->push_back(i);
		}
	}
	return result;
}

bool SimilarityRecognizer::SimilarFinder::IsSimilar(
		std::map<std::string, std::string> &term1,
		std::map<std::string, std::string> &term2) const {
	for(std::map<std::string, std::string>::iterator i = term1.begin();
			i != term1.end(); ++i) {
		if (term2.find(i->first) != term2.end() && term2[i->first] != i->second) {
			return false;
		}
	}
	return true;
}

NamespaceRef SimilarityRecognizer::patterns_namespace() const {
	return _patterns_namespace;
}

std::vector<std::vector<std::string> >
		&SimilarityRecognizer::similar_patterns() {
	return _similar_patterns;
}

void SimilarityRecognizer::LoadSimilarPatterns(const char *file) {
	std::string text = Util::LoadTextFromFile(file);
	std::vector<std::string> patterns;
	Util::LoadSimilarPatterns(text, patterns, _similar_patterns);
	_patterns_namespace = Util::BuildPatterns(patterns);
}

SimilarityRecognizer::SimilarityRecognizer(
		const char *similarity_patterns_file) {
	LoadSimilarPatterns(similarity_patterns_file);
}

void SimilarityRecognizer::FindSimilars(const std::vector<std::string> &terms1,
		const std::vector<std::string> &terms2) {
	std::vector<text::TextRef> terms1_text;
	std::vector<text::TextRef> terms2_text;
	Util::ConvertToText(terms1, terms1_text);
	Util::ConvertToText(terms2, terms2_text);
	FindSimilars(terms1_text, terms2_text);
}

void SimilarityRecognizer::FindSimilars(
		const std::vector<text::TextRef> &terms1,
		const std::vector<text::TextRef> &terms2) {
	boost::scoped_ptr<SimilarFinder> similar_finder(new SimilarFinder(terms1,
			terms2,	patterns_namespace(), similar_patterns()));
	similar_finder->FindSimilars();
}

} // namespace lspl
