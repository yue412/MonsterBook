#ifndef GAME_H
#define GAME_H

#include "MonsterTypes.h"

class CChallenge;
class CMonster;

typedef std::vector<std::pair<CChallenge*, CTeam*>> CSolution;

class CGame
{
    friend class CConfig;
public:
    CGame();
    ~CGame();
public:
    void play(std::vector<CSolution*>& oSolutionList);
private:
    void clear();
    std::vector<CChallenge*> m_oChallengeList;
    std::vector<CMonster*> m_oMonsterList;
};

#endif // !GAME_H
