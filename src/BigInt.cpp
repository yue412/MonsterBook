#include "BigInt.h"
#include <memory>
#include <assert.h>

CBigInt::CBigInt()
{
    init();
}

CBigInt::~CBigInt()
{
}

void CBigInt::setBit(int nPos, char nVal)
{
    auto m = nPos / 64;
    auto n = nPos % 64;
    assert(m < 8);
    m_nIds[m] = nVal == 0 ? m_nIds[m] & (-1 ^ 1i64 << n) : m_nIds[m] | (1i64 << n);
}

CBigInt CBigInt::operator|(const CBigInt & nVal)
{
    CBigInt nResult = *this;
    nResult |= nVal;
    return nResult;
}

CBigInt CBigInt::operator&(const CBigInt & nVal)
{
    CBigInt nResult;
    for (int i = 0; i < 8; i++)
    {
        nResult.m_nIds[i] = m_nIds[i] & nVal.m_nIds[i];
    }
    return nResult;
}

void CBigInt::operator|=(const CBigInt & nVal)
{
    for (int i = 0; i < 8; i++)
    {
        m_nIds[i] = m_nIds[i] | nVal.m_nIds[i];
    }

}

bool CBigInt::operator==(const CBigInt & nVal)
{
    for (int i = 0; i < 8; i++)
    {
        if (m_nIds[i] != nVal.m_nIds[i])
            return false;
    }
    return true;
}

void CBigInt::init()
{
    std::memset(m_nIds, 0, sizeof(__int64) * 8);
}