#include "Challenge.h"



CChallenge::CChallenge() : m_nMin(0), m_nMax(8), m_nClass(EC_ALL)
{
    for (int i = 0; i < EF_ALL; i++)
    {
        m_nRequired[i] = 0.0;
    }
}


CChallenge::~CChallenge()
{
}
