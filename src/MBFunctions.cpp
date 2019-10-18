#include "MBFunctions.h"

G_IMP_FACTORY(MBTest)
G_IMP_FACTORY(MBTest1)
G_IMP_FACTORY(MBTest2)

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

