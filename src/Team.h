#ifndef TEAM_H
#define TEAM_H

#include "MonsterTypes.h"
#include "BigInt.h"

class CMonster;

class CTeam
{
public:
    CTeam();
    ~CTeam();
    std::vector<CMonster*>::iterator begin();
    std::vector<CMonster*>::const_iterator begin() const;
    std::vector<CMonster*>::iterator end();
    std::vector<CMonster*>::const_iterator end() const;
    void push_back(CMonster* pMonster);
    void pop_back();
    int size() const;
    int getCount(EnElementClass nClass) const;
    CMonster* getItems(int nIndex) const;
    CBigInt getMonsterSet();
private:
    std::vector<CMonster*> m_oMonsterList;
    int m_nCount[EC_ALL];
};

typedef std::shared_ptr<CTeam> CTeamPtr;


#endif // !SKILL_H