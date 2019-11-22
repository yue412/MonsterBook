#include "Monster.h"
#include <algorithm>
#include "Skill.h"
#include "SoulBead.h"
#include "Fate.h"

struct CSpecialFeature
{
    EnFeatures nFeature;
    double dValue;
};

CMonster::CMonster() : m_nClass(EC_WOOD), m_nSpeciality(0), m_pSoulBead(nullptr)
{
}


CMonster::~CMonster()
{
}

void CMonster::init(CSoulBead* pSoulBead, const std::vector<CFate*>& oFateList)
{
	m_pSoulBead = pSoulBead;
    std::vector<CSpecialFeature> oFeatureOrder(EF_ALL);
    for (int i = 0; i < EF_ALL; i++)
    {
        oFeatureOrder[i].dValue = m_nFeatures[i] + ((pSoulBead != nullptr) ? pSoulBead->getFeature((EnFeatures)i) : 0.0);
        oFeatureOrder[i].nFeature = (EnFeatures)i;
    }
    std::sort(oFeatureOrder.begin(), oFeatureOrder.end(), [](CSpecialFeature& oFeature1, CSpecialFeature& oFeature2) {
        return oFeature1.dValue > oFeature2.dValue + g_dEpsilon;
    });
    auto dVal = oFeatureOrder[0].dValue;
    for each (auto oFeature in oFeatureOrder)
    {
        if ((dVal - oFeature.dValue) / dVal < 0.2)
            m_nSpeciality |= 1 << oFeature.nFeature;
        else
            break;
    }
    for each (auto pSkill in m_oSkills)
    {
        if (pSkill->getAffectFeature() != EF_ALL)
        {
            m_nSpeciality |= 1 << pSkill->getAffectFeature();
        }
    }
    /*
    for each (auto pFate in oFateList)
    {
        for (int i = 0; i < pFate->getMonsterCount(); i++)
        {
            if (pFate->getMonster(i) == this)
            {
                for (int j = 0; j < pFate->getSkillCount(); j++)
                {
                    auto pSkill = pFate->getSkill(j);
                    if (pSkill->getAffectFeature() != EF_ALL)
                    {
                        m_nSpeciality |= 1 << pSkill->getAffectFeature();
                    }
                }
                return;
            }
        }
    }
    */
}

double CMonster::getFeatureSum(char nFeatureSet)
{
    double dResult = 0.0;
    for (int i = 0; i < EF_ALL; i++)
    {
        if ((1 << i) & nFeatureSet)
        {
            dResult += m_nFeatures[i];
        }
    }
    return dResult;
}

bool CMonster::hasSpeciality(char nSpecialitySet)
{
    return (nSpecialitySet & m_nSpeciality) != 0;
}
