#include "Function.h"



Function::Function()
{
}


Function::~Function()
{
}

void Function::addParam(double dValue)
{
    m_oParams.push_back(dValue);
}

double Function::params(int nIndex)
{
    return m_oParams[nIndex];
}

int Function::paramCount()
{
    return (int)m_oParams.size();
}
