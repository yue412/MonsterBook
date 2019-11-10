#include "Monster.h"



CMonster::CMonster() : m_nClass(EC_WOOD)
{
}


CMonster::~CMonster()
{
}

double CMonster::getFeatureSum(const std::set<EnFeatures>& oFeatureSet)
{
    double dResult = 0.0;
    for each (auto nFeature in oFeatureSet)
    {
        dResult += m_nFeatures[nFeature];
    }
    return dResult;
}
