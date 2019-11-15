#include "Monster.h"
#include <algorithm>
#include "Skill.h"

struct CSpecialFeature
{
    EnFeatures nFeature;
    double dValue;
};

CMonster::CMonster() : m_nClass(EC_WOOD), m_nSpeciality(0)
{
}


CMonster::~CMonster()
{
}

void CMonster::init()
{
    std::vector<CSpecialFeature> oFeatureOrder(EF_ALL);
    for (int i = 0; i < EF_ALL; i++)
    {
        oFeatureOrder[i].dValue = m_nFeatures[i];
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
