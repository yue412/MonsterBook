#include "Challenge.h"



CChallenge::CChallenge() : m_nMin(0), m_nMax(8), m_nClass(EC_ALL), m_nTotal(50), m_bEnableSkill(true)
{
    for (int i = 0; i < EF_ALL; i++)
    {
        m_nRequired[i] = 0.0;
    }
}


CChallenge::~CChallenge()
{
}
