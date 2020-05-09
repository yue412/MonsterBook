#include "Result.h"
#include <algorithm>
#include <assert.h>
#include "Common.h"
#include <math.h>
//#include <mutex>

//std::mutex g_result_mutex;


CResult::CResult(): m_nNum(10)
{
    m_oOrderList.push_back(std::make_pair(RO_FACTOR, MB_ASC));
    m_oOrderList.push_back(std::make_pair(RO_CLOSE, MB_ASC));
    m_oOrderList.push_back(std::make_pair(RO_SIZE, MB_DESC));
    m_oOrderList.push_back(std::make_pair(RO_FEATURES, MB_ASC));
}

CResult::~CResult()
{
}

void CResult::add(CTeamPtr pTeam, double* dFeatures, double* dRequiredFeatures)
{
	//g_result_mutex.lock();
    CResultItem oNewItem;
    initItem(pTeam, dFeatures, dRequiredFeatures, oNewItem);

    auto itr = std::upper_bound(m_oTeamList.begin(), m_oTeamList.end(), oNewItem, [this](const CResultItem& pItem1, const CResultItem& pItem2) {
        for each (auto oOrder in m_oOrderList)
        {
            bool bResult = false;
            switch (oOrder.first)
            {
            case RO_CLOSE:
                if (compare(oOrder.second, pItem1.dTotalNeedFeature, pItem2.dTotalNeedFeature, bResult))
                    return bResult;
                else
                    break;
            case RO_FEATURES:
                if (compare(oOrder.second, pItem1.dTotalFeature, pItem2.dTotalFeature, bResult))
                    return bResult;
                else
                    break;
			case RO_CLOSE_FEATURE:
				if (compare(oOrder.second, pItem1.dClosedFeature, pItem2.dClosedFeature, bResult))
					return bResult;
				else
					break;
            case RO_MAX_CLOSE_FEATURE:
                if (compare(oOrder.second, pItem1.dMaxClosedFeature, pItem2.dMaxClosedFeature, bResult))
                    return bResult;
                else
                    break;
            case RO_FIT_FEATURE_COUNT:
				if (compare(oOrder.second, pItem1.nFitCount, pItem2.nFitCount, bResult))
					return bResult;
				else
					break;
            case RO_SIZE:
                if (compare(oOrder.second, pItem1.pTeam->size(), pItem2.pTeam->size(), bResult))
                    return bResult;
                else
                    break;
            case RO_FACTOR:
                if (compare(oOrder.second, pItem1.dFactor, pItem2.dFactor, bResult))
                    return bResult;
                else
                    break;
            default:
                assert(false);
            }
        }
        // 可能有重复
        //assert(false);
		return false;
    });
    m_oTeamList.insert(itr, oNewItem);
    if ((int)m_oTeamList.size() > m_nNum)
    {
        m_oTeamList.pop_back();
    }
	//g_result_mutex.unlock();
}

void CResult::add(CTeamPtr pTeam, double * dFeatures)
{
    CResultItem oNewItem;
    initItem(pTeam, dFeatures, nullptr, oNewItem);
    for (auto itr = m_oTeamList.begin(); itr != m_oTeamList.end(); )
    {
        auto nResult = compare(*itr, oNewItem);
        if (nResult > 0)
        {
            // 已经有大的了，不用添加
            return;
        }
        else if (nResult < 0)
        {
            itr = m_oTeamList.erase(itr); //返回下一个有效的迭代器，无需+1
        }
        else
        {
            ++itr;
        }
    }
    m_oTeamList.push_back(oNewItem);
}

void CResult::changeOrder(EnResultOrderType nType, int nAscOrDesc, int nOffset)
{
    removeOrder(nType);
    if (nOffset > (int)m_oOrderList.size())
        m_oOrderList.push_back(std::make_pair(nType, nAscOrDesc));
    else
        m_oOrderList.insert(m_oOrderList.begin() + nOffset, std::make_pair(nType, nAscOrDesc));
}

void CResult::merge(const CResult & oResult)
{
    for each (auto oItem in oResult.m_oTeamList)
    {
        this->add(oItem.pTeam, oItem.dFeatures);
    }
}

void CResult::merge(const CResult & oResult, double* dRequiredFeatures)
{
    for each (auto oItem in oResult.m_oTeamList)
    {
        this->add(oItem.pTeam, oItem.dFeatures, dRequiredFeatures);
    }
}

void CResult::removeOrder(EnResultOrderType nType)
{
    auto itr = std::find_if(m_oOrderList.begin(), m_oOrderList.end(), [nType](std::pair<EnResultOrderType, int>& oItem) {
        return oItem.first == nType;
    });
    if (itr != m_oOrderList.end())
        m_oOrderList.erase(itr);
}

void CResult::initItem(CTeamPtr pTeam, double * dFeatures, double* dRequiredFeatures, CResultItem & oItem)
{
    oItem.dTotalFeature = 0;
    for (int i = 0; i < EF_ALL; i++)
    {
        oItem.dTotalFeature += dFeatures[i];
        oItem.dFeatures[i] = dFeatures[i];
    }
	oItem.nFitCount = 0;
    oItem.dTotalNeedFeature = 0;
	oItem.dClosedFeature = 0;
    oItem.dMaxClosedFeature = 0;
    for (int i = 0; i < EF_ALL; i++)
    {
        if (1 << i & m_nFeatureSet)
        {
            oItem.dTotalNeedFeature += dFeatures[i];
			if (dRequiredFeatures)
			{
                if (!(roundEx(dFeatures[i]) + g_dEpsilon > dRequiredFeatures[i]))
                {
                    oItem.dClosedFeature += dRequiredFeatures[i] - dFeatures[i];
                    oItem.dMaxClosedFeature = std::max(oItem.dMaxClosedFeature, dRequiredFeatures[i] - dFeatures[i]);
                }
				else
					++oItem.nFitCount;
			}
        }
    }
    oItem.dFactor = pTeam->getFactor();
    oItem.pTeam = pTeam;
}

bool CResult::compare(int nAscOrDesc, double dVal1, double dVal2, bool & bResult)
{
    if (abs(dVal1 - dVal2) < g_dEpsilon)
        return false;
    bResult = nAscOrDesc == MB_ASC ? dVal1 < dVal2 : dVal1 > dVal2;
    return true;
}

int CResult::compare(const CResultItem & oItem1, const CResultItem & oItem2)
{
    bool bBig = false;
    bool bSmall = false;
    for (int i = 0; i < EF_ALL; i++)
    {
        if (oItem1.dFeatures[i] > oItem2.dFeatures[i] + g_dEpsilon)
            bBig = true;
        else if (oItem1.dFeatures[i] < oItem2.dFeatures[i] - g_dEpsilon)
            bSmall = true;
    }
    return bBig == bSmall ? 0 : (bBig ? 1 : -1);
}
