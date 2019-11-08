#include "Game.h"
#include "Challenge.h"
#include "Monster.h"
#include <algorithm>
#include <set>
#include "armadillo"
#include "Skill.h"

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
            auto vec1 = arma::rowvec6(pChallenge1->featuresRequired());
            auto vec2 = arma::rowvec6(pChallenge2->featuresRequired());
            return arma::sum(vec1) < arma::sum(vec2) - g_dEpsilon;
        });
    CSolution oSolution;
    play(0, m_oMonsterList, oSolution, oSolutionList);
}

bool CGame::play(CChallenge * pChallenge, CTeam & oTeam)
{
    arma::rowvec6 oTotal(arma::fill::zeros);
    for each (auto pMonster in oTeam)
    {
        oTotal += arma::rowvec6(pMonster->getFeatures());
        for (int i = 0; i < pMonster->getSkillCount(); i++)
        {
            arma::rowvec6 incV;
            pMonster->getSkill(i)->affect(oTeam, incV);
            oTotal += incV;
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
            oTeamList.push_back(CTeamPtr(new CTeam(oTeam))); // ”–ƒ⁄¥Ê–π¬©
        else
            play(pChallenge, oMonsterList, nStartIndex + 1, nCount - 1, oTeam, oTeamList);
        oTeam.pop_back();
    }
}

void CGame::play(int nChallengeIndex, const std::vector<CMonster*>& oMonsterList, CSolution& oSolution, std::vector<CSolutionPtr>& oSolutionList)
{
    if (nChallengeIndex >= (int)m_oChallengeList.size() || oMonsterList.empty())
    {
        oSolutionList.push_back(CSolutionPtr(new CSolution(oSolution)));
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
        return pMonster1->getFeatureSum(oFeatureSet) < pMonster2->getFeatureSum(oFeatureSet) - g_dEpsilon;
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
