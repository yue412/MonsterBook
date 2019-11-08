#ifndef SKILL_H
#define SKILL_H

#include "MonsterTypes.h"
#include "armadillo"

class CSkill
{
public:
    virtual ~CSkill();
    virtual void affect(const CTeam& oTeam, arma::rowvec6& oResult) = 0;
protected:
};

class CIncreaseFeatureSkill: public CSkill
{
public:
    CIncreaseFeatureSkill(EnElementClass nClass, EnFeatures nFeature, int nValue): m_nClass(nClass), m_nFeature(nFeature), m_nValue(nValue) {}
    virtual void affect(const CTeam& oTeam, arma::rowvec6& oResult);
private:
    EnElementClass m_nClass;
    EnFeatures m_nFeature;
    int m_nValue;
};

class CProductFeatureSkill : public CSkill
{
public:
    CProductFeatureSkill(EnElementClass nClass, EnFeatures nFeature, int nValue) : m_nClass(nClass), m_nFeature(nFeature), m_nValue(nValue) {}
    virtual void affect(const CTeam& oTeam, arma::rowvec6& oResult);
private:
    EnElementClass m_nClass;
    EnFeatures m_nFeature;
    int m_nValue;
};

class CGreenPlumBambooHorseSkill : public CSkill
{
public:
    virtual void affect(const CTeam& oTeam, arma::rowvec6& oResult);
};

#endif // !SKILL_H