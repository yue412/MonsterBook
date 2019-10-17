#include "Scanner.h"
#include "Parser.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    //std::string sCode = "-1+1*2-3*-6+4\0";
    if (argc > 1)
    {
        Scanner oScanner((const unsigned char*)argv[1], strlen(argv[1]));
        Parser oParser(&oScanner);
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