
#include "DwdsThesaurus.h"
#include "../common/util_classes.h"
#include "../common/bserialize.h"

int ReferenceCount = 0;
CDwdsThesaurus* pDwdsThesaurus = 0;
	
const CDwdsThesaurus* LoadDwdsThesaurus()
{
	if (!pDwdsThesaurus)
	{
		assert (ReferenceCount == 0);
		pDwdsThesaurus = new CDwdsThesaurus;
		if (!pDwdsThesaurus) return 0;
		if (!pDwdsThesaurus->LoadDwdsThesaurus())
		{
			delete pDwdsThesaurus;
			pDwdsThesaurus = 0;
			return 0;
		};
	};
	ReferenceCount++;
	return pDwdsThesaurus;
};

void FreeDwdsThesaurus(const CDwdsThesaurus* pThes)
{
	if (pThes == 0) return;
	assert (pThes == pDwdsThesaurus);
	if (ReferenceCount > 1)
	{
		ReferenceCount--;
		return;
	};
	assert (ReferenceCount == 1);
	assert (pDwdsThesaurus);
	delete pDwdsThesaurus;
	pDwdsThesaurus = 0;
	ReferenceCount = 0;
};

//inline bool ShortStringLessHere(const CShortString& C, const char* obj) 
//{
//	return strcmp(C.GetString(), obj) < 0;
//}


string CDwdsThesaurus::GetAllThesInterpetations(const char* Lemma) const 
{
	vector<CShortString>::const_iterator it = lower_bound (LeavesLemmas.begin(), LeavesLemmas.end(), Lemma, IsLessShortString());
	
	string Result;	
	for( ; !(it == LeavesLemmas.end()) && (!strcmp(it->GetString(), Lemma)); it++)
	{
		size_t  No = it -  LeavesLemmas.begin();
		DWORD  Path = LeavesPathes[No];
		int start = 0;
		if ( Path > 0 )
			start = PathEndIndex[Path-1];
		for (size_t i=start; i < PathEndIndex[Path]; i++)
		{
			Result += ":";
			Result += HyperTerms[Pathes[i]];
		};

	};

	if (!Result.empty())
		Result += ":";

	return Result;
};

int CDwdsThesaurus::FindHyperTerm (const string& s) const
{
	StringVector::const_iterator it = lower_bound (HyperTerms.begin(), HyperTerms.end(), s);
	if (it == HyperTerms.end()) return -1;
	if (*it != s) return -1;
	return it - HyperTerms.begin();
};

string GetThesaurusPath ()
{
	if (!CanGetRegistryString("Software\\Dialing\\Concordance\\DwdsThesaurus")) 
	{
		fprintf (stderr,"Cannot load dwds thesaurus.\n");
		return "";
	};
	return  GetRegistryString("Software\\Dialing\\Concordance\\DwdsThesaurus");
};

bool  CDwdsThesaurus::LoadDwdsThesaurus ()
{
	//fprintf (stderr,"0\n");
	///////////////////////////////////////////
	string fname =  GetThesaurusPath() + string("dwds_thes_hyper.dat");
	FILE* fp = fopen (fname.c_str(), "r");
	if (!fp) 
	{
		fprintf (stderr,"Cannot open file %s\n", fname.c_str());
		return false;

	};
	char buffer[10000];
	while (fgets(buffer, 10000, fp))
	{
		if (strlen(buffer) == 0) 
		{
			fclose(fp);
			return false;
		}
		rtrim(buffer);
		if (strlen(buffer) == 0) 
		{
			fclose(fp);
			return false;
		};
		HyperTerms.push_back(buffer);
	};
	fclose (fp);

	///////////////////////////////////////////
	fname =  GetThesaurusPath() + string("dwds_thes_pathes.bin");
	if (access(fname.c_str(), 04) != 0)
	{
		fprintf (stderr,"Cannot open file %s\n", fname.c_str());
		return false;

	};
	//fprintf (stderr,"1\n");
	ReadVector(fname, Pathes);
	//fprintf (stderr,"2\n");

	///////////////////////////////////////////
	fname =  GetThesaurusPath() + string("dwds_thes_pathes.idx");
	if (access(fname.c_str(), 04) != 0)
	{
		fprintf (stderr,"Cannot open file %s\n", fname.c_str());
		return false;

	};

	ReadVector(fname, PathEndIndex);

	///////////////////////////////////////////
	fname =  GetThesaurusPath() + string("dwds_thes_lemmas_pathes.dat");
	if (access(fname.c_str(), 04) != 0)
	{
		fprintf (stderr,"Cannot open file %s\n", fname.c_str());
		return false;

	};
	ReadVector(fname, LeavesPathes);

	///////////////////////////////////////////
	fname =  GetThesaurusPath() + string("dwds_thes_lemmas_str.dat");
	if (access(fname.c_str(), 04) != 0)
	{
		fprintf (stderr,"Cannot open file %s\n", fname.c_str());
		return false;

	};
	LeavesLemmas.ReadShortStringHolder(fname);
	if (LeavesLemmas.size() != LeavesPathes.size() ) 
	{
		fprintf (stderr,"Bad index in thesaurus %s\n", fname.c_str());
		return false;
	};

	return true;
};





void CDwdsThesaurus::GetAllPathesWhichContainHyperTermNo (size_t HyperTermNo, DwordVector& sub_pathes) const
{
	sub_pathes.clear();
	for (size_t i=0; i < PathEndIndex.size(); i++)
	{
		vector<DWORD>::const_iterator start = Pathes.begin();

		if ( i > 0)
			start += PathEndIndex[i-1];

		vector<DWORD>::const_iterator end  = Pathes.begin()+PathEndIndex[i];

		if ( find(start, end, HyperTermNo) != end) 
			sub_pathes.push_back(i);
	};

};



bool  CDwdsThesaurus::ReadPathFromString(const string& TextPath, vector<int>& HyperTermNos) const
{	
	HyperTermNos.clear();
	StringTokenizer tok(TextPath.c_str(), ":");
	vector <DWORD> Path;
	while (tok()) 
	{
		string Value =  tok.val();
		Trim(Value);
		int i = FindHyperTerm(Value);
		if (i == -1)
			return false;
		HyperTermNos.push_back(i);
	};
	return true; 
};

void CDwdsThesaurus::GetAllPathesWhichStartFromThisSequence (const vector<int>& HyperTermNos, DwordVector& Result) const
{
	Result.clear();
	for (size_t i=0; i < PathEndIndex.size(); i++)
	{
		vector<DWORD>::const_iterator start = Pathes.begin();

		if ( i > 0)
			start += PathEndIndex[i-1];

		vector<DWORD>::const_iterator end  = Pathes.begin()+PathEndIndex[i];

		if (end - start < HyperTermNos.size())
			continue;

		size_t j=0;
		for (; j < HyperTermNos.size(); j++)
			if (HyperTermNos[j] != start[j])
				break;

		if (j == HyperTermNos.size())
			Result.push_back(i);
	};
};

void CDwdsThesaurus::GetAllLemmasWhosePathesStartFromThisSequence (const vector<int>& HyperTermNos, StringSet& Lemmas) const 
{
	DwordVector Result;
	GetAllPathesWhichStartFromThisSequence(HyperTermNos, Result);
	sort(Result.begin(),Result.end());
	Lemmas.clear();

	for (size_t i=0; i<LeavesPathes.size(); i++)
		if (binary_search(Result.begin(),Result.end(), LeavesPathes[i]))
		{
			const char* s = LeavesLemmas[i].GetString();
			Lemmas.insert(s);
		};

};

