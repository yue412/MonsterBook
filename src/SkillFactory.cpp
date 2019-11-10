#include "SkillFactory.h"
#include "Skill.h"
#include "tinyxml.h"
#include "Common.h"


CSkillFactory::CSkillFactory()
{
}


CSkillFactory::~CSkillFactory()
{
}

CSkill * CIncreaseFeatureSkillFactory::create(TiXmlElement * pSkillNode)
{
    auto pNode = pSkillNode->FirstChildElement("ElementClass");
    auto nClass = pNode ? Name2Class(Utf8ToUnicode(pNode->GetText())) : EC_ALL;
    pNode = pSkillNode->FirstChildElement("Feature");
    auto nFeature = pNode ? Name2Feature(Utf8ToUnicode(pNode->GetText())) : EF_ALL;
    pNode = pSkillNode->FirstChildElement("Value");
    auto nValue = std::stoi(pNode->GetText());
    return new CIncreaseFeatureSkill(nClass, nFeature, nValue);
}

CSkill * CGreenPlumBambooHorseSkillFactroy::create(TiXmlElement * pSkillNode)
{
    return new CGreenPlumBambooHorseSkill;
}

CSkill * CProductFeatureSkillFactroy::create(TiXmlElement * pSkillNode)
{
	auto pNode = pSkillNode->FirstChildElement("ElementClass");
	auto nClass = pNode ? Name2Class(Utf8ToUnicode(pNode->GetText())) : EC_ALL;
	pNode = pSkillNode->FirstChildElement("Feature");
	auto nFeature = pNode ? Name2Feature(Utf8ToUnicode(pNode->GetText())) : EF_ALL;
	pNode = pSkillNode->FirstChildElement("Value");
	auto dValue = std::stod(pNode->GetText());
	return new CProductFeatureSkill(nClass, nFeature, dValue);
}

CSkill * CIncreaseSelfFeatureByCountSkillFactroy::create(TiXmlElement * pSkillNode)
{
	auto pNode = pSkillNode->FirstChildElement("ElementClass");
	auto nClass = pNode ? Name2Class(Utf8ToUnicode(pNode->GetText())) : EC_ALL;
	pNode = pSkillNode->FirstChildElement("Feature");
	auto nFeature = pNode ? Name2Feature(Utf8ToUnicode(pNode->GetText())) : EF_ALL;
	pNode = pSkillNode->FirstChildElement("Value");
	auto dValue = std::stod(pNode->GetText());
	return new CIncreaseSelfFeatureByCountSkill(nClass, nFeature, dValue);
}

