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
#include <numeric>

struct CStageInfo
{
    CChallenge* pChallenge;
    std::vector<CMonster*>* pMonsterList;
    int nCount;
    std::vector<__int64> oHitCounts;
    std::vector<int> oTimeList;
	bool bExportFailedInfo;
};

class CGame_Thread
{
public:
    static void calc(CTeam& oTeam, bool bEnableSkill, bool bEnablePercent, double* oResult);
    static bool success(double* dChallengeRequired, double* dTeam);
    static void play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, int nStartIndex, int nCount, CTeam& oTeam, CResult& oResult, CResult& oFailedResult, __int64& nHitCount, bool bExportFailedInfo);
    static bool getStartIndex(std::vector<std::vector<int>>* pStartIndexList, std::vector<int>& nStartIndex);
    static void play_thread(std::vector<std::vector<int>>* pStartIndexList, CResult* pResult, CResult* pFailedResult, CStageInfo* pInfo);
public:
    static std::vector<CFate*>* m_pFateList;
    static std::mutex m_oStageMutex;
    static int m_nIndex;
};

std::mutex CGame_Thread::m_oStageMutex;
std::vector<CFate*>* CGame_Thread::m_pFateList = nullptr;
int CGame_Thread::m_nIndex = 0;

void CGame_Thread::calc(CTeam& oTeam, bool bEnableSkill, bool bEnablePercent, double* oResult)
{
	//if (oTeam.size() == 8 && oTeam[7]->getName() == L"智多星吴用")
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
        if (!bEnableSkill)
            continue;
        int nCount = pMonster->getSkillCount();
        for (int i = 0; i < nCount; i++)
        {
            double incV[EF_ALL];
            pMonster->getSkill(i)->setEnablePercent(bEnablePercent);
            pMonster->getSkill(i)->affect(oTeam, incV);
            addVec(oResult, incV, oResult);
        }
	}
	
    if (!bEnableSkill)
        return;
	CBigInt nIdSet = oTeam.getMonsterSet();
	for each (auto pFate in *m_pFateList)
	{
		CBigInt nFateSet = pFate->getMonsterSet();
		if ((nFateSet & nIdSet) == nFateSet)
		{
			for (int i = 0; i < pFate->getSkillCount(); i++)
			{
				double incV[EF_ALL];
                pFate->getSkill(i)->setEnablePercent(bEnablePercent);
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
		if (!(roundEx(dTeam[i]) + g_dEpsilon > dChallengeRequired[i]))
			return false;
	}
	return true;
}

void CGame_Thread::play(CChallenge* pChallenge, const std::vector<CMonster*>& oMonsterList, int nStartIndex, int nCount, CTeam& oTeam, CResult& oResult, CResult& oFailedResult, __int64& nHitCount, bool bExportFailedInfo)
{
    if (nStartIndex >= (int)oMonsterList.size() || nCount <= 0)
        return;
    for (int i = nStartIndex; i < (int)oMonsterList.size(); i++)
    {
        if (pChallenge->requiredClass() == EC_ALL)
        {
            auto sName = oMonsterList[i]->getName();
            auto itr = std::find_if(oTeam.begin(), oTeam.end(), [sName](CMonster* pMonster) { return pMonster->getName() == sName; });
            if (itr != oTeam.end())
                continue; // 有重复名称的Monster，幻化
        }
        oTeam.push_back(oMonsterList[i]);
        if (oTeam.size() >= pChallenge->getMin())
        {
            double oTotal[EF_ALL];
            calc(oTeam, pChallenge->enableSkill(), pChallenge->enablePercent(), oTotal);
            ++nHitCount;
            if (!pChallenge->calcFlag())
            {
                if (success(pChallenge->featuresRequired(), oTotal))
                    oResult.add(CTeamPtr(new CTeam(oTeam)), oTotal, nullptr);
                else
                {
                    if (oResult.size() == 0 && bExportFailedInfo)
                        oFailedResult.add(CTeamPtr(new CTeam(oTeam)), oTotal, pChallenge->featuresRequired());
                    play(pChallenge, oMonsterList, i + 1, nCount - 1, oTeam, oResult, oFailedResult, nHitCount, bExportFailedInfo);
                }
            }
            else
            {
                oResult.add(CTeamPtr(new CTeam(oTeam)), oTotal);
                play(pChallenge, oMonsterList, i + 1, nCount - 1, oTeam, oResult, oFailedResult, nHitCount, bExportFailedInfo);
            }
        }
        else
        {
            play(pChallenge, oMonsterList, i + 1, nCount - 1, oTeam, oResult, oFailedResult, nHitCount, bExportFailedInfo);
        }
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

void CGame_Thread::play_thread(std::vector<std::vector<int>>* pStartIndexList, CResult* pResult, CResult* pFailedResult, CStageInfo* pInfo)
{
    std::vector<int> nStartIndex;
	while (getStartIndex(pStartIndexList, nStartIndex))
	{
        auto nTime = GetTickCount();
        __int64 nHitCount = 0;
        CTeam oTeam;
        for each (auto i in nStartIndex)
        {
            oTeam.push_back((*pInfo->pMonsterList)[i]);
        }
        if (pInfo->pChallenge->requiredClass() == EC_ALL)
        {
            //std::cout << "1" << std::endl;
            //for each (auto pMonster in oTeam)
            //{
            //    std::cout << pMonster->getName().c_str() << std::endl;
            //}
            std::sort(oTeam.begin(), oTeam.end(), [](CMonster* pMonster1, CMonster* pMonster2) { 
                auto nResult = pMonster1->getName().compare(pMonster2->getName()) > 0;
                return nResult;
            });
//            std::cout << "2" << std::endl;
            auto itr = std::unique(oTeam.begin(), oTeam.end());
            if (itr != oTeam.end())
                continue;
        }
        if (oTeam.size() >= pInfo->pChallenge->getMin())
        {
            double oTotal[EF_ALL];
            calc(oTeam, pInfo->pChallenge->enableSkill(), pInfo->pChallenge->enablePercent(), oTotal);
            ++nHitCount;
            if (!pInfo->pChallenge->calcFlag())
            {
                if (success(pInfo->pChallenge->featuresRequired(), oTotal))
                    pResult->add(CTeamPtr(new CTeam(oTeam)), oTotal, nullptr);
                else
                {
                    if (pResult->size() == 0 && pInfo->bExportFailedInfo)
                        pFailedResult->add(CTeamPtr(new CTeam(oTeam)), oTotal, pInfo->pChallenge->featuresRequired());
                    if (nStartIndex.size() == pInfo->nCount)
                        play(pInfo->pChallenge, *pInfo->pMonsterList, nStartIndex.back() + 1, pInfo->pChallenge->getMax() - pInfo->nCount, oTeam, *pResult, *pFailedResult, nHitCount, pInfo->bExportFailedInfo);
                }
            }
            else
            {
                pResult->add(CTeamPtr(new CTeam(oTeam)), oTotal);
                if (nStartIndex.size() == pInfo->nCount)
                    play(pInfo->pChallenge, *pInfo->pMonsterList, nStartIndex.back() + 1, pInfo->pChallenge->getMax() - pInfo->nCount, oTeam, *pResult, *pFailedResult, nHitCount, pInfo->bExportFailedInfo);
            }
        }
        else
        {
            if (nStartIndex.size() == pInfo->nCount)
                play(pInfo->pChallenge, *pInfo->pMonsterList, nStartIndex.back() + 1, pInfo->pChallenge->getMax() - pInfo->nCount, oTeam, *pResult, *pFailedResult, nHitCount, pInfo->bExportFailedInfo);
        }
        pInfo->oTimeList.push_back(GetTickCount() - nTime);
        pInfo->oHitCounts.push_back(nHitCount);
	}
}

CGame::CGame(): m_bExportFailedInfo(false)
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
        for (auto itr = m_oMonsterList.begin(); itr != m_oMonsterList.end(); )
        {
            if ((*itr)->getName() == sName)
            {
                delete *itr;
                itr = m_oMonsterList.erase(itr);
            }
            else
            {
                itr++;
            }
        }
	}
}

void CGame::disable_exclude_group(std::vector<std::wstring>& oList)
{
    for each (auto str in oList)
    {
        this->m_oDisableExcludeGroupList.push_back(std::stoi(str));
    }
}

bool CGame::success(double * dChallengeRequired, double * dTeam)
{
    return CGame_Thread::success(dChallengeRequired, dTeam);
}

void CGame::play(std::vector<CSolutionPtr>& oSolutionList, std::vector<int>& oChallengeGroupIndexList, std::map<std::wstring, CResult>& oFailedMap)
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
    play(0, m_oMonsterList, oSolution, oSolutionList, oFailedMap);
}

void CGame::play(CChallenge * pChallenge, CResult& oResult, CResult& oFailedResult)
{
    m_nCount = 0;
    play(pChallenge, m_oMonsterList, oResult, oFailedResult);
}

void CGame::play(CChallenge * pChallenge, const std::vector<CMonster*>& oMonsterList, CResult& oResult, CResult& oFailedResult)
{
    CTeam oTeam;
    // 排序
    char nFeatureSet = 0;
    for (int i = 0; i < EF_ALL; i++)
    {
        if (pChallenge->featuresRequired()[i] > g_dEpsilon)
        {
            nFeatureSet |= 1 << i;
        }
    }
    oResult.m_nFeatureSet = nFeatureSet;
    oFailedResult.m_nFeatureSet = nFeatureSet;

    std::vector<CMonster*> oMonsters;
    std::vector<CMonster*> oTempMonsters;
    std::copy_if(oMonsterList.begin(), oMonsterList.end(), std::back_inserter(oTempMonsters), [pChallenge, nFeatureSet](CMonster* pMonster) {
        return /*pChallenge->requiredCharacter().empty() && */!pMonster->ignore() &&
            (!pChallenge->OnlyAll() || pMonster->isAffectAll()) &&
            (pChallenge->requiredClass() == EC_ALL && pMonster->hasSpeciality(nFeatureSet) || pMonster->getClass() == pChallenge->requiredClass()) &&
            (pChallenge->requiredCharacter().empty() || pMonster->getCharacterSet().count(pChallenge->requiredCharacter()) > 0) &&
            (!pChallenge->NotGold() || !pMonster->isGold())
            ;
    });

    std::sort(oTempMonsters.begin(), oTempMonsters.end(), [nFeatureSet](CMonster* pMonster1, CMonster* pMonster2) {
        return pMonster1->getFeatureSum(nFeatureSet) > pMonster2->getFeatureSum(nFeatureSet) + g_dEpsilon;
    });

    int nCount = 0;
    std::copy_if(oTempMonsters.begin(), oTempMonsters.end(), std::back_inserter(oMonsters), [&nCount, pChallenge](CMonster* pMonster) {
        return ++nCount <= pChallenge->getTotal();
    });
    // && ++nCount <= pChallenge->getTotal();

    auto nTime = GetTickCount();
    m_nCount = 0;
    std::cout << ToString(pChallenge->getName()) << ":" << oMonsters.size() << "-" << pChallenge->getMax();

    CGame_Thread::m_nIndex = 0;
    CGame_Thread::m_pFateList = &m_oFateList;

    int nPreCount = pChallenge->getMax() < 2 ? 1 : 2;
    const int nThreadCount = 8;
	std::thread oThreadList[nThreadCount];
    CResult oResultList[nThreadCount];
    CResult oFailedResultList[nThreadCount];
    CStageInfo oInfoList[nThreadCount];
    //int nHitCountList[nThreadCount];

    std::vector<std::vector<int>> oList;
    combination(oMonsters.size(), nPreCount, oList);
    for (int i = 0; i < nThreadCount; i++)
	{
        oResultList[i] = oResult;
        oFailedResultList[i] = oFailedResult;
        oInfoList[i].nCount = nPreCount;
        oInfoList[i].pMonsterList = &oMonsters;
        oInfoList[i].pChallenge = pChallenge;
		oInfoList[i].bExportFailedInfo = m_bExportFailedInfo;
        //nHitCountList[i] = 0;
        //nHitCountList2[i] = 0;
		oThreadList[i] = std::thread(CGame_Thread::play_thread, &oList, &oResultList[i], &oFailedResultList[i], &oInfoList[i]);
	}
    //play(pChallenge, oMonsters, 0, pChallenge->getMax(), oTeam, oResult);
	for (int i = 0; i < nThreadCount; i++)
	{
		oThreadList[i].join();
	}
    __int64 nTotalHitCount = 0;
    for (int i = 0; i < nThreadCount; i++)
    {
        if (pChallenge->calcFlag())
        {
            oResult.merge(oResultList[i]);
        }
        else
        {
            oResult.merge(oResultList[i], pChallenge->featuresRequired());
            oFailedResult.merge(oFailedResultList[i], pChallenge->featuresRequired());
        }
        nTotalHitCount += std::accumulate(oInfoList[i].oHitCounts.begin(), oInfoList[i].oHitCounts.end(), 0i64);
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

	std::cout << "\thitcount:" << nTotalHitCount << "\ttime:" << GetTickCount() - nTime /*<<"\tcreatecount:" << nCount2*/ << std::endl;
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
    CGame_Thread::calc(oTeam, true, true, oTotal);
    for (int i = 0; i < EF_ALL; i++)
    {
        nResult[i] = roundEx(oTotal[i]);
    }
}

void CGame::play(int nChallengeIndex, const std::vector<CMonster*>& oMonsterList, CSolution& oSolution, std::vector<CSolutionPtr>& oSolutionList, std::map<std::wstring, CResult>& oFailedMap)
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
    CResult oFailedResult;
    oFailedResult.setTop(1);
    oFailedResult.changeOrder(RO_FIT_FEATURE_COUNT, MB_DESC, 0);
	oFailedResult.changeOrder(RO_CLOSE_FEATURE, MB_ASC, 1);
	play(m_oChallengeList[nChallengeIndex], oMonsterList, oResult, oFailedResult);
    for each (auto oItem in oResult.m_oTeamList)
    {
        oSolution.push_back(std::make_pair(m_oChallengeList[nChallengeIndex], oItem.pTeam));
        auto oList = oMonsterList;
        removeMonster(oList, oItem.pTeam);
        play(nChallengeIndex + 1, oList, oSolution, oSolutionList, oFailedMap);
        oSolution.pop_back();
    }
    if (oResult.m_oTeamList.size() == 0)
    {
        auto sName = m_oChallengeList[nChallengeIndex]->getName();
        if (oFailedMap.find(sName) == oFailedMap.end())
        {
            oFailedMap[sName] = oFailedResult;
        }
        play(nChallengeIndex + 1, oMonsterList, oSolution, oSolutionList, oFailedMap);
    }
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
