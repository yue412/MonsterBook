#include "Skill.h"
#include "Monster.h"

CSkill::~CSkill()
{
}

void CIncreaseFeatureSkill::affect(const CTeam & oTeam, arma::rowvec6& oResult)
{
    int nCount = m_nClass == EC_ALL ? oTeam.size() : std::count_if(oTeam.begin(), oTeam.end(),
        [this](CMonster* pMonster) { return pMonster->getClass() == this->m_nClass; });
    int nVal = m_nValue * nCount;
    if (m_nFeature == EF_ALL)
    {
        oResult.fill(nVal);
    }
    else
    {
        oResult.zeros();
        oResult[m_nFeature] = nVal;
    }
}

void CGreenPlumBambooHorseSkill::affect(const CTeam & oTeam, arma::rowvec6& oResult)
{
    oResult.zeros();
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

void CProductFeatureSkill::affect(const CTeam & oTeam, arma::rowvec6 & oResult)
{
    arma::mat oClassMat(oTeam.size(), EC_ALL, arma::fill::zeros);
    for each (auto pMonster in oTeam)
        oClassMat[pMonster->getClass()] = 1.0;
    arma::vec oClassVec(oTeam.size(), arma::fill::zeros);
    if (m_nClass == EC_ALL)
        oClassVec.ones();
    else
        oClassVec[m_nClass] = 1.0;
    arma::vec oVec = oClassMat * oClassVec;
    arma::mat oFilterMat(oTeam.size(), oTeam.size(), arma::fill::zeros);
    for (arma::uword i = 0; i < oVec.n_rows; i++)
    {
        if (oVec[i] > g_dEpsilon)
            oFilterMat(i, i) = 1.0;
    }
}
