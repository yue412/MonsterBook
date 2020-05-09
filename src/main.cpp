#include <string>
#include <iostream>
#include <algorithm>
#include "Game.h"
#include "Config.h"
#include "Challenge.h"
#include "Monster.h"
#include <Windows.h>
#include "Common.h"
#include "Result.h"
#include <map>
#include <iterator>
#include <fstream>
#include "tinyxml.h"

void output(std::vector<CSolutionPtr>& oResultList)
{
    std::wstring sNames;
    for (auto itr = oResultList.begin(); itr != oResultList.end(); itr++)
    {
        auto nIndex = itr - oResultList.begin() + 1;
		if (nIndex > 10)
		{
			break;
		}
        std::cout << "Solution " << nIndex << ":" << (*itr)->size() << std::endl;
        for each (auto pair in **itr)
        {
            std::cout << ToString(pair.first->getName()) << ":";
            std::stable_sort(pair.second->begin(), pair.second->end(), [](CMonster* pMonster1, CMonster* pMonster2) {
                return pMonster1->getClass() < pMonster2->getClass(); });
            for each (auto pMonster in *pair.second)
            {
                sNames += pMonster->getName() + L",";
                std::cout << ToString(pMonster->getName()) << "[" << ToString(g_sClassNames[pMonster->getClass()]) << "],";
            }
            std::cout << std::endl;
        }
    }
    std::cout << ToString(sNames) << std::endl;
}

std::wstring getTeamStr(CTeamPtr& pTeam)
{
    std::wstring sResult;
    for each (auto pMonster in *pTeam)
    {
        sResult += pMonster->getName() + L"["  + g_sClassNames[pMonster->getClass()] + L"],";
    }
    return sResult;
}

std::wstring getTeamFeatureStr(double* dFeatures)
{
    std::wstring sResult;
    for (int i = 0; i < EF_ALL; i++)
    {
        sResult += std::to_wstring(roundEx(dFeatures[i])) + L"("  + g_sFeatureNames[i] + L"),";
    }
    return sResult;
}

void output(const CResult& oResult)
{
    for each (auto oItem in oResult.m_oTeamList)
    {
        std::cout << ToString(getTeamStr(oItem.pTeam));
        std::cout << std::endl;
        std::cout << oItem.dTotalFeature << "\t";
        std::cout << oItem.dTotalNeedFeature << "\t";
        std::cout << ToString(getTeamFeatureStr(oItem.dFeatures));
        std::cout << std::endl;
    }
}

void parseParams(const std::wstring& sParams, std::map<std::wstring, std::wstring>& oParamsMap)
{
        std::vector<std::wstring> oList;
        split(sParams, L'=', oList);
        if (oList.size()>1)
            oParamsMap[oList[0]] = oList[1];
}

bool getOrderInfo(const std::wstring& sName, const std::map<std::wstring, std::wstring>& oParamsMap,  int& nAscOrDesc, int& nOffset)
{
	auto itr = oParamsMap.find(sName);
	if (itr != oParamsMap.end())
	{
		auto s = itr->second;
		std::vector<std::wstring> oStringList;
		split(s, L',', oStringList);
		if (oStringList.size() == 2)
		{
			nAscOrDesc = oStringList[0] == L"asc" ? MB_ASC : MB_DESC;
			nOffset = std::stoi(oStringList[1]);
			return true;
		}
	}
	return false;
}

void initResult(std::map<std::wstring, std::wstring>& oParamsMap, CResult& oResultList)
{
	int nAscOrDesc;
	int nOffset;
	if (getOrderInfo(L"size", oParamsMap, nAscOrDesc, nOffset))
		oResultList.changeOrder(RO_SIZE, nAscOrDesc, nOffset);
	if (getOrderInfo(L"features", oParamsMap, nAscOrDesc, nOffset))
		oResultList.changeOrder(RO_FEATURES, nAscOrDesc, nOffset);
	if (getOrderInfo(L"close", oParamsMap, nAscOrDesc, nOffset))
		oResultList.changeOrder(RO_CLOSE, nAscOrDesc, nOffset);
	oResultList.setTop(oParamsMap.find(L"top") == oParamsMap.end() ? 10 : std::stoi(oParamsMap[L"top"]));
}

void initChallenge(std::map<std::wstring, std::wstring>& oParamsMap, CChallenge& oChallenge)
{
    oChallenge.m_sName = oParamsMap.find(L"name") == oParamsMap.end() ? L"" : oParamsMap[L"name"];
	oChallenge.m_nMin = oParamsMap.find(L"min") == oParamsMap.end() ? 0 : std::stoi(oParamsMap[L"min"]);
    oChallenge.m_nMax = oParamsMap.find(L"max") == oParamsMap.end() ? 8 : std::stoi(oParamsMap[L"max"]);
    oChallenge.m_nTotal = oParamsMap.find(L"total") == oParamsMap.end() ? 50 : std::stoi(oParamsMap[L"total"]);
    oChallenge.m_nClass = oParamsMap.find(L"class") == oParamsMap.end() ? EC_ALL : Name2Class(oParamsMap[L"class"]);
    oChallenge.m_bEnableSkill = oParamsMap.find(L"enable_skill") == oParamsMap.end() ? true : oParamsMap[L"enable_skill"] == L"true";
    oChallenge.m_sCharacter = oParamsMap.find(L"character") == oParamsMap.end() ? L"" : oParamsMap[L"character"];
    oChallenge.m_bOnlyAll = oParamsMap.find(L"only_all") == oParamsMap.end() ? false : true;
	for (int i = 0; i < EF_ALL; i++)
	{
		oChallenge.m_nRequired[i] = oParamsMap.find(g_sFeatureShortNames[i]) == oParamsMap.end() ? 0 : std::stoi(oParamsMap[g_sFeatureShortNames[i]]);
	}
}

void getGroupList(std::map<std::wstring, std::wstring>& oParamsMap, std::vector<int>& oGroupList)
{
    if (oParamsMap.find(L"group") == oParamsMap.end())
        oGroupList.push_back(0);
    else
    {
        std::vector<std::wstring> oStringList;
        split(oParamsMap[L"group"], L',', oStringList);
        std::transform(oStringList.begin(), oStringList.end(), std::back_inserter(oGroupList), [](const std::wstring& str) { return std::stoi(str); });
    }
}

//bool isSetChallenge(std::map<std::wstring, std::wstring>& oParamsMap)
//{
//    bool bResult = false;
//    for (int i = 0; i < EF_ALL; i++)
//    {
//        if (oParamsMap.find(g_sFeatureShortNames[i]) != oParamsMap.end())
//        {
//            bResult = true;
//            break;
//        }
//    }
//    return bResult;
//}

struct CHistoryItem
{
    CHistoryItem() : bSuccess(true) {}

    std::wstring sChallenge;
    bool bSuccess;
    std::wstring sMonsters;
    std::wstring sFeatures;
};

std::string getNodeText(TiXmlElement* pNode)
{
    if (pNode)
    {
        const char* str = pNode->GetText();
        if (str)
            return str;
    }
    return "";
}

void getHistroyList(std::vector<CHistoryItem>& oHistoryList)
{
    auto sConfig = getFullPath(L"MonsterBookHistory.log");
    TiXmlDocument oDocument;
    if (oDocument.LoadFile(ToString(sConfig), TIXML_ENCODING_UTF8))
    {
        auto pRoot = oDocument.RootElement();
        if (!pRoot)
            return;
        auto pItemNode = pRoot->FirstChildElement("Item");
        while (pItemNode)
        {
            CHistoryItem oNewItem;
            oNewItem.sChallenge = Utf8ToUnicode(getNodeText(pItemNode->FirstChildElement("Challenge")));
            oNewItem.bSuccess = std::stoi(getNodeText(pItemNode->FirstChildElement("Success"))) != 0;
            oNewItem.sMonsters = Utf8ToUnicode(getNodeText(pItemNode->FirstChildElement("Monsters")));
            oNewItem.sFeatures = Utf8ToUnicode(getNodeText(pItemNode->FirstChildElement("Features")));
            oHistoryList.push_back(oNewItem);
            pItemNode = pItemNode->NextSiblingElement();
        }
    }
}

std::wstring getChallengeStr(CChallenge* pChallenge)
{
    std::wstring sResult;
    if (!pChallenge->getName().empty())
        sResult += L"name=" + pChallenge->getName() + L" ";
    if (pChallenge->getMin() != 0)
        sResult += L"min=" + ToUnicode(std::to_string(pChallenge->getMin())) + L" ";
    if (pChallenge->getMax() != 8)
        sResult += L"max=" + ToUnicode(std::to_string(pChallenge->getMax())) + L" ";
    if (pChallenge->requiredClass() != EC_ALL)
        sResult += L"class=" + g_sClassNames[pChallenge->requiredClass()] + L" ";
    if (!pChallenge->enableSkill())
        sResult += L"enable_skill=false ";
    if (!pChallenge->requiredCharacter().empty())
        sResult += L"character=" + pChallenge->requiredCharacter() + L" ";
    for (int i = 0; i < EF_ALL; i++)
    {
        double dVal = pChallenge->featuresRequired()[i];
        if (abs(dVal) > g_dEpsilon) // 不为零
        {
            sResult += g_sFeatureShortNames[i] + L"=" + ToUnicode(std::to_string((int)dVal)) + L" ";
        }
    }
    return sResult.substr(0, sResult.length() - 1);
}

CHistoryItem makeHistoryItem(CChallenge* pChallenge, bool bSuccess, CResult& oResult)
{
    CHistoryItem oItem;
    oItem.sChallenge = getChallengeStr(pChallenge);
    oItem.bSuccess = bSuccess;
    
    if (!oResult.getTeamList().empty())
    {
        oItem.sMonsters = getTeamStr(oResult.getTeamList().front().pTeam);
        oItem.sFeatures = getTeamFeatureStr(oResult.getTeamList().front().dFeatures);
    }

    return oItem;
}

TiXmlNode* createTextNode(std::string sName, std::string sValue)
{
    TiXmlElement* pNode = new TiXmlElement(sName);
    pNode->LinkEndChild(new TiXmlText(sValue));
    return pNode;
}

void saveHistoryList(std::vector<CHistoryItem>& oList)
{
    TiXmlDocument oDocument;
    TiXmlElement *pRoot = new TiXmlElement("MonsterBook");
    oDocument.LinkEndChild(pRoot);
    for each (auto oItem in oList)
    {
        TiXmlElement* pItemNode = new TiXmlElement("Item");
        pRoot->LinkEndChild(pItemNode);

        pItemNode->LinkEndChild(createTextNode("Challenge", UnicodeToUtf8(oItem.sChallenge)));
        pItemNode->LinkEndChild(createTextNode("Success", std::to_string(oItem.bSuccess)));
        pItemNode->LinkEndChild(createTextNode("Monsters", UnicodeToUtf8(oItem.sMonsters)));
        pItemNode->LinkEndChild(createTextNode("Features", UnicodeToUtf8(oItem.sFeatures)));
    }
    auto sConfig = getFullPath(L"MonsterBookHistory.log");
    oDocument.SaveFile(UnicodeToUtf8(sConfig));
}

void saveResultList(const std::wstring& sFileName, CResult& oResult)
{
    std::ofstream out;
    out.open(ToString(sFileName), std::ios::out);
    if (out)
    {
        std::cout << "save" << std::endl;
        out << "名称";
        for (int i = 0; i < EF_ALL; i++)
        {
            out << "," << ToString(g_sFeatureNames[i]);
        }
        out << std::endl;

        for each (auto oItem in oResult.getTeamList())
        {
            std::wstring sName;
            for (auto itr = oItem.pTeam->begin(); itr != oItem.pTeam->end(); ++itr)
            {
                sName += (*itr)->getName() + L"|";
            }
            out << ToString(sName);

            for (int i = 0; i < EF_ALL; i++)
            {
                out << "," << oItem.dFeatures[i];
            }
            out << std::endl;
        }
        out.close();
    }
}

void LoadResultList(const std::wstring& sFileName, CResult& oResult, CGame* pGame)
{
    std::ifstream in;
    in.open(ToString(sFileName), std::ios::in);
    if (in)
    {
        std::string sLine;
        std::getline(in, sLine); // 跳过标题
        while (!in.eof())
        {
            std::getline(in, sLine);
            std::vector<std::wstring> oList;
            split(ToUnicode(sLine), L',', oList);
            if (oList.size() == EF_ALL + 1)
            {
                auto sNames = oList[0];
                std::vector<std::wstring> oNameList;
                split(sNames, L'|', oNameList);
                CTeamPtr pTeam(new CTeam());
                bool bError = false;
                for each (auto sName in oNameList)
                {
                    if (sName.empty())
                        continue;
                    auto itr = std::find_if(pGame->m_oMonsterList.begin(), pGame->m_oMonsterList.end(), [sName](CMonster* pMonster) {
                        return pMonster->getName() == sName;
                    });
                    if (itr == pGame->m_oMonsterList.end())
                    {
                        bError = true;
                        break;
                    }
                    pTeam->push_back(*itr);
                }
                if (bError)
                    continue;

                CResultItem oItem;
                for (int i = 0; i < EF_ALL; i++)
                {
                    oItem.dFeatures[i] = std::stod(oList[i + 1]);
                }
                oItem.dTotalFeature = 0;
                oItem.dTotalNeedFeature = 0;
                oItem.nFitCount = 0;
                oItem.dClosedFeature = 0;
                oItem.dMaxClosedFeature = 0;
                oItem.dFactor = 0;
                oItem.pTeam = pTeam;

                oResult.getTeamList().push_back(oItem);
            }
        }

        in.close();
    }
}

void logHistory(CChallenge* pChallenge, bool bSuccess, CResult& oResult)
{
    //if (isSetChallenge(oParamsMap))
    if (pChallenge)
    {
        std::vector<CHistoryItem> oHistoryList;
        getHistroyList(oHistoryList);
        auto oNew = makeHistoryItem(pChallenge, bSuccess, oResult);
        auto itr = std::find_if(oHistoryList.begin(), oHistoryList.end(), [oNew](CHistoryItem& oItem) {
            return oItem.sChallenge == oNew.sChallenge; });
        if (itr != oHistoryList.end())
            oHistoryList.erase(itr);
        oHistoryList.insert(oHistoryList.begin(), oNew);
        if (oHistoryList.size() > 20)
        {
            oHistoryList.resize(20);
        }
        saveHistoryList(oHistoryList);
    }
}

int main(int argc, char* argv[])
{
	std::map<std::wstring, std::wstring> oParamsMap;
    while (true)
    {
        std::string s;
        std::cout << ">";
        std::cin >> s;
        if (s.empty())
            continue;
        std::wstring str = ToUnicode(s);
        //std::transform(str.begin(), str.end(), str.begin(), ::tolower);

        CGame oGame;
        if (oParamsMap.find(L"disable") != oParamsMap.end())
        {
            std::vector<std::wstring> oStringList;
            split(oParamsMap[L"disable"], L',', oStringList);
            oGame.disable_exclude_group(oStringList);
        }

        CConfig::init(&oGame);

		if (str == L"simulator")
        {
            auto sNames = oParamsMap[L"team"];
            std::vector<std::wstring> oMonsterList;
            split(sNames, L',', oMonsterList);
            int oResult[EF_ALL];

            oGame.simulator(oMonsterList, oResult);
            for (int i = 0; i < EF_ALL; i++)
            {
                std::cout << oResult[i] << ",";
            }
            std::cout << std::endl;
        }
        else if (str == L"play")
        {
            CChallenge oChallenge;
            initChallenge(oParamsMap, oChallenge);

            CResult oResultList;
            initResult(oParamsMap, oResultList);
            CResult oFailedResultList;
            //oFailedResultList.changeOrder(RO_CLOSE, MB_DESC, 0);
            //oFailedResultList.setTop(oParamsMap.find(L"top") == oParamsMap.end() ? 10 : std::stoi(oParamsMap[L"top"]));
            oResultList.setTop(1);
            oFailedResultList.setTop(1);
            if (oParamsMap.find(L"min_max_close") == oParamsMap.end())
            {
                oFailedResultList.changeOrder(RO_FIT_FEATURE_COUNT, MB_DESC, 0);
                oFailedResultList.changeOrder(RO_CLOSE_FEATURE, MB_ASC, 1);
            }
            else
                oFailedResultList.changeOrder(RO_MAX_CLOSE_FEATURE, MB_ASC, 0);
            if (oParamsMap.find(L"exclude") != oParamsMap.end())
            {
                std::vector<std::wstring> oStringList;
                split(oParamsMap[L"exclude"], L',', oStringList);
                oGame.exclude(oStringList);
            }
            oGame.exclude(oGame.m_oExcludeMonsters);
            oGame.m_bExportFailedInfo = oParamsMap.find(L"export") != oParamsMap.end() && (oParamsMap[L"export"] == L"on");
            auto nTime = GetTickCount();
            oGame.play(&oChallenge, oResultList, oFailedResultList);
            std::cout << "count: " << oGame.m_nCount << std::endl;
            std::cout << "play: " << GetTickCount() - nTime << std::endl;

            if (oResultList.size() > 0)
            {
                output(oResultList);
            }
            else
            {
                std::cout << "<--------- no match result !!! --------->" << std::endl;
                auto dRequired = oChallenge.featuresRequired();
                for (int i = 0; i < EF_ALL; i++)
                {
                    if (dRequired[i] > g_dEpsilon)
                    {
                        std::cout << dRequired[i] << "(" << ToString(g_sFeatureNames[i]) << "),";
                    }
                }
                std::cout << std::endl;
                output(oFailedResultList);
            }
            logHistory(&oChallenge, oResultList.size() > 0, (oResultList.size() > 0) ? oResultList : oFailedResultList);
        }
        else if (str == L"play_fast")
        {
            CChallenge oChallenge;
            initChallenge(oParamsMap, oChallenge);

            CResult oResultList;
            initResult(oParamsMap, oResultList);
            CResult oFailedResultList;
            //oFailedResultList.changeOrder(RO_CLOSE, MB_DESC, 0);
            //oFailedResultList.setTop(oParamsMap.find(L"top") == oParamsMap.end() ? 10 : std::stoi(oParamsMap[L"top"]));
            oResultList.setTop(1);
            oFailedResultList.setTop(1);
            if (oParamsMap.find(L"min_max_close") == oParamsMap.end())
            {
                oFailedResultList.changeOrder(RO_FIT_FEATURE_COUNT, MB_DESC, 0);
                oFailedResultList.changeOrder(RO_CLOSE_FEATURE, MB_ASC, 1);
            }
            else
                oFailedResultList.changeOrder(RO_MAX_CLOSE_FEATURE, MB_ASC, 0);
            /*
            if (oParamsMap.find(L"exclude") != oParamsMap.end())
            {
                std::vector<std::wstring> oStringList;
                split(oParamsMap[L"exclude"], L',', oStringList);
                oGame.exclude(oStringList);
            }
            oGame.exclude(oGame.m_oExcludeMonsters);
            */
            oGame.m_bExportFailedInfo = oParamsMap.find(L"export") != oParamsMap.end() && (oParamsMap[L"export"] == L"on");
            auto nTime = GetTickCount();
            CResult oList;
            if (oChallenge.requiredClass() != EC_ALL)
            {
                LoadResultList(getFullPath(g_sClassNames[oChallenge.requiredClass()] + L".csv"), oList, &oGame);
            }
            for each (auto oItem in oList.getTeamList())
            {
                if (CGame::success(oChallenge.featuresRequired(), oItem.dFeatures))
                    oResultList.add(oItem.pTeam, oItem.dFeatures, nullptr);
                else
                {
                    if (oResultList.size() == 0 && oGame.m_bExportFailedInfo)
                        oFailedResultList.add(oItem.pTeam, oItem.dFeatures, oChallenge.featuresRequired());
                }
            }
            std::cout << "play: " << GetTickCount() - nTime << std::endl;

            if (oResultList.size() > 0)
            {
                output(oResultList);
            }
            else
            {
                std::cout << "<--------- no match result !!! --------->" << std::endl;
                auto dRequired = oChallenge.featuresRequired();
                for (int i = 0; i < EF_ALL; i++)
                {
                    if (dRequired[i] > g_dEpsilon)
                    {
                        std::cout << dRequired[i] << "(" << ToString(g_sFeatureNames[i]) << "),";
                    }
                }
                std::cout << std::endl;
                output(oFailedResultList);
            }
            logHistory(&oChallenge, oResultList.size() > 0, (oResultList.size() > 0) ? oResultList : oFailedResultList);
        }
        else if (str == L"calc")
        {
            for (int i = 0; i < EC_ALL; i++)
            {
                CChallenge oChallenge;
                oChallenge.m_sName = g_sClassNames[i];
                oChallenge.m_nClass = (EnElementClass)i;
                oChallenge.m_bCalcFlag = true;
                oChallenge.m_nTotal = 1024;
                for (int i = 0; i < EF_ALL; i++)
                {
                    oChallenge.m_nRequired[i] = 1.0;
                }
                CResult oResultList;
                CResult oFailedResultList;
                auto nTime = GetTickCount();
                oGame.play(&oChallenge, oResultList, oFailedResultList);
                std::cout << "result_count: " << oResultList.size() << std::endl;
                saveResultList(getFullPath(g_sClassNames[i] + L".csv"), oResultList);
                std::cout << "play: " << GetTickCount() - nTime << std::endl;
            }
        }
        else if (str == L"settings")
		{
			for each (auto oPair in oParamsMap)
			{
				std::cout << ToString(oPair.first) << "\t" << ToString(oPair.second) << std::endl;
			}
		}
        else if (str == L"clear")
        {
            oParamsMap.clear();
        }
        else if (str == L"help")
        {
            std::cout << "Command:" << std::endl;
            std::cout << "\tsimulator" << std::endl;
            std::cout << "\tplay" << std::endl;
            std::cout << "\tsettings" << std::endl;
            std::cout << "\tclear" << std::endl;
            std::cout << "\thelp" << std::endl;
            std::cout << "\thistory" << std::endl;
            std::cout << "\tlist" << std::endl;
            std::cout << "\tplayall" << std::endl;
            std::cout << "ClassNames:" << std::endl;
            std::cout << "\t木,地,水,火,光,暗" << std::endl;
            std::cout << "FeatureShortNames:" << std::endl;
            std::cout << "\tpower,int,speed,charm,lucky,tech" << std::endl;
        }
        else if (str == L"history")
        {
            std::vector<CHistoryItem> oList;
            getHistroyList(oList);
            for each (auto oItem in oList)
            {
                std::cout << ToString(oItem.sChallenge) << std::endl;
            }
        }
        else if (str == L"history_result")
        {
            std::vector<CHistoryItem> oList;
            getHistroyList(oList);
            for each (auto oItem in oList)
            {
                std::cout << ToString(oItem.sChallenge) << std::endl;
                std::cout << "\t" << ToString(oItem.sMonsters) << std::endl;
                std::cout << "\t" << (oItem.bSuccess ? "Succ:" : "Failed:") << ToString(oItem.sFeatures) << std::endl;
            }
        }
        else if (str == L"list")
        {
            for each (auto pMonster in oGame.m_oMonsterList)
            {
                std::cout << ToString(pMonster->getName()) << "\t" << (int)pMonster->getSpeciality() << std::endl;
            }
        }
        else if (str == L"playall")
        {
            if (oParamsMap.find(L"exclude") != oParamsMap.end())
            {
                std::vector<std::wstring> oStringList;
                split(oParamsMap[L"exclude"], L',', oStringList);
                oGame.exclude(oStringList);
            }
            oGame.exclude(oGame.m_oExcludeMonsters);
            auto nTime = GetTickCount();
            std::vector<CSolutionPtr> oResultList;
            std::vector<int> oGroupList;
            getGroupList(oParamsMap, oGroupList);
            std::map<std::wstring, CResult> oFailedResultList;
			oGame.m_bExportFailedInfo = oParamsMap.find(L"export") != oParamsMap.end() && (oParamsMap[L"export"] == L"on");
            oGame.play(oResultList, oGroupList, oFailedResultList);
            std::wcout << L"play: " << GetTickCount() - nTime << std::endl;
            std::sort(oResultList.begin(), oResultList.end(), [](CSolutionPtr pSolution1, CSolutionPtr pSolution2) {
                return pSolution1->size() > pSolution2->size();
            });
            output(oResultList);
            if (oFailedResultList.size() > 0)
            {
                std::cout << "<--------- failed match --------->" << std::endl;
                for each (auto oPair in oFailedResultList)
                {
                    std::wstring sName = oPair.first;
                    auto itr = std::find_if(oGame.m_oChallengeList.begin(), oGame.m_oChallengeList.end(), [sName](CChallenge* pChallenge) {
                        return pChallenge->getName() == sName;
                    });
                    std::cout << ToString(sName) << ":";
                    if (itr != oGame.m_oChallengeList.end())
                    {
                        auto dRequired = (*itr)->featuresRequired();
                        for (int i = 0; i < EF_ALL; i++)
                        {
                            if (dRequired[i] > g_dEpsilon)
                            {
                                std::cout << dRequired[i] << "(" << ToString(g_sFeatureNames[i]) << "),";
                            }
                        }
                    }
                    std::cout << std::endl;
                    output(oPair.second);
                }
            }
        }
		else
		{
			parseParams(str, oParamsMap);
		}
    }

    return 0;
}