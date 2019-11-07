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


#endif // !MONSTERTYPES_H

