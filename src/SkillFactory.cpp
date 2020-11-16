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
    pNode = pSkillNode->FirstChildElement("Trigger");
    auto nTrigger = pNode ? std::stoi(pNode->GetText()) : 0;
    return new CIncreaseFeatureSkill(nClass, nFeature, nValue, nTrigger);
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

CSkill * CIncreaseSelfFeatureSkillFactroy::create(TiXmlElement * pSkillNode)
{
    auto pNode = pSkillNode->FirstChildElement("Feature");
    auto nFeature = pNode ? Name2Feature(Utf8ToUnicode(pNode->GetText())) : EF_ALL;
    pNode = pSkillNode->FirstChildElement("Value");
    auto dValue = std::stod(pNode->GetText());
    return new CIncreaseSelfFeatureSkill(nFeature, dValue);
}

CSkill * CIncreaseSelfFeatureByCharacterSkillFactroy::create(TiXmlElement * pSkillNode)
{
    auto pNode = pSkillNode->FirstChildElement("Character");
    auto sCharacter = pNode ? Utf8ToUnicode(pNode->GetText()) : L"";

    pNode = pSkillNode->FirstChildElement("Features");
    auto sFeatures = Utf8ToUnicode(pNode->GetText());
    std::vector<std::wstring> oStrings;
    split(sFeatures, L'|', oStrings);
    int nFeatureFlag[EF_ALL];
    memset(nFeatureFlag, 0, sizeof(int)*EF_ALL);
    for each (auto str in oStrings)
    {
        auto i = Name2Feature(str);
        nFeatureFlag[i] = 1;
    }

    pNode = pSkillNode->FirstChildElement("Value");
    auto dValue = std::stod(pNode->GetText());
    return new CIncreaseSelfFeatureByCharacterSkill(sCharacter, nFeatureFlag, dValue);
}

CSkill * CProductSelfFeatureByCharacterSkillFactroy::create(TiXmlElement * pSkillNode)
{
    auto pNode = pSkillNode->FirstChildElement("Character");
    auto sCharacter = pNode ? Utf8ToUnicode(pNode->GetText()) : L"";

    pNode = pSkillNode->FirstChildElement("Features");
    auto sFeatures = Utf8ToUnicode(pNode->GetText());
    std::vector<std::wstring> oStrings;
    split(sFeatures, L'|', oStrings);
    int nFeatureFlag[EF_ALL];
    memset(nFeatureFlag, 0, sizeof(int)*EF_ALL);
    for each (auto str in oStrings)
    {
        auto i = Name2Feature(str);
        nFeatureFlag[i] = 1;
    }

    pNode = pSkillNode->FirstChildElement("Value");
    auto dValue = std::stod(pNode->GetText());
    return new CProductSelfFeatureByCharacterSkill(sCharacter, nFeatureFlag, dValue);
}

CSkill * CArtifactSkillFactroy::create(TiXmlElement * pSkillNode)
{
    return new CArtifactSkill();
}

CSkill * CProductFeatureByCharacterSkillFactroy::create(TiXmlElement * pSkillNode)
{
    auto pNode = pSkillNode->FirstChildElement("ElementClass");
    auto nClass = pNode ? Name2Class(Utf8ToUnicode(pNode->GetText())) : EC_ALL;
    pNode = pSkillNode->FirstChildElement("Feature");
    auto nFeature = pNode ? Name2Feature(Utf8ToUnicode(pNode->GetText())) : EF_ALL;
    pNode = pSkillNode->FirstChildElement("Value");
    auto dValue = std::stod(pNode->GetText());
    pNode = pSkillNode->FirstChildElement("Trigger");
    auto nTrigger = pNode ? std::stoi(pNode->GetText()) : 0;
    pNode = pSkillNode->FirstChildElement("Character");
    auto sCharacter = pNode ? Utf8ToUnicode(pNode->GetText()) : L"";

    return new CProductFeatureByCharacterSkill(nTrigger, sCharacter, nClass, nFeature, dValue);
}
