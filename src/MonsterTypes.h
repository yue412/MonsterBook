#ifndef MONSTERTYPES_H
#define MONSTERTYPES_H

#include <vector>
#include <memory>

enum EnElementClass
{
    EC_WOOD,
    EC_EARTH,
    EC_WATER,
    EC_FIRE,
    EC_LIGHT,
    EC_DARK,
    EC_ALL
};

const std::wstring g_sClassNames[] = { L"木", L"地", L"水", L"火", L"光", L"暗", L"全" };

enum EnFeatures
{
    EF_POWER,
    EF_INTELLIGENCE,
    EF_SPEED,
    EF_CHARM,
    EF_LUCKY,
    EF_TECHNIQUE,
    EF_ALL
};

const std::wstring g_sFeatureNames[] = { L"力", L"智", L"速", L"魅", L"运", L"技", L"全" };

class CMonster;
typedef std::vector<CMonster*> CTeam;
typedef std::shared_ptr<CTeam> CTeamPtr;

const double g_dEpsilon = 1e-6;

#endif // !MONSTERTYPES_H

