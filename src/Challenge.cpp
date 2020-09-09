#include "Challenge.h"



CChallenge::CChallenge() : m_nMin(0), m_nMax(8), m_nClass(EC_ALL), m_nTotal(50), m_bEnableSkill(true), m_bCalcFlag(false), m_bOnlyAll(false), m_bEnablePercent(true), m_bNotGold(false)
{
    for (int i = 0; i < EF_ALL; i++)
    {
        m_nRequired[i] = 0.0;
    }
}


CChallenge::~CChallenge()
{
}
