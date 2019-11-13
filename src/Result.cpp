#include "Result.h"
#include <algorithm>
#include <assert.h>


CResult::CResult(): m_nNum(10)
{
    m_oOrderList.push_back(std::make_pair(RO_CLOSE, MB_ASC));
}

CResult::~CResult()
{
}

void CResult::add(CTeamPtr pTeam, double* dFeatures)
{
    CResultItem oNewItem;
    initItem(pTeam, dFeatures, oNewItem);

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
}

void CResult::insertOrder(EnResultOrderType nType, int nAscOrDesc, int nOffset)
{
    removeOrder(nType);
    if (nOffset > (int)m_oOrderList.size())
        m_oOrderList.push_back(std::make_pair(nType, nAscOrDesc));
    else
        m_oOrderList.insert(m_oOrderList.begin() + nOffset, std::make_pair(nType, nAscOrDesc));
}

void CResult::removeOrder(EnResultOrderType nType)
{
    auto itr = std::find_if(m_oOrderList.begin(), m_oOrderList.end(), [nType](std::pair<EnResultOrderType, int>& oItem) {
        return oItem.first == nType;
    });
    if (itr != m_oOrderList.end())
        m_oOrderList.erase(itr);
}

void CResult::initItem(CTeamPtr pTeam, double * dFeatures, CResultItem & oItem)
{
    oItem.dTotalFeature = 0;
    for (int i = 0; i < EF_ALL; i++)
    {
        oItem.dTotalFeature += dFeatures[i];
        oItem.dFeatures[i] = dFeatures[i];
    }
    oItem.dTotalNeedFeature = 0;
    for each (auto nFeature in m_oFeatureSet)
    {
        oItem.dTotalNeedFeature += dFeatures[nFeature];
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
