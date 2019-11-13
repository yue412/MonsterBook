#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <string>
#include <map>
#include "MonsterTypes.h"

class CChallenge
{
	friend class CConfig;
	friend void initChallenge(std::map<std::wstring, std::wstring>& oParamsMap, CChallenge& oChallenge);
public:
    CChallenge();
    ~CChallenge();
public:
    std::wstring getName() { return m_sName; }
	int getMin() { return m_nMin; }
	int getMax() { return m_nMax; }
	double* featuresRequired() { return m_nRequired; }
private:
    std::wstring m_sName;
	int m_nMin;
	int m_nMax;
    double m_nRequired[EF_ALL];
};

#endif // !CHALLENGE_H
