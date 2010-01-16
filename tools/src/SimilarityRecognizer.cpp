 /*
  * Author: Antonov Vadim (avadim@gmail.com)
  */

#include <iostream>
#include <boost/scoped_ptr.hpp>

#include "lspl/patterns/Pattern.h"
#include "lspl/patterns/expressions/AttributeExpression.h"
#include "lspl/patterns/expressions/ConstantExpression.h"
#include "lspl/patterns/expressions/VariableExpression.h"
#include "lspl/patterns/restrictions/AgreementRestriction.h"
#include "lspl/patterns/restrictions/AndRestriction.h"
#include "AbbrAnalyzer.h"
#include "SimilarityRecognizer.h"
#include "Util.h"

namespace lspl {

SimilarityRecognizer::SimilarFinder::SimilarFinder(
		const std::vector<text::TextRef> &terms1,
		const std::vector<text::TextRef> &terms2,
		NamespaceRef patterns_namespace,
		std::vector<NamespaceRef> &similar_patterns) :
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

std::vector<NamespaceRef>
		&SimilarityRecognizer::SimilarFinder::similar_patterns() const {
	return _similar_patterns;
}

const dictionaries::SynDictionary &
		SimilarityRecognizer::SimilarFinder::synonim_dictionary() const {
	return _synonim_dictionary;
}

std::vector<std::set<int> *> *
		SimilarityRecognizer::SimilarFinder::FindSimilars() {
	std::vector<std::set<int> *> *result =
			new std::vector<std::set<int> *>();
	for(int i = 0; i < terms1().size(); ++i) {
		result->push_back(new std::set<int>());
	}

	/// terms1 && main patterns
	for(int j = 0; j < patterns_namespace()->getPatternCount(); ++j) {
		patterns::PatternRef pattern =
				patterns_namespace()->getPatternByIndex(j);
		std::cout << "FindSimilars " <<
				Util::out.convert(pattern->getSource()) << std::endl;

		for(int i = 0; i < terms1().size(); ++i) {
			std::string normalized_match =
					Util::GetNormalizedMatch(terms1()[i], pattern); 

			std::cout << "FindSimilars " <<
					Util::out.convert(terms1()[i]->getContent()) << std::endl;

			if (normalized_match == "") {
				continue;
			}

			std::map<std::string, std::string> pattern_words;
//#ifdef DEBUG
			std::cout << "FindSimilars " <<
					Util::out.convert(normalized_match) << std::endl;
			std::cout << "FindSimilars " <<
					Util::out.convert(pattern->getSource()) << std::endl;
//#endif
			if (!Util::BuildWordsByPattern(normalized_match, pattern,
					pattern_words)) {
				continue;
			}

			std::vector<int> *term_result =	FindSimilars(terms1()[i],
					pattern_words, similar_patterns()[j]);
			for(int k = 0; k < term_result->size(); ++k) {
				(*result)[i]->insert((*term_result)[k]);
			}
		}
	}
	return result;
}

std::vector<int> *SimilarityRecognizer::SimilarFinder::FindSimilars(
		const text::TextRef term1,
		std::map<std::string, std::string> &pattern_words,
		NamespaceRef similar_patterns_namespace) {
//#ifdef DEBUG
	std::cout << "\tFind Similars (second) ";
//#endif
	patterns::restrictions::AndRestriction andRestriction;
	for(std::map<std::string, std::string>::iterator i = pattern_words.begin();
			i != pattern_words.end(); ++i) {
		patterns::restrictions::AgreementRestriction *restriction =
				new patterns::restrictions::AgreementRestriction();
		restriction->addArgument(
				new patterns::expressions::AttributeExpression(
						new patterns::expressions::VariableExpression(i->first),
				text::attributes::AttributeKey::BASE));
		restriction->addArgument(
				new patterns::expressions::ConstantExpression(i->second));
		andRestriction.addArgument(restriction);
//#ifdef DEBUG
		std::cout << i->first << ":\"" << Util::out.convert(i->second) <<"\"";
//#endif
	}
//#ifdef DEBUG
	std::cout << std::endl;
//#endif
	std::vector<int> *result = new std::vector<int>();

	std::vector<bool> is_similar(terms2().size(), false);

	for(int l = 0; l < similar_patterns_namespace->getPatternCount(); ++l) {
		patterns::PatternRef similar_pattern =
				similar_patterns_namespace->getPatternByIndex(l);
//#ifdef DEBUG
		std::cout << "\tFind Similars (second) " <<
				Util::out.convert(similar_pattern->getSource()) << " " <<
				std::endl;
//#endif
		for(int k = 0; k < terms2().size(); ++k) {
			if (is_similar[k]) {
				continue;
			}
			std::string normalized_match = Util::GetNormalizedMatch(terms2()[k],
					similar_pattern, andRestriction); 

			if (normalized_match == "") {
				continue;
			}

			std::cout << "\tFind Similars (second) " <<
					Util::out.convert(terms2()[k]->getContent()) << " " <<
					Util::out.convert(normalized_match) << std::endl;

			std::map<std::string, std::string> similar_pattern_words;
			if (!Util::BuildWordsByPattern(normalized_match, similar_pattern,
					similar_pattern_words)) {
				continue;
			}

			if (IsSimilar(pattern_words, similar_pattern_words)) {
				std::cout << "Similar by patterns: " <<
						Util::out.convert(term1->getContent()) << " && " <<
						Util::out.convert(terms2()[k]->getContent()) <<
						std::endl;
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
			if (Util::IsOrphologicalVariant(term1->getContent(),
					terms2()[i]->getContent())) {
				std::cout << "Similar as orphographical variants: " <<
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

std::vector<NamespaceRef>
		&SimilarityRecognizer::similar_patterns() {
	return _similar_patterns;
}

void SimilarityRecognizer::LoadSimilarPatterns(const char *file) {
	std::string text = Util::LoadTextFromFile(file);
	std::vector<std::string> patterns;
	std::vector<std::vector<std::string> > similar_patterns;
	Util::LoadSimilarPatterns(text, patterns, similar_patterns);
	_patterns_namespace = Util::BuildPatterns(patterns);
	for(int i = 0; i < similar_patterns.size(); ++i) {
		NamespaceRef similar_namespace =
				Util::BuildPatterns(similar_patterns[i]);
		_similar_patterns.push_back(similar_namespace);
	}
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
