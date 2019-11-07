#include <string>
#include <iostream>
#include "armadillo"
#include "Game.h"
#include "Config.h"
#include "Challenge.h"
#include "Monster.h"

void output(std::vector<CSolution*>& oResultList)
{
    for (auto itr = oResultList.begin(); itr != oResultList.end(); itr++)
    {
        auto nIndex = itr - oResultList.begin() + 1;
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
    CGame oGame;
    CConfig::init(&oGame);
    std::vector<CSolution*> oResultList;
    oGame.play(oResultList);
    output(oResultList);
    //arma::vec vec;
    //arma::mat mat;
    //std::cout << mat*vec <<std::endl;
    //v.print();
    //arma::vec vec = "1,0,0";
    //arma::mat mat = "1,0,0,0,1,0,0,0,1";
    //arma::mat v = mat*vec;
    //v.print();
    return 0;
}