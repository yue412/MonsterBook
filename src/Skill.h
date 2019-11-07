#ifndef SKILL_H
#define SKILL_H

#include "MonsterTypes.h"

class CSkill
{
public:
    CSkill();
    virtual ~CSkill();
    virtual void affect(const CTeam& oTeam) = 0;
};

#endif // !SKILL_H