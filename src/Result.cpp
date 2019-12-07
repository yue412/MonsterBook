#include "Result.h"
#include <algorithm>
#include <assert.h>
#include "Common.h"
//#include <mutex>

//std::mutex g_result_mutex;


CResult::CResult(): m_nNum(10)
{
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
            default:
                assert(false);
            }
        }
        assert(false);
		return false;
    });
    m_oTeamList.insert(itr, oNewItem);
    if ((int)m_oTeamList.size() > m_nNum)
    {
        m_oTeamList.pop_back();
    }
	//g_result_mutex.unlock();
}

void CResult::changeOrder(EnResultOrderType nType, int nAscOrDesc, int nOffset)
{
    removeOrder(nType);
    if (nOffset > (int)m_oOrderList.size())
        m_oOrderList.push_back(std::make_pair(nType, nAscOrDesc));
    else
        m_oOrderList.insert(m_oOrderList.begin() + nOffset, std::make_pair(nType, nAscOrDesc));
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
    for (int i = 0; i < EF_ALL; i++)
    {
        if (1 << i & m_nFeatureSet)
        {
            oItem.dTotalNeedFeature += dFeatures[i];
			if (dRequiredFeatures)
			{
				if (!(roundEx(dFeatures[i]) + g_dEpsilon > dRequiredFeatures[i]))
					oItem.dClosedFeature += dRequiredFeatures[i] - dFeatures[i];
				else
					++oItem.nFitCount;
			}
        }
    }
    oItem.pTeam = pTeam;
}

bool CResult::compare(int nAscOrDesc, double dVal1, double dVal2, bool & bResult)
{
    if (abs(dVal1 - dVal2) < g_dEpsilon)
        return false;
    bResult = nAscOrDesc == MB_ASC ? dVal1 < dVal2 : dVal1 > dVal2;
    return true;
}
