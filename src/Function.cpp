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

FunctionFactoryTable::~FunctionFactoryTable()
{
    clear();
}

void FunctionFactoryTable::add(const std::string & sName, FunctionFactory * pFactory)
{
    if (!find(sName))
    {
        m_oFactoryMap.insert(std::make_pair(sName, pFactory));
    }
}

FunctionFactory * FunctionFactoryTable::find(const std::string & sName)
{
    auto itr = m_oFactoryMap.find(sName);
    if (itr != m_oFactoryMap.end())
    {
        return itr->second;
    }
    else
    {
        return nullptr;
    }
}

void FunctionFactoryTable::clear()
{
    for each (auto p in m_oFactoryMap)
    {
        delete p.second;
    }
    m_oFactoryMap.clear();
}
