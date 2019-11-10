#include "Game.h"
#include "Challenge.h"
#include "Monster.h"
#include <algorithm>
#include <set>
#include "Skill.h"
#include "Common.h"

CGame::CGame()
{
}


CGame::~CGame()
{
    clear();
}

void CGame::play(std::vector<CSolutionPtr>& oSolutionList)
{
    std::sort(m_oChallengeList.begin(), m_oChallengeList.end(), 
        [](CChallenge* pChallenge1, CChallenge* pChallenge2) {
			return sum(pChallenge1->featuresRequired()) > sum(pChallenge2->featuresRequired()) + g_dEpsilon;
        });
    CSolution oSolution;
    play(0, m_oMonsterList, oSolution, oSolutionList);
}

bool CGame::play(CChallenge * pChallenge, CTeam & oTeam)
{
    double oTotal[EF_ALL];
	fill(oTotal, 0.0);
    for each (auto pMonster in oTeam)
    {
		addVec(oTotal, pMonster->getFeatures(), oTotal);
        for (int i = 0; i < pMonster->getSkillCount(); i++)
        {
            double incV[EF_ALL];
            pMonster->getSkill(i)->affect(oTeam, incV);
			addVec(oTotal, incV, oTotal);
        }
    }
    for (int i = 0; i < EF_ALL; i++)
    {
        if (!(ceil(oTotal[i]) + g_dEpsilon > pChallenge->featuresRequired()[i]))
            return false;
    }
    return true;
}

void CGame::play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, std::size_t nStartIndex, std::size_t nCount, CTeam& oTeam, std::vector<CTeamPtr>& oTeamList)
{
    if (nStartIndex >= oMonsterList.size() || nCount <= 0)
        return;
    for (std::size_t i = nStartIndex; i < oMonsterList.size(); i++)
    {
        oTeam.push_back(oMonsterList[i]);
        if (play(pChallenge, oTeam))
            oTeamList.push_back(CTeamPtr(new CTeam(oTeam)));
        else
			play(pChallenge, oMonsterList, i + 1, nCount - 1, oTeam, oTeamList);
        oTeam.pop_back();
    }
}

void CGame::play(int nChallengeIndex, const std::vector<CMonster*>& oMonsterList, CSolution& oSolution, std::vector<CSolutionPtr>& oSolutionList)
{
    if (nChallengeIndex >= (int)m_oChallengeList.size() || oMonsterList.empty())
    {
		if (oSolution.size() > 0)
		{
			oSolutionList.push_back(CSolutionPtr(new CSolution(oSolution)));
		}
        return;
    }
    std::vector<CTeamPtr> oTeamList;
    CTeam oTeam;
    std::vector<CMonster*> oMonsters = oMonsterList;
    // ≈≈–Ú
    std::set<EnFeatures> oFeatureSet;
    for (int i = 0; i < EF_ALL; i++)
    {
        if (m_oChallengeList[nChallengeIndex]->featuresRequired()[i] > g_dEpsilon)
            oFeatureSet.insert((EnFeatures)i);
    }
    std::sort(oMonsters.begin(), oMonsters.end(), [oFeatureSet](CMonster* pMonster1, CMonster* pMonster2) {
        return pMonster1->getFeatureSum(oFeatureSet) > pMonster2->getFeatureSum(oFeatureSet) + g_dEpsilon;
    });

    play(m_oChallengeList[nChallengeIndex], oMonsters, 0, m_oChallengeList[nChallengeIndex]->getMax(), oTeam, oTeamList);
    for each (auto pTeam in oTeamList)
    {
        oSolution.push_back(std::make_pair(m_oChallengeList[nChallengeIndex], pTeam));
        auto oList = oMonsterList;
        removeMonster(oList, pTeam);
        play(nChallengeIndex + 1, oList, oSolution, oSolutionList);
        oSolution.pop_back();
    }
    play(nChallengeIndex + 1, oMonsterList, oSolution, oSolutionList);
}

void CGame::removeMonster(std::vector<CMonster*>& oMonsterList, CTeamPtr pTeam)
{
    std::for_each(pTeam->begin(), pTeam->end(), 
        [&oMonsterList](CMonster* pMonster) { 
        auto itr = std::find(oMonsterList.begin(), oMonsterList.end(), pMonster);
        if (itr != oMonsterList.end())
            oMonsterList.erase(itr);
    });
}

void CGame::clear()
{
    for each (auto p in m_oChallengeList)
    {
        delete p;
    }
    m_oChallengeList.clear();
    for each (auto p in m_oMonsterList)
    {
        delete p;
    }
    m_oMonsterList.clear();
}
