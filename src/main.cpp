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
  //  for (auto itr = oResultList.begin(); itr != oResultList.end(); itr++)
  //  {
  //      auto nIndex = itr - oResultList.begin() + 1;
		//if (nIndex > 10)
		//{
		//	break;
		//}
  //      std::wcout << L"Solution " << nIndex << L":" << std::endl;
  //      for each (auto pair in **itr)
  //      {
  //          std::wcout << pair.first->getName() << L"[";
  //          for each (auto pMonster in *pair.second)
  //          {
  //              std::wcout << pMonster->getName() << L",";
  //          }
  //          std::wcout << L"]" << std::endl;
  //      }
  //  }
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
        std::cout << std::endl;
        std::cout << "\t\t";
        for (int i = 0; i < EF_ALL; i++)
        {
            std::cout << (int)ceil(oItem.dFeatures[i]) << ",";
        }
        std::cout << std::endl;
    }
}

void parseParams(const std::wstring& sParams, std::map<std::wstring, std::wstring>& oParamsMap)
{
    std::vector<std::wstring> oParamsList;
    split(sParams, L';', oParamsList);
    for each (auto sParam in oParamsList)
    {
        std::vector<std::wstring> oList;
        split(sParam, L'=', oList);
        switch (oList.size())
        {
        case 0:
            break;
        case 1:
            oParamsMap.insert(std::make_pair(L"", oList[0]));
            break;
        default:
            oParamsMap.insert(std::make_pair(oList[0], oList[1]));
        }
    }
}

bool parse(const std::wstring& sLine, std::wstring& sCommand, std::wstring& sParams)
{
    sCommand = L"";
    sParams = L"";
    std::vector<std::wstring> oStringList;
    split(sLine, L':', oStringList);
    if (oStringList.size() == 0)
        return false;
    sCommand = oStringList[0];
    if (oStringList.size() == 1)
        return true;
    sParams = oStringList[1];
    return true;
}

int main(int argc, char* argv[])
{
    while (true)
    {
        std::string s;
        std::cin >> s;
        if (s.empty())
            continue;
        std::wstring str = ToUnicode(s);
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        std::wstring sCmd, sParams;
        if (!parse(str, sCmd, sParams))
        {
            std::cout << "parse error!" << std::endl;
            continue;
        }
        std::map<std::wstring, std::wstring> oParamsMap;
        parseParams(sParams, oParamsMap);
        CGame oGame;
        CConfig::init(&oGame);
        if (sCmd == L"team")
        {
            auto sNames = oParamsMap[L""];
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
        else if (sCmd == L"challenge")
        {
            CChallenge oChallenge;
            oChallenge.m_nMin = oParamsMap.find(L"min") == oParamsMap.end() ? 0 : std::stoi(oParamsMap[L"min"]);
            oChallenge.m_nMax = oParamsMap.find(L"max") == oParamsMap.end() ? 8 : std::stoi(oParamsMap[L"max"]);
            for (int i = 0; i < EF_ALL; i++)
            {
                oChallenge.m_nRequired[i] = oParamsMap.find(g_sFeatureNames[i]) == oParamsMap.end() ? 0 : std::stoi(oParamsMap[g_sFeatureNames[i]]);
            }
            auto nTime = GetTickCount();
            CResult oResultList;
            oGame.play(&oChallenge, oResultList);
            std::cout << "count: " << oGame.m_nCount << std::endl;
            std::cout << "play: " << GetTickCount() - nTime << std::endl;
            output(oResultList);
        }
        else
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

    }

    return 0;
}