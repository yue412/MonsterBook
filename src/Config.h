#ifndef CONFIG_H
#define CONFIG_H

#include "MonsterTypes.h"

class CGame;
class CSkill;
class TiXmlElement;

class CConfig
{
public:
    static void init(CGame* pGame);
private:
	static CSkill* createSkill(std::wstring& sName, TiXmlElement* pSkillNode);
	static void initFeatures(TiXmlElement* pNode, const std::string& sName, double* dFeatures);
    static void initSkills(TiXmlElement* pNode, std::vector<CSkill*>& oSkills);
};

#endif // !CONFIG_H