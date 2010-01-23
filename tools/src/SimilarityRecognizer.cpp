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
#include "lspl/transforms/ContextRetriever.h"
#include "AbbrAnalyzer.h"
#include "SimilarityRecognizer.h"
#include "Util.h"

namespace lspl {

SimilarityRecognizer::SimilarFinder::SimilarFinder(
		const TTerms &terms1, const TTerms &terms2,
		NamespaceRef patterns_namespace,
		TSimilarPatterns &similar_patterns,
		const TStConditions &st_conditions) : 
		_terms1(terms1),
		_terms2(terms2),
		_patterns_namespace(patterns_namespace),
		_similar_patterns(similar_patterns),
		_synonim_dictionary("Synonim", "synonim_dictionary.txt"),
		_st_conditions(st_conditions){
}

const TTerms &SimilarityRecognizer::SimilarFinder::terms1() const {
	return _terms1;
}

const TTerms &SimilarityRecognizer::SimilarFinder::terms2() const {
	return _terms2;
}

NamespaceRef SimilarityRecognizer::SimilarFinder::patterns_namespace() const {
	return _patterns_namespace;
}

TSimilarPatterns
		&SimilarityRecognizer::SimilarFinder::similar_patterns() const {
	return _similar_patterns;
}

const dictionaries::SynDictionary &
		SimilarityRecognizer::SimilarFinder::synonim_dictionary() const {
	return _synonim_dictionary;
}

TResults *SimilarityRecognizer::SimilarFinder::FindSimilars() {
	TResults *result = new TResults();
	for(int i = 0; i < terms1().size(); ++i) {
		result->push_back(new TResult());
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

			TTermWords pattern_words;
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

			text::MatchList matches = terms1()[i]->getMatches(pattern);
			assert(matches.size() == 1);
			static transforms::ContextRetriever contextRetriver;
			patterns::matchers::Context context =
					contextRetriver.apply(*(matches[0]->getVariant(0)));
			std::vector<int> *term_result =	FindSimilars(terms1()[i],
					pattern_words, similar_patterns()[j], _st_conditions[j],
					context);
			for(int k = 0; k < term_result->size(); ++k) {
				(*result)[i]->insert((*term_result)[k]);
			}
		}
	}
	for(int i = 0; i < terms1().size(); ++i) {
		for(int j = 0; j < terms2().size(); ++j) {
			if ((*result)[i]->find(j) != (*result)[i]->end()) {
				continue;
			}
			if (IsSimilar(terms1()[i]->getContent(),
					terms2()[j]->getContent())) {
				(*result)[i]->insert(j);
			}
		}
	}
	return result;
}

boost::shared_ptr<patterns::restrictions::AndRestriction>
		SimilarityRecognizer::SimilarFinder::GenerateAndRestriction(
				const TTermWords &pattern_words) {
	boost::shared_ptr<patterns::restrictions::AndRestriction> andRestriction(
			new patterns::restrictions::AndRestriction());
	for(TTermWords::const_iterator i = pattern_words.begin();
			i != pattern_words.end(); ++i) {
		patterns::restrictions::AgreementRestriction *restriction =
				new patterns::restrictions::AgreementRestriction();
		restriction->addArgument(
				new patterns::expressions::AttributeExpression(
						new patterns::expressions::VariableExpression(
								patterns::matchers::Variable(i->first)),
				text::attributes::AttributeKey::BASE));
		restriction->addArgument(
				new patterns::expressions::ConstantExpression(i->second));
		andRestriction->addArgument(restriction);
	}
	return andRestriction;
}

std::vector<int> *SimilarityRecognizer::SimilarFinder::FindSimilars(
		const TTerm &term1,
		TTermWords &pattern_words,
		NamespaceRef similar_patterns_namespace,
		const TStConditionsForSimilarPatterns &st_conditions,
		patterns::matchers::Context &context) {
//#ifdef DEBUG
	std::cout << "\tFind Similars (second) ";
	for(TTermWords::iterator i = pattern_words.begin();
			i != pattern_words.end(); ++i) {
		std::cout << i->first << ":\"" << Util::out.convert(i->second) <<"\"";
	}
	std::cout << std::endl;
//#endif
	std::vector<int> *result = new std::vector<int>();

	std::vector<bool> is_similar(terms2().size(), false);

	for(int l = 0; l < similar_patterns_namespace->getPatternCount(); ++l) {
		boost::shared_ptr<patterns::restrictions::AndRestriction>
				andRestriction = GenerateAndRestriction(pattern_words);
		if (st_conditions[l] !=  NULL) {
			text::TransitionConstRef transition =
					context.getValues(st_conditions[l]->second).first->second;
			patterns::restrictions::AgreementRestriction *restriction =
					new patterns::restrictions::AgreementRestriction();
			restriction->addArgument(
					new patterns::expressions::AttributeExpression(
							new patterns::expressions::VariableExpression(
									st_conditions[l]->first),
					text::attributes::AttributeKey::STEM));
			restriction->addArgument(
					new patterns::expressions::AttributeExpression(
							new patterns::expressions::ConstantExpression(
									*transition),
					text::attributes::AttributeKey::STEM));
			andRestriction->addArgument(restriction);
			restriction->dump(std::cerr);
			std::cerr << std::endl;
		}
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
					similar_pattern, *(andRestriction.get())); 

			if (normalized_match == "") {
				continue;
			}

			std::cout << "\tFind Similars (second) " <<
					Util::out.convert(terms2()[k]->getContent()) << " " <<
					Util::out.convert(normalized_match) << std::endl;

			TTermWords similar_pattern_words;
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
		if (is_similar[i]) {
			result->push_back(i);
		}
	}
	return result;
}

bool SimilarityRecognizer::SimilarFinder::IsSimilar(const std::string &term1,
		const std::string &term2) const {
	if (Util::Normalize(term1) == Util::Normalize(term2)) {
		std::cout << "Similar as equal words: " <<
				Util::out.convert(term1) << " && " <<
				Util::out.convert(term2) << std::endl;
		return true;
	}
	if (synonim_dictionary().acceptsWords(term1, term2)) {
		std::cout << "Similar as synonims: " <<
				Util::out.convert(term1) << " && " <<
				Util::out.convert(term2) << std::endl;
		return true;
	}
	if (Util::IsOrphologicalVariant(term1, term2)) {
		std::cout << "Similar as orphographical variants: " <<
				Util::out.convert(term1) << " && " <<
				Util::out.convert(term2) << std::endl;
		return true;
	}
	if (AbbrAnalyzer::Analyze(term1, term2) == 1) {
		std::cout << "Similar as abbr: " <<
				Util::out.convert(term1) << " && " <<
				Util::out.convert(term2) << std::endl;
		return true;
	}
	return false;
}

bool SimilarityRecognizer::SimilarFinder::IsSimilar(TTermWords &term1,
		TTermWords &term2) const {
	for(TTermWords::iterator i = term1.begin();	i != term1.end(); ++i) {
		if (term2.find(i->first) != term2.end() &&
				term2[i->first] != i->second) {
			return false;
		}
	}
	return true;
}

NamespaceRef SimilarityRecognizer::patterns_namespace() const {
	return _patterns_namespace;
}

TSimilarPatterns &SimilarityRecognizer::similar_patterns() {
	return _similar_patterns;
}

void SimilarityRecognizer::LoadSimilarPatterns(const char *file) {
	std::string text = Util::LoadTextFromFile(file);
	std::vector<std::string> patterns;
	std::vector<std::vector<std::string> > similar_patterns;
	Util::LoadSimilarPatterns(text, patterns, similar_patterns);
	_patterns_namespace = Util::BuildPatterns(patterns);
	for(int i = 0; i < similar_patterns.size(); ++i) {
		TStConditionsForSimilarPatterns st_conditions;
		_st_conditions.push_back(st_conditions);
		for(int j = 0; j < similar_patterns[i].size(); ++j) {
			std::cout << similar_patterns[i][j] << std::endl;
			std::string condition =
					Util::ExtractStCondition(similar_patterns[i][j]);
			std::cout << condition << "!!" << similar_patterns[i][j] << std::endl;
			if (condition == "") {
				_st_conditions[i].push_back(NULL);
				continue;
			}
			std::pair<std::string, std::string> vars =
					Util::ConvertStCondition(condition);
			std::cout << vars.first << "!" << vars.second << std::endl;
			patterns::matchers::Variable var1(vars.first);
			patterns::matchers::Variable var2(vars.second);
			if (similar_patterns[i][j].find(vars.first) == std::string::npos) {
				patterns::matchers::Variable var3 = var1;
				var1 = var2;
				var2 = var3;
			}
			TStCondition *st_condition = new TStCondition(var1, var2);
			_st_conditions[i].push_back(st_condition);
		}
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
	TTerms terms1_text;
	TTerms terms2_text;
	Util::ConvertToText(terms1, terms1_text);
	Util::ConvertToText(terms2, terms2_text);
	FindSimilars(terms1_text, terms2_text);
}

void SimilarityRecognizer::FindSimilars(const TTerms &terms1,
		const TTerms &terms2) {
	boost::scoped_ptr<SimilarFinder> similar_finder(new SimilarFinder(terms1,
			terms2,	patterns_namespace(), similar_patterns(), _st_conditions));
	similar_finder->FindSimilars();
}

} // namespace lspl
