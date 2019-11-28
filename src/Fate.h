#ifndef FATE_H
#define FATE_H

#include <string>
#include <vector>
#include "BigInt.h"

class CSkill;
class CMonster;

class CFate
{
    friend class CConfig;
public:
    ~CFate();
    void init();
    CSkill* getSkill(int nIndex) { return m_oSkills[nIndex]; }
    int getSkillCount() { return (int)m_oSkills.size(); }
    CBigInt& getMonsterSet() { return m_nMonsterSet; }
    CMonster* getMonster(int nIndex) { return m_oMonsters[nIndex]; }
    int getMonsterCount() { return (int)m_oMonsters.size(); }
private:
    std::wstring m_sName;
    std::vector<CSkill*> m_oSkills;
    std::vector<CMonster*> m_oMonsters;
    CBigInt m_nMonsterSet;
};

#endif // !MONSTERID_H