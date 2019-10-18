#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <map>
#include <string>

class Function
{
public:
    Function();
    virtual ~Function();
    void addParam(double dValue);
    double params(int nIndex);
    int paramCount();
    virtual double execute() = 0;
private:
    std::vector<double> m_oParams;
};

class FunctionFactory
{
public:
    virtual ~FunctionFactory() {}
    virtual Function* create() = 0;
};

class FunctionFactoryTable
{
public:
    ~FunctionFactoryTable();
public:
    void add(const std::string& sName, FunctionFactory* pFactory);
    FunctionFactory* find(const std::string& sName);
private:
    void clear();
    std::map<std::string, FunctionFactory*> m_oFactoryMap;
};

#define G_DEC_FACTORY(className) \
class className##Factory : public FunctionFactory \
{ \
public: \
    virtual Function* create(); \
};

#define G_IMP_FACTORY(className) \
Function * className##Factory::create() \
{ \
    return new className; \
}

#endif // !FUNCTION_H