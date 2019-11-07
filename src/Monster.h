#ifndef CMONSTER_H
#define CMONSTER_H

#include <string>
#include "MonsterTypes.h"

class CSkill;

class CMonster
{
public:
    CMonster();
    ~CMonster();
public:
    std::wstring getName() { return m_sName; }
    EnElementClass getClass();
    int getFeature(EnFeatures nFeature);
    int* getFeatures();
    CSkill* getSkill(int nIndex);
    int getSkillCount();
private:
    std::wstring m_sName;
};

#endif // !CMONSTER_H