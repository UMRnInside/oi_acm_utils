#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

std::vector<long long> checkers = {2, 3, 5, 7, 11, 13, 17, 19, 23, 19260817};

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
    long long tmp = n-1; // That is u !
    int j = 0;
    while (tmp % 2 == 0) // tmp%2 == 0
    {
        j++;
        tmp /= 2;
    }
    // n-1 => pow(2,j) * tmp
    
    // Ferman's little theorem
    // a^(p-1) === 1(mod p), p-1 == a^(u*2^j)
    // so a^(u*2^j) === 1 (mod p)
    // -> a^(u*2^(j-1)) * a^(u*2^(j-1)) === 1(mod p)
    long long x = fastpow(checker, tmp, n);
    if (x==1 || x==n-1) return true;
    while (j--)
    {
        x = safemul(x, x, n);
        if (x == n-1) return true;
    }
    // see https://www.luogu.org/problemnew/solution/P3383
    return false;
}

bool miller_rabin(long long n)
{
    if (n == 2) return true;
    if (n<2 || (n&1) == 0) return false; //0,1 and even numbers
    
    for (auto ch : checkers)
    {
        printf("Checking %lld using %lld...\n", n, ch);
        if (ch == n) return true;
        if (!miller_rabin_sr(n, ch) )
            return false;
    }
    return true;
}

int main()
{
    long long x;
    while (!feof(stdin))
    {
        scanf("%lld", &x);
        printf("%s\n", miller_rabin(x) ? "Yes" : "No");
    }
    return 0;
}
