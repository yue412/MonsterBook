#ifndef BIGINT_H
#define BIGINT_H

class CBigInt
{
public:
    CBigInt();
    ~CBigInt();
    void setBit(int nPos, char nVal);
    CBigInt operator|(const CBigInt& nVal);
    CBigInt operator&(const CBigInt& nVal);
    void operator|=(const CBigInt& nVal);
    bool operator==(const CBigInt& nVal);
private:
    void init();
    __int64 m_nIds[8];
};

#endif // !MONSTERID_H