#include "Scanner.h"
#include "Parser.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    std::string sCode = "1+1\0";
    Scanner oScanner((const unsigned char*)sCode.c_str(), sCode.size());
    Parser oParser(&oScanner);
    oParser.Parse();
    std::cout << oParser.errors->count << " errors detected" << std::endl;
    return 0;
}