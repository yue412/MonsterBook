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

const std::wstring g_sClassNames[] = { L"ľ", L"��", L"ˮ", L"��", L"��", L"��", L"ȫ" };

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

const std::wstring g_sFeatureNames[] = { L"��", L"��", L"��", L"��", L"��", L"��", L"ȫ" };

class CMonster;
typedef std::vector<CMonster*> CTeam;
typedef std::shared_ptr<CTeam> CTeamPtr;

const double g_dEpsilon = 1e-6;

#endif // !MONSTERTYPES_H

