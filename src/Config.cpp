#include "Config.h"
#include "Game.h"
#include "tinyxml.h"
#include "Common.h"
#include "Monster.h"
#include "Challenge.h"
#include <algorithm>

void CConfig::init(CGame * pGame)
{
	auto sConfig = getExePath() + L"\\config.xml";
	TiXmlDocument oDocument;
	if (oDocument.LoadFile(toString(sConfig)))
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
				pMonster->m_sName = toWstring(pMonsterNode->Attribute("name"));
				pMonster->m_nClass = Name2Class(toWstring(pMonsterNode->Attribute("class")));
				// Features
				initFeatures(pMonsterNode, "Features", pMonster->m_nFeatures);
				// Skills
				auto pSkillsNode = pMonsterNode->FirstChildElement("Skills");
				if (pSkillsNode)
				{
					auto pSkillNode = pSkillsNode->FirstChildElement();
					while (pSkillNode)
					{
						std::wstring sSkillName = toWstring(pSkillNode->Attribute("name"));
						pMonster->m_oSkills.push_back(createSkill(sSkillName, pSkillNode));
						pSkillNode = pSkillNode->NextSiblingElement();
					}
				}

				pMonsterNode = pMonsterNode->NextSiblingElement();
			}
		}
		auto pChallengesNode = pRoot->FirstChildElement("Challenges");
		if (pChallengesNode)
		{
			auto pChallengeNode = pChallengesNode->FirstChildElement();
			while (pChallengeNode)
			{
				auto pChallenge = new CChallenge;
				pGame->m_oChallengeList.push_back(pChallenge);
				pChallenge->m_sName = toWstring(pChallengeNode->Attribute("name"));
				pChallengeNode->Attribute("min", &pChallenge->m_nMin);
				pChallengeNode->Attribute("max", &pChallenge->m_nMax);
				// Features
				initFeatures(pChallengeNode, "FeaturesRequired", pChallenge->m_nRequired);
				pChallengeNode = pChallengeNode->NextSiblingElement();
			}
		}
	}
}

EnElementClass CConfig::Name2Class(std::wstring& sName)
{
	auto itr = std::find(g_sClassNames, g_sClassNames + 6, sName);
	int n = itr - g_sClassNames;
	return (EnElementClass)n;
}

CSkill* CConfig::createSkill(std::wstring& sName, TiXmlElement* pSkillNode)
{
	return nullptr;
}

void CConfig::initFeatures(TiXmlElement* pNode, const std::string& sName, int* nFeatures)
{
	auto pFeaturesNode = pNode->FirstChildElement(sName);
	std::wstring sText;
	if (pFeaturesNode)
	{
		sText = toWstring(pFeaturesNode->GetText());
	}
	std::vector<std::wstring> oStringList;
	split(sText, L',', oStringList);
	for (std::size_t i = 0; i < c_nFeatureCount; i++)
	{
		nFeatures[i] = i < oStringList.size() ? std::stoi(oStringList[i]) : 0;
	}
}
