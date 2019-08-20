#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <algorithm>
#define MAXN 1007
#define MAXL 100007
#define CHAR_RANGE 128
#define memset0(_p) memset( (_p), 0, sizeof( (_p) ) )
#define c2ui(_x) ( (size_t)(_x) )
namespace AC
{
    const int SZ = MAXN * 53;
    int total, trie[SZ][CHAR_RANGE]; // 'a' - 'z'
    int fail[SZ], index[SZ], value[SZ];

    int count[MAXN]; // for inserted strings
    void init()
    {
        memset0(fail);
        memset0(trie);
        memset0(value);
        memset0(count);
        memset0(index);

        total = 0;
    }
    void insert(char* s, int id)
    {
        int u = 0;
        // 0 - terminated.
        for (char* p = s;*p;p++)
        {
            if (trie[u][ c2ui(*p) ] == 0)
                trie[u][ c2ui(*p) ] = ++total;
            u = trie[u][ c2ui(*p) ];
        }
        index[u] = id;
    }

    void build()
    {
        std::queue<int> q;
        // NOTE! i<CHAR_RANGE
        for (int i=0;i<CHAR_RANGE;i++)
            if (trie[0][i])
                q.push(trie[0][i]);
        while (!q.empty())
        {
            int cur = q.front();q.pop();
            for (int i=0;i<CHAR_RANGE;i++)
            {
                if (trie[cur][i])
                {
                    fail[ trie[cur][i] ] = trie[fail[cur]][i];
                    q.push(trie[cur][i]);
                }
                else
                {
                    trie[cur][i] = trie[fail[cur]][i];
                }
            }
        }
    }

    int run_str(char* t)
    {
        int cur = 0, max_appeared = 0;
        for (char* p=t;*p;p++)
        {
            cur = trie[cur][ c2ui(*p) ];
            for (int j=cur;j>0;j=fail[j])
                value[j]++;
        }
        for (int i=0;i<=total;i++)
            if (index[i])
            {
                max_appeared = std::max(max_appeared, value[i]);
                count[ index[i] ] = value[i];
            }
        return max_appeared;
    }
}

int n;
char strs[MAXN][100];

int main()
{
    char target[2097152];
    while ( scanf("%d", &n) > 0)
    {
        if (n == 0)
            break;
        AC::init();
        for (int i=1;i<=n;i++)
        {
            scanf("%s", strs[i]);
            AC::insert(strs[i], i);
        }
        AC::build();

        scanf("%s", target);
        AC::run_str(target);
        for (int i=1;i<=n;i++)
            if (AC::count[i] > 0)
                printf("%s: %d\n", strs[i], AC::count[i]);
    }
    return 0;
}
