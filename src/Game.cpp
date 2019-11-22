#include "Game.h"
#include "Challenge.h"
#include "Monster.h"
#include <algorithm>
#include <set>
#include "Skill.h"
#include "Common.h"
#include <assert.h>
#include <iterator>
#include "SoulBead.h"
#include <iostream>
#include <Windows.h>
#include "Fate.h"

CGame::CGame(): m_nClass(EC_ALL)
{
}


CGame::~CGame()
{
    clear();
}

void CGame::exclude(std::vector<std::wstring>& oMonsterList)
{
	for each (auto sName in oMonsterList)
	{
		auto itr = std::find_if(m_oMonsterList.begin(), m_oMonsterList.end(), [sName](CMonster* pMonster) {
			return pMonster->getName() == sName;
		});
		if (itr != m_oMonsterList.end())
		{
			delete *itr;
			m_oMonsterList.erase(itr);
		}
	}
}

void CGame::play(std::vector<CSolutionPtr>& oSolutionList)
{
    std::sort(m_oChallengeList.begin(), m_oChallengeList.end(), 
        [](CChallenge* pChallenge1, CChallenge* pChallenge2) {
			return pChallenge1->getMax() == pChallenge2->getMax() ? 
                sum(pChallenge1->featuresRequired()) > sum(pChallenge2->featuresRequired()) + g_dEpsilon :
                pChallenge1->getMax() < pChallenge2->getMax();
        });
    CSolution oSolution;
    play(0, m_oMonsterList, oSolution, oSolutionList);
}

void CGame::play(CChallenge * pChallenge, CResult& oResult)
{
    m_nCount = 0;
    play(pChallenge, m_oMonsterList, oResult);
}

void CGame::play(CChallenge * pChallenge, const std::vector<CMonster*>& oMonsterList, CResult& oResult)
{
    CTeam oTeam;
    // ≈≈–Ú
    char nFeatureSet = 0;
    for (int i = 0; i < EF_ALL; i++)
    {
        if (pChallenge->featuresRequired()[i] > g_dEpsilon)
        {
            nFeatureSet |= 1 << i;
        }
    }
    oResult.m_nFeatureSet = nFeatureSet;

    std::vector<CMonster*> oMonsters;
    std::copy_if(oMonsterList.begin(), oMonsterList.end(), std::back_inserter(oMonsters), [this, nFeatureSet](CMonster* pMonster) {
        return (this->m_nClass == EC_ALL || pMonster->getClass() == this->m_nClass) && pMonster->hasSpeciality(nFeatureSet);
    });

    std::sort(oMonsters.begin(), oMonsters.end(), [nFeatureSet](CMonster* pMonster1, CMonster* pMonster2) {
        return pMonster1->getFeatureSum(nFeatureSet) > pMonster2->getFeatureSum(nFeatureSet) + g_dEpsilon;
    });
    auto nTime = GetTickCount();
    m_nCount = 0;
    std::cout << ToString(pChallenge->getName()) << ":" << oMonsters.size() << "-" << pChallenge->getMax();
    play(pChallenge, oMonsters, 0, pChallenge->getMax(), oTeam, oResult);
    std::cout << "\thitcount:" << m_nCount << "\ttime:" << GetTickCount() - nTime << std::endl;
}

void CGame::simulator(std::vector<std::wstring>& oMonsterList, int * nResult)
{
    CTeam oTeam;
    for each (auto sName in oMonsterList)
    {
        auto itr = std::find_if(m_oMonsterList.begin(), m_oMonsterList.end(), [sName](CMonster* pMonster) {
            return pMonster->getName() == sName;
        });
        if (itr != m_oMonsterList.end())
        {
            oTeam.push_back(*itr);
        }
    }
    double oTotal[EF_ALL];
    calc(oTeam, oTotal);
    for (int i = 0; i < EF_ALL; i++)
    {
        nResult[i] = (int)ceil(oTotal[i] - g_dEpsilon);
    }
}

void CGame::calc(CTeam & oTeam, double * oResult)
{
    //if (oTeam.size() == 8 && oTeam[7]->getName() == L"÷«∂‡–«Œ‚”√")
    //{
    //    fill(oResult, 0.0);
    //}
    ++m_nCount;
    fill(oResult, 0.0);
    for each (auto pMonster in oTeam)
    {
        assert(pMonster != nullptr);
        addVec(oResult, pMonster->getFeatures(), oResult);
		addVec(oResult, pMonster->getSoulBead()->getFeatures(), oResult);
        int nCount = pMonster->getSkillCount();
		for (int i = 0; i < nCount; i++)
        {
            double incV[EF_ALL];
            pMonster->getSkill(i)->affect(oTeam, incV);
            addVec(oResult, incV, oResult);
        }
    }
    CBigInt nIdSet = oTeam.getMonsterSet();
    for each (auto pFate in m_oFateList)
    {
        CBigInt nFateSet = pFate->getMonsterSet();
        if ((nFateSet & nIdSet) == nFateSet)
        {
            for (int i = 0; i < pFate->getSkillCount(); i++)
            {
                double incV[EF_ALL];
                pFate->getSkill(i)->affect(oTeam, incV);
                addVec(oResult, incV, oResult);
            }
            break;
        }
    }
}

bool CGame::success(double * dChallengeRequired, double* dTeam)
{
    for (int i = 0; i < EF_ALL; i++)
    {
        if (!(ceil(dTeam[i] - g_dEpsilon) + g_dEpsilon > dChallengeRequired[i]))
            return false;
    }
    return true;
}

void CGame::play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, std::size_t nStartIndex, std::size_t nCount, CTeam& oTeam, CResult& oResult)
{
    if (nStartIndex >= oMonsterList.size() || nCount <= 0)
        return;
    for (std::size_t i = nStartIndex; i < oMonsterList.size(); i++)
    {
        oTeam.push_back(oMonsterList[i]);
        double oTotal[EF_ALL];
        calc(oTeam, oTotal);
        if (success(pChallenge->featuresRequired(), oTotal))
            oResult.add(CTeamPtr(new CTeam(oTeam)), oTotal);
        else
			play(pChallenge, oMonsterList, i + 1, nCount - 1, oTeam, oResult);
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
    CResult oResult;
    oResult.setTop(1);
    play(m_oChallengeList[nChallengeIndex], oMonsterList, oResult);
    for each (auto oItem in oResult.m_oTeamList)
    {
        oSolution.push_back(std::make_pair(m_oChallengeList[nChallengeIndex], oItem.pTeam));
        auto oList = oMonsterList;
        removeMonster(oList, oItem.pTeam);
        play(nChallengeIndex + 1, oList, oSolution, oSolutionList);
        oSolution.pop_back();
    }
    if (oResult.m_oTeamList.size() == 0)
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
	for each (auto p in m_oSoulBeadList)
	{
		delete p;
	}
    m_oSoulBeadList.clear();
    for each (auto p in m_oFateList)
    {
        delete p;
    }
    m_oFateList.clear();
}
