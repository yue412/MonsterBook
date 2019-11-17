#ifndef SKILLFACTORY_H
#define SKILLFACTORY_H

#include <vector>
#include <string>

class CSkill;
class TiXmlElement;

class CSkillFactory
{
public:
    CSkillFactory();
    virtual ~CSkillFactory();
public:
    virtual std::wstring getName() = 0;
    virtual CSkill* create(TiXmlElement* pSkillNode) = 0;
};

class CIncreaseFeatureSkillFactory : public CSkillFactory
{
public:
    virtual std::wstring getName() { return L"IncreaseFeature"; }
    virtual CSkill* create(TiXmlElement* pSkillNode);
};

class CGreenPlumBambooHorseSkillFactroy : public CSkillFactory
{
public:
    virtual std::wstring getName() { return L"GreenPlumBambooHorse"; }
    virtual CSkill* create(TiXmlElement* pSkillNode);
};

class CProductFeatureSkillFactroy : public CSkillFactory
{
public:
	virtual std::wstring getName() { return L"ProductFeature"; }
	virtual CSkill* create(TiXmlElement* pSkillNode);
};

class CIncreaseSelfFeatureByCountSkillFactroy : public CSkillFactory
{
public:
	virtual std::wstring getName() { return L"IncreaseSelfFeatureByCount"; }
	virtual CSkill* create(TiXmlElement* pSkillNode);
};

class CIncreaseSelfFeatureSkillFactroy : public CSkillFactory
{
public:
    virtual std::wstring getName() { return L"IncreaseSelfFeature"; }
    virtual CSkill* create(TiXmlElement* pSkillNode);
};

#endif // !SKILLFACTORY_H