#ifndef CMONSTER_H
#define CMONSTER_H

#include <string>
#include "MonsterTypes.h"

class CSkill;

class CMonster
{
	friend class CConfig;
public:
    CMonster();
    ~CMonster();
public:
    std::wstring getName() { return m_sName; }
	EnElementClass getClass() { return m_nClass; }
	int getFeature(EnFeatures nFeature) { return m_nFeatures[nFeature]; }
	int* getFeatures() { return m_nFeatures; }
	CSkill* getSkill(int nIndex) { return m_oSkills[nIndex]; }
	int getSkillCount() { return (int)m_oSkills.size(); }
private:
    std::wstring m_sName;
	EnElementClass m_nClass;
	std::vector<CSkill*> m_oSkills;
	int m_nFeatures[c_nFeatureCount];
};

#endif // !CMONSTER_H