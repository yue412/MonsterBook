#ifndef GAME_H
#define GAME_H

#include "MonsterTypes.h"
#include "Result.h"

class CChallenge;
class CMonster;

typedef std::vector<std::pair<CChallenge*, CTeamPtr>> CSolution;
typedef std::shared_ptr<CSolution> CSolutionPtr;

class CGame
{
    friend class CConfig;
public:
    CGame();
    ~CGame();
public:
    void play(std::vector<CSolutionPtr>& oSolutionList);
    void play(CChallenge* pChallenge, CResult& oResult);
    void simulator(std::vector<std::wstring>& oMonsterList, int* nResult);
    int m_nCount;
private:
    // ÃÙ’Ω“ªπÿ
    void calc(CTeam& oTeam, double* oResult);
    bool success(double* dChallengeRequired, double* dTeam);
    void play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, CResult& oResult);
    void play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, std::size_t nStartIndex, std::size_t nCount, CTeam& oTeam, CResult& oResult);
    void play(int nChallengeIndex, const std::vector<CMonster*>& oMonsterList, CSolution& oSolution, std::vector<CSolutionPtr>& oSolutionList);
    void removeMonster(std::vector<CMonster*>& oMonsterList, CTeamPtr pTeam);
    void clear();
    std::vector<CChallenge*> m_oChallengeList;
    std::vector<CMonster*> m_oMonsterList;
};

#endif // !GAME_H
