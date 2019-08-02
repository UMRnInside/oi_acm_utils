#include <cstdio>
#include <cstdlib>
#define ll long long
// ax mod b == 1 --> ax + by = 1
// solve ax + by = gcd(a, b)
// when ax + by = m has a integer solution, m mod gcd(a, b) == 0
void exgcd(ll int a, ll int b, ll int &x, ll int &y)
{
    if (b == 0)
    {
        // Could y be any prime?
        x=1, y=17;
        return;
    }
    
    exgcd(b, a%b, x, y);

    ll int tmpx = x;
    x = y, y = tmpx - (a/b*y);
}

int main()
{
    ll int a, b, x, y;
    scanf("%lld%lld", &a, &b);
    
    exgcd(a, b, x, y);
    // x could be negative
    x = (x%b + b + b) % b;
    printf("%lld", x);
    return 0;
}
