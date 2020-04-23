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
    RO_FEATURES,
	RO_FIT_FEATURE_COUNT,
	RO_CLOSE_FEATURE,
    RO_FACTOR
};

#define MB_DESC 1 // ½µÐò
#define MB_ASC 0

struct CResultItem
{
    double dFeatures[EF_ALL];
    double dTotalFeature;
    double dTotalNeedFeature;
	int nFitCount;
	double dClosedFeature;
    double dFactor;
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
    void add(CTeamPtr pTeam, double* dFeatures, double* dRequiredFeatures);
	void changeOrder(EnResultOrderType nType, int nAscOrDesc, int nOffset);
    void merge(const CResult& oResult, double* dRequiredFeatures);
    int size() { return (int)m_oTeamList.size(); }
    std::list<CResultItem>& getTeamList() { return m_oTeamList; }
private:
	void removeOrder(EnResultOrderType nType);
	void initItem(CTeamPtr pTeam, double* dFeatures, double* dRequiredFeatures, CResultItem& oItem);
    bool compare(int nAscOrDesc, double dVal1, double dVal2, bool& bResult);
    int m_nNum;
    std::list<CResultItem> m_oTeamList;
    char m_nFeatureSet;
    std::vector<std::pair<EnResultOrderType, int>> m_oOrderList;
};

#endif // !RESULT_H