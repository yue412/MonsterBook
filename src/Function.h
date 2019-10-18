#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>

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

#endif // !FUNCTION_H