#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <string>

class CChallenge
{
public:
    CChallenge();
    ~CChallenge();
public:
    std::wstring getName() { return m_sName; }
    int getMin();
    int getMax();
    int* featuresRequired();
private:
    std::wstring m_sName;
};

#endif // !CHALLENGE_H
