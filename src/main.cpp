#include <string>
#include <iostream>
#include <algorithm>
#include "Game.h"
#include "Config.h"
#include "Challenge.h"
#include "Monster.h"
#include <Windows.h>

void output(std::vector<CSolutionPtr>& oResultList)
{
    for (auto itr = oResultList.begin(); itr != oResultList.end(); itr++)
    {
        auto nIndex = itr - oResultList.begin() + 1;
		if (nIndex > 10)
		{
			break;
		}
        std::wcout << L"Solution " << nIndex << L":" << std::endl;
        for each (auto pair in **itr)
        {
            std::wcout << pair.first->getName() << L"[";
            for each (auto pMonster in *pair.second)
            {
                std::wcout << pMonster->getName() << L",";
            }
            std::wcout << L"]" << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
	auto nTime = GetTickCount();
    CGame oGame;
    CConfig::init(&oGame);
    std::vector<CSolutionPtr> oResultList;
    oGame.play(oResultList);
	std::wcout << L"play: " << GetTickCount() - nTime << std::endl;
	std::sort(oResultList.begin(), oResultList.end(), [](CSolutionPtr pSolution1, CSolutionPtr pSolution2){
		return pSolution1->size() > pSolution2->size();
	});
    output(oResultList);

    return 0;
}