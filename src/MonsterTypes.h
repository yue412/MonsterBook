#ifndef MONSTERTYPES_H
#define MONSTERTYPES_H

#include <vector>
#include <memory>
#include <string>

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
const std::wstring g_sFeatureShortNames[] = { L"power", L"int", L"speed", L"charm", L"lucky", L"tech", L"all" };

const double g_dEpsilon = 1e-6;

#endif // !MONSTERTYPES_H

