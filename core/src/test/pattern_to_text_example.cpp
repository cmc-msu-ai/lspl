#include <iostream>
#include <lspl/Namespace.h>
#include <lspl/patterns/PatternBuilder.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/text/readers/PlainTextReader.h>
#include <lspl/text/Text.h>
#include <fstream>
#include <lspl/text/Match.h>
#include <cstdlib>
#include <lspl/transforms/TextTransformBuilder.h>

using namespace std;

int main(int argc, char** argv) {
	lspl::NamespaceRef ns = new lspl::Namespace();
	lspl::patterns::PatternBuilderRef builder = new lspl::patterns::PatternBuilder(ns, new lspl::transforms::TextTransformBuilder(ns));
	builder->build("ND = A1 N1 <N1=A2> =text> A1 N1 ';' #N1 '- существительное ;' #A1 '- прилагательное'");
	lspl::patterns::PatternRef pattern = ns->getPatternByName("ND");

	lspl::text::readers::PlainTextReader reader;
	lspl::text::TextRef text = reader.readFromStream( cin );

	lspl::text::MatchList matches = text->getMatches( pattern );

	for(int i=0; i<matches.size(); i++)
		cout<<matches[i]->getVariants().at(0)->getTransformResult<std::string>()<<endl;
				
	return 0;
}
