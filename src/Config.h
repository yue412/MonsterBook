#ifndef CONFIG_H
#define CONFIG_H

#include "MonsterTypes.h"

class CGame;
class CSkill;
class TiXmlElement;
class CMonster;

class CConfig
{
public:
    static void init(CGame* pGame);
private:
	static CSkill* createSkill(std::wstring& sName, TiXmlElement* pSkillNode);
	static void initFeatures(TiXmlElement* pNode, const std::string& sName, double* dFeatures);
    static void initSkills(TiXmlElement* pNode, std::vector<CSkill*>& oSkills, CMonster* pOwner = nullptr);
};

#endif // !CONFIG_H