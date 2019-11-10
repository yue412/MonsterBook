#ifndef SKILL_H
#define SKILL_H

#include "MonsterTypes.h"

class CSkill
{
public:
    virtual ~CSkill();
    virtual void affect(const CTeam& oTeam, double* oResult) = 0;
protected:
};

class CIncreaseFeatureSkill: public CSkill
{
public:
    CIncreaseFeatureSkill(EnElementClass nClass, EnFeatures nFeature, int nValue): m_nClass(nClass), m_nFeature(nFeature), m_nValue(nValue) {}
	virtual void affect(const CTeam& oTeam, double* oResult);
private:
    EnElementClass m_nClass;
    EnFeatures m_nFeature;
    int m_nValue;
};

class CProductFeatureSkill : public CSkill
{
public:
    CProductFeatureSkill(EnElementClass nClass, EnFeatures nFeature, double dValue) : m_nClass(nClass), m_nFeature(nFeature), m_dValue(dValue) {}
	virtual void affect(const CTeam& oTeam, double* oResult);
private:
    EnElementClass m_nClass;
    EnFeatures m_nFeature;
    double m_dValue;
};

class CIncreaseSelfFeatureByCountSkill : public CSkill
{
public:
	CIncreaseSelfFeatureByCountSkill(EnElementClass nClass, EnFeatures nFeature, double dValue) : m_nClass(nClass), m_nFeature(nFeature), m_dValue(dValue) {}
	virtual void affect(const CTeam& oTeam, double* oResult);
private:
	EnElementClass m_nClass;
	EnFeatures m_nFeature;
	double m_dValue;
};

class CGreenPlumBambooHorseSkill : public CSkill
{
public:
	virtual void affect(const CTeam& oTeam, double* oResult);
};

#endif // !SKILL_H