#ifndef RESULT_H
#define RESULT_H

#include "MonsterTypes.h"
#include <list>
#include <set>
#include "Team.h"

enum EnResultOrderType
{
    RO_CLOSE,
    RO_SIZE,
    RO_FEATURES
};

#define MB_DESC 1 // ½µÐò
#define MB_ASC 0

struct CResultItem
{
    double dFeatures[EF_ALL];
    double dTotalFeature;
    double dTotalNeedFeature;
    CTeamPtr pTeam;
};

class CResult
{
    friend void output(const CResult& oResult);
    friend class CGame;
public:
    CResult();
    ~CResult();
public:
    void setTop(int nNum) { m_nNum = nNum; }
    void add(CTeamPtr pTeam, double* dFeatures);
	void changeOrder(EnResultOrderType nType, int nAscOrDesc, int nOffset);
    void merge(const CResult& oResult);
private:
	void removeOrder(EnResultOrderType nType);
	void initItem(CTeamPtr pTeam, double* dFeatures, CResultItem& oItem);
    bool compare(int nAscOrDesc, double dVal1, double dVal2, bool& bResult);
    int m_nNum;
    std::list<CResultItem> m_oTeamList;
    char m_nFeatureSet;
    std::vector<std::pair<EnResultOrderType, int>> m_oOrderList;
};

#endif // !RESULT_H