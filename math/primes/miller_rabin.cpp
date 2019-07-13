#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

long long safemul(long long a, long long n, long long modn)
{
    long long result = 0;
    for (;n;n>>=1)
    {
        if (n&1)
            result = (result + a) % modn;
        a = (a+a) % modn;
    }
    return result;
}

long long fastpow(long long base, long long power, long long modn)
{
    long long result = 1;
    for(;power;power>>=1)
    {
        if (power & 1LL)
            result = safemul(result, base, modn);
        base = safemul(base, base, modn);
    }
    return result;
}

bool miller_rabin_sr(long long n, long long checker)
{
    // Miller-Rabin check, single round
    long long tmp = n-1;
    int j = 0;
    while (tmp & 1 == 0) // tmp%2 == 0
    {
        j++;
        tmp >>= 1;
    }
    // Ferman's little theorem
    long long x = fastpow(a, tmp, n);
    // TODO: see https://www.luogu.org/problemnew/solution/P3383
    return false;
}
