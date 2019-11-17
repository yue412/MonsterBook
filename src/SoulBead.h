#ifndef SOULBEAD_H
#define SOULBEAD_H

#include "MonsterTypes.h"

class CSoulBead
{
	friend class CConfig;
public:
	CSoulBead(): m_nClass(EC_ALL) {}
	EnElementClass getClass() { return m_nClass; }
	double getFeature(EnFeatures nFeature) { return m_nFeatures[nFeature]; }
	double* getFeatures() { return m_nFeatures; }
private:
	EnElementClass m_nClass;
	double m_nFeatures[EF_ALL];
};

#endif // !SOULBEAD_H

