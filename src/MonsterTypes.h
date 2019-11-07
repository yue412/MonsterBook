#ifndef MONSTERTYPES_H
#define MONSTERTYPES_H

#include <vector>

enum EnElementClass
{
    EC_WOOD,
    EC_EARTH,
    EC_WATER,
    EC_FIRE,
    EC_LIGHT,
    EC_DARK
};

const std::wstring g_sClassNames[] = { L"ľ", L"��", L"ˮ", L"��", L"��", L"��" };

enum EnFeatures
{
    EF_POWER,
    EF_INTELLIGENCE,
    EF_SPEED,
    EF_CHARM,
    EF_LUCKY,
    EF_TECHNIQUE
};

class CMonster;
typedef std::vector<CMonster*> CTeam;

const std::size_t c_nFeatureCount = 6;

#endif // !MONSTERTYPES_H

