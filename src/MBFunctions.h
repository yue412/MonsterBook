#ifndef MBFUNCTION_H
#define MBFUNCTION_H

#include "Function.h"

class MBTest : public Function
{
public:
    virtual double execute();
};

class MBTest1 : public Function
{
public:
    virtual double execute();
};

class MBTest2 : public Function
{
public:
    virtual double execute();
};

G_DEC_FACTORY(MBTest)
G_DEC_FACTORY(MBTest1)
G_DEC_FACTORY(MBTest2)

#endif // !MBFUNCTION_H

