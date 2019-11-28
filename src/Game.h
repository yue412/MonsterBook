#ifndef GAME_H
#define GAME_H

#include "MonsterTypes.h"
#include "Result.h"
#include "Team.h"

class CChallenge;
class CMonster;
class CSoulBead;
class CFate;

typedef std::vector<std::pair<CChallenge*, CTeamPtr>> CSolution;
typedef std::shared_ptr<CSolution> CSolutionPtr;

class CGame
{
    friend class CConfig;
    friend int main(int argc, char* argv[]);
public:
    CGame();
    ~CGame();
public:
    void play(std::vector<CSolutionPtr>& oSolutionList, std::vector<int>& oChallengeGroupIndexList);
    void play(CChallenge* pChallenge, CResult& oResult);
    void simulator(std::vector<std::wstring>& oMonsterList, int* nResult);
	void exclude(std::vector<std::wstring>& oMonsterList);
    void limitMonsterClass(EnElementClass nClass) { m_nClass = nClass; }
    int m_nCount;
private:
    // ÃÙ’Ω“ªπÿ
    void play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, CResult& oResult);
    void play(int nChallengeIndex, const std::vector<CMonster*>& oMonsterList, CSolution& oSolution, std::vector<CSolutionPtr>& oSolutionList);
    void combination(int m, int n, std::vector<std::vector<int>>& oList);
    void doCombination(int m, int nStart, int nCount, std::vector<int>& oComb, std::vector<std::vector<int>>& oList);
    void removeMonster(std::vector<CMonster*>& oMonsterList, CTeamPtr pTeam);
    void clear();
    std::vector<CChallenge*> m_oChallengeList;
    std::vector<std::vector<CChallenge*>> m_oChallengeGroupList;
    std::vector<CMonster*> m_oMonsterList;
	std::vector<CSoulBead*> m_oSoulBeadList;
    std::vector<CFate*> m_oFateList;
    EnElementClass m_nClass;
    //CStageInfo* m_pStackTop;
};

#endif // !GAME_H
