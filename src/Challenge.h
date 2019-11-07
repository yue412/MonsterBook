#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <string>
#include "MonsterTypes.h"

class CChallenge
{
	friend class CConfig;
public:
    CChallenge();
    ~CChallenge();
public:
    std::wstring getName() { return m_sName; }
	int getMin() { return m_nMin; }
	int getMax() { return m_nMax; }
	int* featuresRequired() { return m_nRequired; }
private:
    std::wstring m_sName;
	int m_nMin;
	int m_nMax;
	int m_nRequired[c_nFeatureCount];
};

#endif // !CHALLENGE_H
