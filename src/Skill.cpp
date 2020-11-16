#include "Skill.h"
#include "Monster.h"
#include <algorithm>
#include "Common.h"
#include "Team.h"

CSkill::~CSkill()
{
}

int CSkill::getAffectFeature()
{
    return 0;
}

void CIncreaseFeatureSkill::affect(const CTeam & oTeam, double* oResult)
{
    int nCount = m_nClass == EC_ALL ? oTeam.size() : oTeam.getCount(m_nClass);
    int nVal = m_nValue * nCount;
    if (nCount < m_nTrigger)
    {
        fill(oResult, 0.0);
    }
    else if (m_nFeature == EF_ALL)
    {
		fill(oResult, nVal);
    }
    else
    {
		fill(oResult, 0.0);
        oResult[m_nFeature] = nVal;
    }
}

void CProductFeatureSkill::affect(const CTeam & oTeam, double* oResult)
{
	fill(oResult, 0.0);
    if (!m_bEnablePercent)
        return;
	for each (auto pMonster in oTeam)
	{
		if (m_nClass == EC_ALL || pMonster->getClass() == m_nClass)
		{
			for (int i = 0; i < EF_ALL; i++)
			{
				oResult[i] += m_nFeature == EF_ALL || m_nFeature == i ? pMonster->getFeature((EnFeatures)i) * (m_dValue - 1) : 0.0;
 			}
		}
	}
}

void CIncreaseSelfFeatureByCountSkill::affect(const CTeam & oTeam, double* oResult)
{
	fill(oResult, 0.0);
	for each (auto pMonster in oTeam)
	{
		if (m_nClass == EC_ALL || pMonster->getClass() == m_nClass)
		{
			for (int i = 0; i < EF_ALL; i++)
			{
				oResult[i] += m_nFeature == EF_ALL || m_nFeature == i ? m_dValue : 0.0;
			}
		}
	}
}

void CIncreaseSelfFeatureSkill::affect(const CTeam & oTeam, double * oResult)
{
    if (m_nFeature == EF_ALL)
    {
        fill(oResult, m_dValue);
    }
    else
    {
        fill(oResult, 0);
        oResult[m_nFeature] = m_dValue;
    }
}

void CIncreaseSelfFeatureByCharacterSkill::affect(const CTeam & oTeam, double * oResult)
{
    fill(oResult, 0.0);
    auto nCount = std::count_if(oTeam.begin(), oTeam.end(), [this](CMonster* pMonster) {
        return pMonster->getCharacterSet().count(this->m_sCharacter) > 0;
    });
    for (int i = 0; i < EF_ALL; i++)
    {
        oResult[i] += m_nFeatureFlag[i] * nCount * m_dValue;
    }
}

void CProductSelfFeatureByCharacterSkill::affect(const CTeam & oTeam, double * oResult)
{
    fill(oResult, 0.0);
    if (!m_bEnablePercent)
        return;
    auto nCount = std::count_if(oTeam.begin(), oTeam.end(), [this](CMonster* pMonster) {
        return pMonster->getCharacterSet().count(this->m_sCharacter) > 0;
    });
    for (int i = 0; i < EF_ALL; i++)
    {
        oResult[i] += m_nFeatureFlag[i] * nCount * m_pOwner->getFeature((EnFeatures)i) * (m_dValue);
    }
}

void CArtifactSkill::affect(const CTeam & oTeam, double * oResult)
{
    fill(oResult, 0.0);
    auto nCount = std::count_if(oTeam.begin(), oTeam.end(), [this](CMonster* pMonster) {
        return pMonster->getCharacterSet().count(L"·¨Æ÷") > 0;
    });
    if (nCount > 1)
    {
        for (int i = 0; i < EF_ALL; i++)
        {
            for each (auto pMonster in oTeam)
            {
                oResult[i] -= pMonster->getFeature((EnFeatures)i) * 0.5;
            }
        }
    }
}

void CProductFeatureByCharacterSkill::affect(const CTeam & oTeam, double * oResult)
{
    fill(oResult, 0.0);
    if (!m_bEnablePercent)
        return;
    auto nCount = std::count_if(oTeam.begin(), oTeam.end(), [this](CMonster* pMonster) {
        return pMonster->getCharacterSet().count(this->m_sCharacter) > 0;
    });
    if (nCount < m_nTrigger)
        return;
    for each (auto pMonster in oTeam)
    {
        if (m_nClass == EC_ALL || pMonster->getClass() == m_nClass)
        {
            for (int i = 0; i < EF_ALL; i++)
            {
                oResult[i] += m_nFeature == EF_ALL || m_nFeature == i ? pMonster->getFeature((EnFeatures)i) * (m_dValue - 1) : 0.0;
            }
        }
    }
}
