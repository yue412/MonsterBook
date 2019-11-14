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

void output(std::vector<CSolutionPtr>& oResultList)
{
    for (auto itr = oResultList.begin(); itr != oResultList.end(); itr++)
    {
        auto nIndex = itr - oResultList.begin() + 1;
		if (nIndex > 10)
		{
			break;
		}
        std::cout << "Solution " << nIndex << ":" << std::endl;
        for each (auto pair in **itr)
        {
            std::cout << ToString(pair.first->getName()) << "[";
            for each (auto pMonster in *pair.second)
            {
                std::cout << ToString(pMonster->getName()) << ",";
            }
            std::cout << "]" << std::endl;
        }
    }
}

void output(const CResult& oResult)
{
    for each (auto oItem in oResult.m_oTeamList)
    {
        std::cout << oItem.dTotalFeature << "\t";
        std::cout << oItem.dTotalNeedFeature << "\t";
        for each (auto pMonster in *oItem.pTeam)
        {
            std::cout << ToString(pMonster->getName()) << ",";
        }
        //std::cout << std::endl;
        std::cout << "\t";
        for (int i = 0; i < EF_ALL; i++)
        {
            std::cout << (int)ceil(oItem.dFeatures[i]) << ",";
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
	for (int i = 0; i < EF_ALL; i++)
	{
		oChallenge.m_nRequired[i] = oParamsMap.find(g_sFeatureShortNames[i]) == oParamsMap.end() ? 0 : std::stoi(oParamsMap[g_sFeatureShortNames[i]]);
	}
}

int main(int argc, char* argv[])
{
	std::map<std::wstring, std::wstring> oParamsMap;
    while (true)
    {
        std::string s;
        std::cin >> s;
        if (s.empty())
            continue;
        std::wstring str = ToUnicode(s);
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);

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
            CChallenge oChallenge;
			initChallenge(oParamsMap, oChallenge);

            CResult oResultList;
			initResult(oParamsMap, oResultList);

			if (oParamsMap.find(L"exclude") != oParamsMap.end())
			{
				std::vector<std::wstring> oStringList;
				split(oParamsMap[L"exclude"], L',', oStringList);
				oGame.exclude(oStringList);
			}
			auto nTime = GetTickCount();
			oGame.play(&oChallenge, oResultList);
            std::cout << "count: " << oGame.m_nCount << std::endl;
            std::cout << "play: " << GetTickCount() - nTime << std::endl;

            output(oResultList);
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
        else if (str == L"playall")
        {
            auto nTime = GetTickCount();
            std::vector<CSolutionPtr> oResultList;
            oGame.play(oResultList);
            std::wcout << L"play: " << GetTickCount() - nTime << std::endl;
            std::sort(oResultList.begin(), oResultList.end(), [](CSolutionPtr pSolution1, CSolutionPtr pSolution2) {
                return pSolution1->size() > pSolution2->size();
            });
            output(oResultList);
        }
		else
		{
			parseParams(str, oParamsMap);
		}
    }

    return 0;
}