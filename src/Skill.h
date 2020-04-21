#ifndef SKILL_H
#define SKILL_H

#include "MonsterTypes.h"
#include <set>

class CTeam;
class CMonster;

class CSkill
{
public:
    CSkill(): m_pOwner(nullptr) {}
    virtual ~CSkill();
    virtual void affect(const CTeam& oTeam, double* oResult) = 0;
    virtual int getAffectFeature();
    virtual void setOwner(CMonster* pOwner) { m_pOwner = pOwner; }
protected:
    CMonster* m_pOwner;
};

class CIncreaseFeatureSkill: public CSkill
{
public:
    CIncreaseFeatureSkill(EnElementClass nClass, EnFeatures nFeature, int nValue, int nTrigger = 0): m_nClass(nClass), m_nFeature(nFeature), m_nValue(nValue), m_nTrigger(nTrigger){}
	virtual void affect(const CTeam& oTeam, double* oResult);
    virtual int getAffectFeature() { return m_nFeature == EF_ALL ? 0 : 1 << m_nFeature; }
    int getValue() { return m_nValue; }
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
    virtual int getAffectFeature() { return m_nFeature == EF_ALL ? 0 : 1 << m_nFeature; }
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
    virtual int getAffectFeature() { return m_nFeature == EF_ALL ? 0 : 1 << m_nFeature; }
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
    virtual int getAffectFeature() { return m_nFeature == EF_ALL ? 0 : 1 << m_nFeature; }
private:
    EnFeatures m_nFeature;
    double m_dValue;
};

class CIncreaseSelfFeatureByCharacterSkill : public CSkill
{
public:
    CIncreaseSelfFeatureByCharacterSkill(const std::wstring& sCharacter, int* nFeatures, double dValue) : m_sCharacter(sCharacter), m_dValue(dValue), m_nAffectFeatures(0) {
        for (int i = 0; i < EF_ALL; i++)
        {
            m_nFeatureFlag[i] = nFeatures[i];
            if (nFeatures[i] > 0)
            {
                m_nAffectFeatures |= 1 << i;
            }
        }
    }
    virtual void affect(const CTeam& oTeam, double* oResult);
    virtual int getAffectFeature() { return m_nAffectFeatures; }
private:
    std::wstring m_sCharacter;
    int m_nFeatureFlag[EF_ALL];
    int m_nAffectFeatures;
    double m_dValue;
};

class CProductSelfFeatureByCharacterSkill : public CSkill
{
public:
    CProductSelfFeatureByCharacterSkill(const std::wstring& sCharacter, int* nFeatures, double dValue) : m_sCharacter(sCharacter), m_dValue(dValue), m_nAffectFeatures(0) {
        for (int i = 0; i < EF_ALL; i++)
        {
            m_nFeatureFlag[i] = nFeatures[i];
            if (nFeatures[i] > 0)
            {
                m_nAffectFeatures |= 1 << i;
            }
        }
    }
    virtual void affect(const CTeam& oTeam, double* oResult);
    virtual int getAffectFeature() { return m_nAffectFeatures; }
private:
    std::wstring m_sCharacter;
    int m_nFeatureFlag[EF_ALL];
    int m_nAffectFeatures;
    double m_dValue;
};

// ·¨Æ÷»¥³â
class CArtifactSkill : public CSkill
{
public:
    CArtifactSkill() {}
    virtual void affect(const CTeam& oTeam, double* oResult);
    virtual int getAffectFeature() { return 0; }
};


#endif // !SKILL_H