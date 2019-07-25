#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>

int n, m;
std::vector<int> primes;
bool* is_prime;
int* phi; // Euler's totient function

void euler_init()
{
    // init
    for (int i=2;i<=n;i++) is_prime[i] = true;
    is_prime[0] = false, is_prime[1] = false;
    phi[1] = 1;

    // euler sieve
    for (int i=2;i<=n;i++)
    {
        if (is_prime[i])
        {
            primes.push_back(i);
            phi[i] = i-1;
        }
        for (auto pr : primes)
        {
            if (i*pr > n) break;

            is_prime[i*pr] = false;
            if (i%pr == 0)
            {
                phi[ i*pr ] = pr * phi[i];
                break;// for sieve
            }
            else
                phi[ i*pr ] = (pr-1) * phi[i];
        }
        // See https://www.luogu.org/blog/user51357/xian-xing-shai-qiu-ou-la-han-shuo
    }
}

int main()
{
    scanf("%d%d", &n, &m);
    is_prime = (bool*)malloc(sizeof(bool) * (n+17));
    phi = (int*)malloc(sizeof(int) * (n+17));

    euler_init();

    while (m --> 0)
    {
        int tmp;
        scanf("%d", &tmp);
        printf("%d\n", phi[tmp]);
    }
    return 0;
}
