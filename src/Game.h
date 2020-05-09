#ifndef GAME_H
#define GAME_H

#include "MonsterTypes.h"
#include "Result.h"
#include "Team.h"
#include <map>

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
    void play(std::vector<CSolutionPtr>& oSolutionList, std::vector<int>& oChallengeGroupIndexList, std::map<std::wstring, CResult>& oFailedMap);
    void play(CChallenge* pChallenge, CResult& oResult, CResult& oFailedResult);
    void simulator(std::vector<std::wstring>& oMonsterList, int* nResult);
	void exclude(std::vector<std::wstring>& oMonsterList);
    void disable_exclude_group(std::vector<std::wstring>& oList);
    static bool success(double* dChallengeRequired, double* dTeam);
    int m_nCount;
	bool m_bExportFailedInfo;
    std::vector<CMonster*> m_oMonsterList;
private:
    // ÃÙ’Ω“ªπÿ
    void play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, CResult& oResult, CResult& oFailedResult);
    void play(int nChallengeIndex, const std::vector<CMonster*>& oMonsterList, CSolution& oSolution, std::vector<CSolutionPtr>& oSolutionList, std::map<std::wstring, CResult>& oFailedMap);
    void combination(int m, int n, std::vector<std::vector<int>>& oList);
    void doCombination(int m, int nStart, int nCount, std::vector<int>& oComb, std::vector<std::vector<int>>& oList);
    void removeMonster(std::vector<CMonster*>& oMonsterList, CTeamPtr pTeam);
    void clear();
    std::vector<CChallenge*> m_oChallengeList;
    std::vector<std::vector<CChallenge*>> m_oChallengeGroupList;
	std::vector<CSoulBead*> m_oSoulBeadList;
    std::vector<CFate*> m_oFateList;
    std::vector<std::wstring> m_oExcludeMonsters;
    std::vector<int> m_oDisableExcludeGroupList;
	//EnElementClass m_nClass;
    //CStageInfo* m_pStackTop;
};

#endif // !GAME_H
