#include "utilit.h"
#include "cgic.h"
extern void log(string t);
extern void print_error(const string &err);

	const size_t MaxBtnNameLen = 256;

bool ReadFromHtml(char* CorporaName, char* SearchText, int& StartHitNo,  int& ResultLimit, string& MoreBtnName, int& Timeout, string& HeaderFormat)
{
	char sMoreBtnName[MaxBtnNameLen];	
	if (cgiFormStringNoNewlines("MoreButton", sMoreBtnName, 256) != cgiFormSuccess)
			strcpy(sMoreBtnName,"more");
	MoreBtnName = sMoreBtnName;

	if (cgiFormSubmitClicked("more") == cgiFormSuccess)
	{
		cgiFormInteger("LastHitNo",&StartHitNo, 0);
		cgiFormStringNoNewlines("LastQuery", SearchText, 10240);
	}
	else
	{
		cgiFormStringNoNewlines("SearchText", SearchText, 10240);
		StartHitNo = 0;
	};
	cgiFormStringNoNewlines("CorporaName", CorporaName, 120);
	cgiFormInteger("Timeout",&Timeout, 100);
	cgiFormInteger("ResultLimit",&ResultLimit, 10);
	{
		char HeaderConstants[1024];
		cgiFormStringNoNewlines("HeaderFormat", HeaderConstants, 1024);
		HeaderFormat = HeaderConstants;
		Trim(HeaderFormat);
		if (HeaderFormat.empty())
			HeaderFormat = "<br>Occurrences: %i-%i of %i<br>";
	};


	return true;
};


