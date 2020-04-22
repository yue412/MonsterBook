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

void output(const CResult& oResult)
{
    for each (auto oItem in oResult.m_oTeamList)
    {
        std::cout << oItem.dTotalFeature << "\t";
        std::cout << oItem.dTotalNeedFeature << "\t";
        for each (auto pMonster in *oItem.pTeam)
        {
            std::cout << ToString(pMonster->getName()) << "[" << ToString(g_sClassNames[pMonster->getClass()]) << "],";
        }
        //std::cout << std::endl;
        std::cout << "\t";
        for (int i = 0; i < EF_ALL; i++)
        {
            std::cout << roundEx(oItem.dFeatures[i]) << "("<< ToString(g_sFeatureNames[i]) <<"),";
        }
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
	oChallenge.m_nMin = oParamsMap.find(L"min") == oParamsMap.end() ? 0 : std::stoi(oParamsMap[L"min"]);
	oChallenge.m_nMax = oParamsMap.find(L"max") == oParamsMap.end() ? 8 : std::stoi(oParamsMap[L"max"]);
    oChallenge.m_nClass = oParamsMap.find(L"class") == oParamsMap.end() ? EC_ALL : Name2Class(oParamsMap[L"class"]);
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

bool isSetChallenge(std::map<std::wstring, std::wstring>& oParamsMap)
{
    bool bResult = false;
    for (int i = 0; i < EF_ALL; i++)
    {
        if (oParamsMap.find(g_sFeatureShortNames[i]) != oParamsMap.end())
        {
            bResult = true;
            break;
        }
    }
    return bResult;
}

void getHistroyList(std::vector<std::wstring>& oStringList)
{
    std::ifstream in;
    in.open(UnicodeToUtf8(getFullPath(L"MonsterBook.log")), std::ios::in);
    if (in.is_open())
    {
        while (!in.eof())
        {
            std::string str;
            std::getline(in, str);
            if (!str.empty())
            {
                oStringList.push_back(Utf8ToUnicode(str));
            }
        }
    }
}

std::wstring getParamStr(std::map<std::wstring, std::wstring>& oParamsMap)
{
    std::wstring sResult;
    for each (auto oPair in oParamsMap)
    {
        sResult += oPair.first + L"=" + oPair.second + L" ";
    }
    return sResult;
}

void saveHistoryList(std::vector<std::wstring>& oList)
{
    std::ofstream out;
    out.open(UnicodeToUtf8(getFullPath(L"MonsterBook.log")), std::ios::out);
    if (out.is_open())
    {
        for each (auto str in oList)
        {
            out << UnicodeToUtf8(str) << std::endl;
        }
        out.close();
    }
}

void logHistory(std::map<std::wstring, std::wstring>& oParamsMap)
{
    if (isSetChallenge(oParamsMap))
    {
        std::vector<std::wstring> oStringList;
        getHistroyList(oStringList);
        auto sParams = getParamStr(oParamsMap);
        auto itr = std::find(oStringList.begin(), oStringList.end(), sParams);
        if (itr == oStringList.end())
        {
            oStringList.insert(oStringList.begin(), sParams);
            if (oStringList.size() > 20)
            {
                oStringList.resize(20);
            }
            saveHistoryList(oStringList);
        }
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
            logHistory(oParamsMap);
            CChallenge oChallenge;
			initChallenge(oParamsMap, oChallenge);

            CResult oResultList;
			initResult(oParamsMap, oResultList);
            CResult oFailedResultList;
            //oFailedResultList.changeOrder(RO_CLOSE, MB_DESC, 0);
            //oFailedResultList.setTop(oParamsMap.find(L"top") == oParamsMap.end() ? 10 : std::stoi(oParamsMap[L"top"]));
            oResultList.setTop(1);
            oFailedResultList.setTop(1);
            oFailedResultList.changeOrder(RO_FIT_FEATURE_COUNT, MB_DESC, 0);
            oFailedResultList.changeOrder(RO_CLOSE_FEATURE, MB_ASC, 1);
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
            std::cout << "\tÄ¾,µØ,Ë®,»ð,¹â,°µ" << std::endl;
            std::cout << "FeatureShortNames:" << std::endl;
            std::cout << "\tpower,int,speed,charm,lucky,tech" << std::endl;
        }
        else if (str == L"history")
        {
            std::vector<std::wstring> oList;
            getHistroyList(oList);
            for each (auto str in oList)
            {
                std::cout << ToString(str) << std::endl;
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