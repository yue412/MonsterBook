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
#include <thread>
#include <mutex>
#include <chrono>

struct CStageInfo
{
    CChallenge* pChallenge;
    std::vector<CMonster*>* pMonsterList;
    int nCount;
    std::vector<int> oHitCounts;
    std::vector<int> oTimeList;
};

class CGame_Thread
{
public:
    static void calc(CTeam& oTeam, double* oResult);
    static bool success(double* dChallengeRequired, double* dTeam);
    static void play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, int nStartIndex, int nCount, CTeam& oTeam, CResult& oResult, int& nHitCount);
    static bool getStartIndex(std::vector<std::vector<int>>* pStartIndexList, std::vector<int>& nStartIndex);
    static void play_thread(std::vector<std::vector<int>>* pStartIndexList, CResult* pResult, CStageInfo* pInfo);
public:
    static std::vector<CFate*>* m_pFateList;
    static std::mutex m_oStageMutex;
    static int m_nIndex;
};

std::mutex CGame_Thread::m_oStageMutex;
std::vector<CFate*>* CGame_Thread::m_pFateList = nullptr;
int CGame_Thread::m_nIndex = 0;

void CGame_Thread::calc(CTeam& oTeam, double* oResult)
{
	//if (oTeam.size() == 8 && oTeam[7]->getName() == L"÷«∂‡–«Œ‚”√")
	//{
	//    fill(oResult, 0.0);
	//}
	//++m_nCount;
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
	for each (auto pFate in *m_pFateList)
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

bool CGame_Thread::success(double* dChallengeRequired, double* dTeam)
{
	for (int i = 0; i < EF_ALL; i++)
	{
		if (!(ceil(dTeam[i] - g_dEpsilon) + g_dEpsilon > dChallengeRequired[i]))
			return false;
	}
	return true;
}

void CGame_Thread::play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, int nStartIndex, int nCount, CTeam& oTeam, CResult& oResult, int& nHitCount)
{
    if (nStartIndex >= (int)oMonsterList.size() || nCount <= 0)
        return;
    for (int i = nStartIndex; i < (int)oMonsterList.size(); i++)
    {
        oTeam.push_back(oMonsterList[i]);
        double oTotal[EF_ALL];
        calc(oTeam, oTotal);
        ++nHitCount;
        if (success(pChallenge->featuresRequired(), oTotal))
            oResult.add(CTeamPtr(new CTeam(oTeam)), oTotal);
        else
            play(pChallenge, oMonsterList, i + 1, nCount - 1, oTeam, oResult, nHitCount);
        oTeam.pop_back();
    }
}

bool CGame_Thread::getStartIndex(std::vector<std::vector<int>>* pStartIndexList, std::vector<int>& nStartIndex)
{
    m_oStageMutex.lock();
    bool bResult = m_nIndex < (int)pStartIndexList->size();
    if (bResult)
        nStartIndex = (*pStartIndexList)[m_nIndex++];
    m_oStageMutex.unlock();
    return bResult;
}

void CGame_Thread::play_thread(std::vector<std::vector<int>>* pStartIndexList, CResult* pResult, CStageInfo* pInfo)
{
    std::vector<int> nStartIndex;
	while (getStartIndex(pStartIndexList, nStartIndex))
	{
        auto nTime = GetTickCount();
        int nHitCount = 0;
        CTeam oTeam;
        for each (auto i in nStartIndex)
        {
            oTeam.push_back((*pInfo->pMonsterList)[i]);
        }
        double oTotal[EF_ALL];
        calc(oTeam, oTotal);
        ++nHitCount;
        if (success(pInfo->pChallenge->featuresRequired(), oTotal))
            pResult->add(CTeamPtr(new CTeam(oTeam)), oTotal);
        else if (nStartIndex.size() == pInfo->nCount)
           play(pInfo->pChallenge, *pInfo->pMonsterList, nStartIndex.back() + 1, pInfo->pChallenge->getMax() - pInfo->nCount, oTeam, *pResult, nHitCount);
        pInfo->oTimeList.push_back(GetTickCount() - nTime);
        pInfo->oHitCounts.push_back(nHitCount);
	}
}

CGame::CGame()
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

void CGame::play(std::vector<CSolutionPtr>& oSolutionList, std::vector<int>& oChallengeGroupIndexList)
{
   // std::sort(m_oChallengeList.begin(), m_oChallengeList.end(), 
   //     [](CChallenge* pChallenge1, CChallenge* pChallenge2) {
			//return pChallenge1->getMax() == pChallenge2->getMax() ? 
   //             sum(pChallenge1->featuresRequired()) > sum(pChallenge2->featuresRequired()) + g_dEpsilon :
   //             pChallenge1->getMax() < pChallenge2->getMax();
   //     });
    m_oChallengeList.clear();
    for each (auto nIndex in oChallengeGroupIndexList)
    {
        if (nIndex < (int)m_oChallengeGroupList.size())
        {
            std::vector<CChallenge*>& oChallengeList = m_oChallengeGroupList[nIndex];
            std::copy(oChallengeList.begin(), oChallengeList.end(), std::back_inserter(m_oChallengeList));
        }
    }
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
    int nCount = 0;
    std::copy_if(oMonsterList.begin(), oMonsterList.end(), std::back_inserter(oMonsters), [&nCount, pChallenge, nFeatureSet](CMonster* pMonster) {
        return (pChallenge->requiredClass() == EC_ALL && pMonster->hasSpeciality(nFeatureSet) || pMonster->getClass() == pChallenge->requiredClass()) && ++nCount <= 50;
    });

    std::sort(oMonsters.begin(), oMonsters.end(), [nFeatureSet](CMonster* pMonster1, CMonster* pMonster2) {
        return pMonster1->getFeatureSum(nFeatureSet) > pMonster2->getFeatureSum(nFeatureSet) + g_dEpsilon;
    });
    auto nTime = GetTickCount();
    m_nCount = 0;
    std::cout << ToString(pChallenge->getName()) << ":" << oMonsters.size() << "-" << pChallenge->getMax();

    CGame_Thread::m_nIndex = 0;
    CGame_Thread::m_pFateList = &m_oFateList;

    int nPreCount = 2;
    const int nThreadCount = 8;
	std::thread oThreadList[nThreadCount];
    CResult oResultList[nThreadCount];
    CStageInfo oInfoList[nThreadCount];

    std::vector<std::vector<int>> oList;
    combination(oMonsters.size(), nPreCount, oList);
    for (int i = 0; i < nThreadCount; i++)
	{
        oResultList[i] = oResult;
        oInfoList[i].nCount = nPreCount;
        oInfoList[i].pMonsterList = &oMonsters;
        oInfoList[i].pChallenge = pChallenge;
        //nHitCountList[i] = 0;
        //nHitCountList2[i] = 0;
		oThreadList[i] = std::thread(CGame_Thread::play_thread, &oList, &oResultList[i], &oInfoList[i]);
	}
    //play(pChallenge, oMonsters, 0, pChallenge->getMax(), oTeam, oResult);
	for (int i = 0; i < nThreadCount; i++)
	{
		oThreadList[i].join();
	}

    for (int i = 0; i < nThreadCount; i++)
    {
        oResult.merge(oResultList[i]);
        /*
        std::cout << "thread:\t";
        int nTotalHitCount = 0;
        //std::cout << "hitCount:\t";
        for each (auto nHitCount in oInfoList[i].oHitCounts)
        {
            //std::cout << nHitCount << ",\t";
            nTotalHitCount += nHitCount;
        }
       // std::cout << std::endl;
        int nTotalTime = 0;
        //std::cout << "time:\t";
        for each (auto nTime in oInfoList[i].oTimeList)
        {
            //std::cout << nTime << ",\t";
            nTotalTime += nTime;
        }
        //std::cout << std::endl;
        std::cout << "total:\t" << nTotalHitCount << "," << nTotalTime << std::endl;
        //m_nCount += nHitCountList[i];
        //nCount2 += nHitCountList2[i];
        */
    }

	std::cout /*<< "\thitcount:" << m_nCount*/ << "\ttime:" << GetTickCount() - nTime /*<<"\tcreatecount:" << nCount2*/ << std::endl;
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
    CGame_Thread::m_pFateList = &m_oFateList;
    CGame_Thread::calc(oTeam, oTotal);
    for (int i = 0; i < EF_ALL; i++)
    {
        nResult[i] = (int)ceil(oTotal[i] - g_dEpsilon);
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

void CGame::combination(int m, int n, std::vector<std::vector<int>>& oList)
{
    std::vector<int> oComb;
    doCombination(m, 0, n, oComb, oList);
}

void CGame::doCombination(int m, int nStart, int nCount, std::vector<int>& oComb ,std::vector<std::vector<int>>& oList)
{
    if (nStart >= m || nCount <= 0)
    {
        return;
    }
    for (int i = nStart; i < m; i++)
    {
        oComb.push_back(i);
        oList.push_back(oComb);
        doCombination(m, i + 1, nCount - 1, oComb, oList);
        oComb.pop_back();
    }
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
    m_oChallengeList.clear();
    for each (auto oList in m_oChallengeGroupList)
    {
        for each (auto p in oList)
        {
            delete p;
        }
    }
    m_oChallengeGroupList.clear();
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
