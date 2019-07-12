#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>

int n, m;
std::vector<int> primes;
bool* is_prime;

void euler_sieve()
{
    // init
    for (int i=2;i<=n;i++) is_prime[i] = true;
    is_prime[0] = false, is_prime[1] = false;
    
    // sieve
    for (int i=2;i<=n;i++)
    {
        if (is_prime[i]) primes.push_back(i);
        for (auto pr : primes)
        {
            if (i*pr > n) break;
            
            is_prime[i*pr] = false;
            if (i%pr == 0) break;
        }
    }
}

int main()
{
    scanf("%d%d", &n, &m);
    is_prime = (bool*)malloc(sizeof(bool) * (n+17));
    
    euler_sieve();
    
    while (m --> 0)
    {
        int tmp;
        scanf("%d", &tmp);
        printf("%s\n", is_prime[tmp] ? "Yes" : "No");
    }
    return 0;
}
