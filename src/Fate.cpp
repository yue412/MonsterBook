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

void CFate::init()
{
    for each (auto pMonster in m_oMonsters)
    {
        this->m_nMonsterSet |= pMonster->getId();
    }
}
