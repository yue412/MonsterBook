#include "Fate.h"
#include "Monster.h"
#include "Skill.h"

CFate::~CFate()
{
    for each (auto pSkill in m_oSkills)
    {
        delete pSkill;
    }
    m_oSkills.clear();
}

CBigInt CFate::getMonsterSet()
{
    CBigInt nResult;
    for each (auto pMonster in m_oMonsters)
    {
        nResult |= pMonster->getId();
    }
    return nResult;
}
