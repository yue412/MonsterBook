#ifndef SKILL_H
#define SKILL_H

#include "MonsterTypes.h"

class CTeam;

class CSkill
{
public:
    virtual ~CSkill();
    virtual void affect(const CTeam& oTeam, double* oResult) = 0;
    virtual EnFeatures getAffectFeature();
protected:
};

class CIncreaseFeatureSkill: public CSkill
{
public:
    CIncreaseFeatureSkill(EnElementClass nClass, EnFeatures nFeature, int nValue, int nTrigger = 0): m_nClass(nClass), m_nFeature(nFeature), m_nValue(nValue), m_nTrigger(nTrigger){}
	virtual void affect(const CTeam& oTeam, double* oResult);
    virtual EnFeatures getAffectFeature() { return m_nFeature; }
private:
    EnElementClass m_nClass;
    EnFeatures m_nFeature;
    int m_nValue;
    int m_nTrigger;
};

class CProductFeatureSkill : public CSkill
{
public:
    CProductFeatureSkill(EnElementClass nClass, EnFeatures nFeature, double dValue) : m_nClass(nClass), m_nFeature(nFeature), m_dValue(dValue) {}
	virtual void affect(const CTeam& oTeam, double* oResult);
    virtual EnFeatures getAffectFeature() { return m_nFeature; }
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
    virtual EnFeatures getAffectFeature() { return m_nFeature; }
private:
	EnElementClass m_nClass;
	EnFeatures m_nFeature;
	double m_dValue;
};

class CIncreaseSelfFeatureSkill : public CSkill
{
public:
    CIncreaseSelfFeatureSkill(EnFeatures nFeature, double dValue) : m_nFeature(nFeature), m_dValue(dValue) {}
    virtual void affect(const CTeam& oTeam, double* oResult);
    virtual EnFeatures getAffectFeature() { return m_nFeature; }
private:
    EnFeatures m_nFeature;
    double m_dValue;
};

class CGreenPlumBambooHorseSkill : public CSkill
{
public:
	virtual void affect(const CTeam& oTeam, double* oResult);
    virtual EnFeatures getAffectFeature() { return EF_POWER; }
};

#endif // !SKILL_H