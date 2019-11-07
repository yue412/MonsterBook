#include "Game.h"
#include "Challenge.h"
#include "Monster.h"


CGame::CGame()
{
}


CGame::~CGame()
{
    clear();
}

void CGame::play(std::vector<CSolution*>& oSolutionList)
{
}

void CGame::clear()
{
    for each (auto p in m_oChallengeList)
    {
        delete p;
    }
    m_oChallengeList.clear();
    for each (auto p in m_oMonsterList)
    {
        delete p;
    }
    m_oMonsterList.clear();
}
