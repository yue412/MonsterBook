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
    int getTotal() { return m_nTotal; }
	double* featuresRequired() { return m_nRequired; }
    EnElementClass requiredClass() { return m_nClass; }
    bool enableSkill() { return m_bEnableSkill; }
    std::wstring requiredCharacter() { return  m_sCharacter; }
private:
    std::wstring m_sName;
	int m_nMin;
	int m_nMax;
    int m_nTotal;
    double m_nRequired[EF_ALL];
    bool m_bEnableSkill;
    EnElementClass m_nClass;
    std::wstring m_sCharacter;
};

#endif // !CHALLENGE_H
