

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__

#include <string>
#include <map>
#include "Symbol.h"


#include "Scanner.h"



class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_ident=1,
		_number=2
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

double m_dResult;
std::map<std::string, Symbol*> m_oSymbolTable;

Symbol* addSymbol(const std::string& name, EnDataType type, EnSymbolKind kind, void* adr)
{
	if(findSymbol(name) == nullptr)
	{
		Symbol* pSymbol = new Symbol();
		pSymbol->name = name;
		pSymbol->type = type;
		pSymbol->kind = kind;
		pSymbol->adr = adr;
		m_oSymbolTable.insert(std::make_pair(name, pSymbol));
		return pSymbol;
	}
	else
		return nullptr;
}

Symbol* findSymbol(const std::string& name)
{
	auto itr = m_oSymbolTable.find(name);
	if(itr == m_oSymbolTable.end())
		return nullptr;
	else
		return itr->second;

}



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void MonsterBook();
	void Expr(double& dVal);
	void Term(double& dVal);
	void Factor(double& dVal);

	void Parse();

}; // end Parser



#endif

