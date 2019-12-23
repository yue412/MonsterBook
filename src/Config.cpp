#include "Config.h"
#include "Game.h"
#include "tinyxml.h"
#include "Common.h"
#include "Monster.h"
#include "Challenge.h"
#include <algorithm>
#include <memory>
#include "SkillFactory.h"
#include "SoulBead.h"
#include "Fate.h"

typedef std::shared_ptr<CSkillFactory> CSkillFactoryPtr;
std::vector<CSkillFactoryPtr> g_oSkillFactory = { 
    CSkillFactoryPtr(new CIncreaseFeatureSkillFactory),
	CSkillFactoryPtr(new CProductFeatureSkillFactroy),
	CSkillFactoryPtr(new CIncreaseSelfFeatureByCountSkillFactroy),
    CSkillFactoryPtr(new CIncreaseSelfFeatureSkillFactroy),
    CSkillFactoryPtr(new CIncreaseSelfFeatureByCharacterSkillFactroy)
};

void CConfig::init(CGame * pGame)
{
    int nIndex = 0;
	auto sConfig = getExePath() + L"\\config.xml";
	TiXmlDocument oDocument;
	if (oDocument.LoadFile(ToString(sConfig), TIXML_ENCODING_UTF8))
	{
		auto pRoot = oDocument.RootElement();
		if (!pRoot)
			return;
		auto pMonstersNode = pRoot->FirstChildElement("Monsters");
		if (pMonstersNode)
		{
			auto pMonsterNode = pMonstersNode->FirstChildElement();
			while (pMonsterNode)
			{
				auto pMonster = new CMonster;
				pGame->m_oMonsterList.push_back(pMonster);
                pMonster->m_nId.setBit(nIndex, 1); ++nIndex;
				pMonster->m_sName = Utf8ToUnicode(pMonsterNode->Attribute("name"));
				pMonster->m_nClass = Name2Class(Utf8ToUnicode(pMonsterNode->Attribute("class")));
                pMonsterNode->QueryBoolAttribute("ignore", &pMonster->m_bIgnore);
                auto pCharacterNode = pMonsterNode->FirstChildElement("Character");
                if (pCharacterNode)
                {
                    auto sText = Utf8ToUnicode(pCharacterNode->GetText());
                    std::vector<std::wstring> oStrings;
                    split(sText, L'|', oStrings);
                    for each (auto str in oStrings)
                    {
                        pMonster->m_oCharacterSet.insert(str);
                    }
                }
				// Features
				initFeatures(pMonsterNode, "Features", pMonster->m_nFeatures);
				// Skills
                initSkills(pMonsterNode, pMonster->m_oSkills);
                //pMonster->init();
				pMonsterNode = pMonsterNode->NextSiblingElement();
			}
		}
		auto pChallengesNode = pRoot->FirstChildElement("Challenges");
		while (pChallengesNode)
		{
            pGame->m_oChallengeGroupList.resize(pGame->m_oChallengeGroupList.size() + 1);
            std::vector<CChallenge*>& oChallengeList = pGame->m_oChallengeGroupList.back();
            auto pChallengeNode = pChallengesNode->FirstChildElement();
			while (pChallengeNode)
			{
				auto pChallenge = new CChallenge;
				oChallengeList.push_back(pChallenge);
				pChallenge->m_sName = Utf8ToUnicode(pChallengeNode->Attribute("name"));
				pChallengeNode->Attribute("min", &pChallenge->m_nMin);
				pChallengeNode->Attribute("max", &pChallenge->m_nMax);
                pChallengeNode->Attribute("total", &pChallenge->m_nTotal);
                auto sClass = pChallengeNode->Attribute("class");
                if (sClass != nullptr)
                    pChallenge->m_nClass = Name2Class(Utf8ToUnicode(sClass));
                // Features
                auto pFeaturesRequiredNode = pChallengeNode->FirstChildElement("FeaturesRequired");
                while (pFeaturesRequiredNode)
                {
                    auto nFeature = Name2Feature(Utf8ToUnicode(pFeaturesRequiredNode->Attribute("feature")));
                    int nValue = 0;
                    pFeaturesRequiredNode->Attribute("limit", &nValue);
                    if (nFeature == EF_ALL)
                    {
                        for (int i = 0; i < EF_ALL; i++)
                        {
                            pChallenge->m_nRequired[i] = nValue;
                        }
                    }
                    else
                    {
                        pChallenge->m_nRequired[nFeature] = nValue;
                    }
                    pFeaturesRequiredNode = pFeaturesRequiredNode->NextSiblingElement("FeaturesRequired");
                }

				pChallengeNode = pChallengeNode->NextSiblingElement();
			}
            pChallengesNode = pChallengesNode->NextSiblingElement("Challenges");
		}
		auto pSoulBeadsNode = pRoot->FirstChildElement("SoulBeads");
		if (pSoulBeadsNode)
		{
			auto pSoulBeadNode = pSoulBeadsNode->FirstChildElement();
			while (pSoulBeadNode)
			{
				auto pSoulBead = new CSoulBead;
				pGame->m_oSoulBeadList.push_back(pSoulBead);
				pSoulBead->m_nClass = Name2Class(Utf8ToUnicode(pSoulBeadNode->Attribute("class")));

				// Features
				initFeatures(pSoulBeadNode, "Features", pSoulBead->m_nFeatures);

				pSoulBeadNode = pSoulBeadNode->NextSiblingElement();
			}
		}
        auto pFatesNode = pRoot->FirstChildElement("Fates");
        if (pFatesNode)
        {
            auto pFateNode = pFatesNode->FirstChildElement();
            while (pFateNode)
            {
                auto pFate = new CFate;
                pGame->m_oFateList.push_back(pFate);
                pFate->m_sName = Utf8ToUnicode(pFateNode->Attribute("name"));
                // Skills
                initSkills(pFateNode, pFate->m_oSkills);
                // Monsters
                auto pMonstersNode = pFateNode->FirstChildElement("Monsters");
                if (pMonstersNode)
                {
                    auto sMonsters = Utf8ToUnicode(pMonstersNode->GetText());
                    std::vector<std::wstring> oStringList;
                    split(sMonsters, L'|', oStringList);
                    for each (auto sName in oStringList)
                    {
                        auto itr = std::find_if(pGame->m_oMonsterList.begin(), pGame->m_oMonsterList.end(), [sName](CMonster* pMonster) {
                            return pMonster->getName() == sName;
                        });
                        if (itr != pGame->m_oMonsterList.end())
                        {
                            pFate->m_oMonsters.push_back(*itr);
                        }
                    }
                }
                pFate->init();

                pFateNode = pFateNode->NextSiblingElement();
            }
        }
    }
	for each (auto pMonster in pGame->m_oMonsterList)
	{
		auto nClass = pMonster->getClass();
		auto itr = std::find_if(pGame->m_oSoulBeadList.begin(), pGame->m_oSoulBeadList.end(), [nClass](CSoulBead* pBead) {
			return pBead->getClass() == nClass;
			});
		assert(itr != pGame->m_oSoulBeadList.end());
		if (itr != pGame->m_oSoulBeadList.end())
		{
			pMonster->init(*itr, pGame->m_oFateList);
		}
	}
}

CSkill* CConfig::createSkill(std::wstring& sName, TiXmlElement* pSkillNode)
{
    auto itr = std::find_if(g_oSkillFactory.begin(), g_oSkillFactory.end(), 
        [sName](CSkillFactoryPtr pFactory) { return pFactory->getName() == sName; });
	assert(itr != g_oSkillFactory.end());
	return itr != g_oSkillFactory.end() ? (*itr)->create(pSkillNode) : nullptr;
}

void CConfig::initFeatures(TiXmlElement* pNode, const std::string& sName, double* dFeatures)
{
	auto pFeaturesNode = pNode->FirstChildElement(sName);
	std::wstring sText;
	if (pFeaturesNode)
	{
		sText = Utf8ToUnicode(pFeaturesNode->GetText());
	}
	std::vector<std::wstring> oStringList;
	split(sText, L',', oStringList);
	for (std::size_t i = 0; i < EF_ALL; i++)
	{
		dFeatures[i] = i < oStringList.size() ? std::stod(oStringList[i]) : 0;
	}
}

void CConfig::initSkills(TiXmlElement * pNode, std::vector<CSkill*>& oSkills)
{
    auto pSkillsNode = pNode->FirstChildElement("Skills");
    if (pSkillsNode)
    {
        auto pSkillNode = pSkillsNode->FirstChildElement();
        while (pSkillNode)
        {
            std::wstring sSkillName = Utf8ToUnicode(pSkillNode->Attribute("name"));
            oSkills.push_back(createSkill(sSkillName, pSkillNode));
            pSkillNode = pSkillNode->NextSiblingElement();
        }
    }

}
