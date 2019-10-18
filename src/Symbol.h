#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

enum EnDataType
{
    dtUnknow,
    dtDouble
};

enum EnSymbolKind
{
    skVar,
    skFunc
};

class Symbol
{
public:
    std::string name;
    EnDataType type;
    EnSymbolKind kind;
    void* adr;
public:
    Symbol()
    {
        type = dtUnknow;
        kind = skVar;
        adr = nullptr;
    }
    virtual ~Symbol();
};

#endif