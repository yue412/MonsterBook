#include "MBFunctions.h"

double MBTest::execute()
{
    return 12345;
}

double MBTest1::execute()
{
    return params(0)*params(0);
}

double MBTest2::execute()
{
    return params(0)*params(1);
}
