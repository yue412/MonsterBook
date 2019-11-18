#ifndef CMONSTER_H
#define CMONSTER_H

#include <string>
#include <set>
#include "MonsterTypes.h"

class CSkill;
class CSoulBead;

class CMonster
{
	friend class CConfig;
public:
    CMonster();
    ~CMonster();
public:
    void init(CSoulBead* pSoulBead);
	CSoulBead* getSoulBead() { return m_pSoulBead; }
    std::wstring getName() { return m_sName; }
	EnElementClass getClass() { return m_nClass; }
    double getFeature(EnFeatures nFeature) { return m_nFeatures[nFeature]; }
	double* getFeatures() { return m_nFeatures; }
    double getFeatureSum(char nFeatureSet);
	CSkill* getSkill(int nIndex) { return m_oSkills[nIndex]; }
	int getSkillCount() { return (int)m_oSkills.size(); }
    bool hasSpeciality(char nSpecialitySet);
    char getSpeciality() { return m_nSpeciality; }
private:
    std::wstring m_sName;
	EnElementClass m_nClass;
	std::vector<CSkill*> m_oSkills;
	double m_nFeatures[EF_ALL];
    char m_nSpeciality;
	CSoulBead* m_pSoulBead;
};

#endif // !CMONSTER_H