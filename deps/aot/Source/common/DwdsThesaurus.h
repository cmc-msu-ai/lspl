#ifndef __DwdsThesaurus_H_
#define __DwdsThesaurus_H_

#include "../common/util_classes.h"


class CDwdsThesaurus 
{
	//  all possible supernodes of the thesaurus Adel, H...
	StringVector HyperTerms;
	// a buffer to store pathes from the leaf to the root
	vector<DWORD> Pathes;

	// an index which points to vector Pathes
	// the count of PathEndIndex  is equal to the count of leaves in the thesaurus
	// Pathes[ PathEndIndex[i-1].... PathEndIndex[i] ]  is a path number i
	vector<DWORD> PathEndIndex;

	// an index which points to vector Pathes
	vector<DWORD> LeavesPathes;
	CShortStringHolder LeavesLemmas;

	DwordVector m_AllPathes;

public:
	bool	LoadDwdsThesaurus ();
	int		FindHyperTerm (const string& s) const;
	void	GetAllPathesWhichContainHyperTermNo (size_t HyperTermNo, DwordVector& sub_pathes) const;	
	bool	ReadPathFromString(const string& TextPath, vector<int>& HyperTermNos) const;
	void	GetAllPathesWhichStartFromThisSequence (const vector<int>& HyperTermNos, DwordVector& Result) const;
	void	GetAllLemmasWhosePathesStartFromThisSequence (const vector<int>& HyperTermNos, StringSet& Lemmas) const;
	string	GetAllThesInterpetations(const char* Lemma) const;
};

extern const CDwdsThesaurus* LoadDwdsThesaurus();
extern void FreeDwdsThesaurus(const CDwdsThesaurus*);

#endif
