#include "Monster.h"
#include <algorithm>
#include "Skill.h"

CMonster::CMonster() : m_nClass(EC_WOOD), m_nSpeciality(0)
{
}


CMonster::~CMonster()
{
}

void CMonster::init()
{
    m_oFeatureOrder.resize(6);
    for (int i = 0; i < EF_ALL; i++)
    {
        m_oFeatureOrder[i].dValue = m_nFeatures[i];
        m_oFeatureOrder[i].nFeature = (EnFeatures)i;
    }
    std::sort(m_oFeatureOrder.begin(), m_oFeatureOrder.end(), [](CSpecialFeature& oFeature1, CSpecialFeature& oFeature2) {
        return oFeature1.dValue > oFeature2.dValue + g_dEpsilon;
    });
    auto dVal = m_oFeatureOrder[0].dValue;
    for each (auto oFeature in m_oFeatureOrder)
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
            m_nSpeciality |= pSkill->getAffectFeature();
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
