#include "Team.h"
#include "Monster.h"

CTeam::CTeam()
{
    memset(m_nCount, 0, sizeof(int) * EC_ALL);
}

CTeam::~CTeam()
{
}

std::vector<CMonster*>::iterator CTeam::begin()
{
    return this->m_oMonsterList.begin();
}

std::vector<CMonster*>::const_iterator CTeam::begin() const
{
    return m_oMonsterList.begin();
}

std::vector<CMonster*>::iterator CTeam::end()
{
    return m_oMonsterList.end();
}

std::vector<CMonster*>::const_iterator CTeam::end() const
{
    return m_oMonsterList.end();
}

void CTeam::push_back(CMonster * pMonster)
{
    m_nCount[pMonster->getClass()] += 1;
    m_oMonsterList.push_back(pMonster);
}

void CTeam::pop_back()
{
    if (!m_oMonsterList.empty())
    {
        auto pMonster = m_oMonsterList[m_oMonsterList.size() - 1];
        m_oMonsterList.pop_back();
        m_nCount[pMonster->getClass()] -= 1;
    }
}

int CTeam::size() const
{
    return (int)m_oMonsterList.size();
}

int CTeam::getCount(EnElementClass nClass) const
{
    return m_nCount[nClass];
}

CMonster * CTeam::getItems(int nIndex) const
{
    return m_oMonsterList[nIndex];
}
