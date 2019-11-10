#include "Skill.h"
#include "Monster.h"
#include <algorithm>
#include "Common.h"

CSkill::~CSkill()
{
}

void CIncreaseFeatureSkill::affect(const CTeam & oTeam, double* oResult)
{
    int nCount = m_nClass == EC_ALL ? oTeam.size() : std::count_if(oTeam.begin(), oTeam.end(),
        [this](CMonster* pMonster) { return pMonster->getClass() == this->m_nClass; });
    int nVal = m_nValue * nCount;
    if (m_nFeature == EF_ALL)
    {
		fill(oResult, nVal);
    }
    else
    {
		fill(oResult, 0.0);
        oResult[m_nFeature] = nVal;
    }
}

void CGreenPlumBambooHorseSkill::affect(const CTeam & oTeam, double* oResult)
{
	fill(oResult, 0.0);
    auto itr = std::find_if(oTeam.begin(), oTeam.end(), [](CMonster* pMonster) { return pMonster->getName() == L"青梅公举"; });
    if (itr != oTeam.end())
    {
        itr = std::find_if(oTeam.begin(), oTeam.end(), [](CMonster* pMonster) { return pMonster->getName() == L"竹马王子"; });
        if (itr != oTeam.end())
        {
            oResult[EF_POWER] = 200;
        }
    }
}

void CProductFeatureSkill::affect(const CTeam & oTeam, double* oResult)
{
	fill(oResult, 0.0);
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
