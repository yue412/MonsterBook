#include "Scanner.h"
#include "Parser.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    //std::string sCode = "-1+1*2-3*-6+4\0";
    while (true)
    {
        std::string str;
        std::getline(std::cin, str);
        Scanner oScanner((const unsigned char*)str.c_str(), str.length());
        Parser oParser(&oScanner);
        double a = 4.0;
        oParser.addSymbol("a", dtDouble, skVar, &a);
        oParser.Parse();
        if (oParser.errors->count == 0)
        {
            std::cout << "Result: " << oParser.m_dResult << std::endl;
        }
        else
        {
            std::cout << oParser.errors->count << " errors detected" << std::endl;
        }
    }
    return 0;
}