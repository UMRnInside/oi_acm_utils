#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <algorithm>
#define MAXN 207
#define MAXL 100007
#define memset0(_p) memset( (_p), 0, sizeof( (_p) ) )
#define c2ui(_x) ( (size_t)( (_x) - 'a') )
namespace AC
{
    const int SZ = MAXN * 80;
    int total, trie[SZ][26]; // 'a' - 'z'
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
        // NOTE! i<26
        for (int i=0;i<26;i++)
            if (trie[0][i])
                q.push(trie[0][i]);
        while (!q.empty())
        {
            int cur = q.front();q.pop();
            for (int i=0;i<26;i++)
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
    char target[1048576];
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
        int max_appeared = AC::run_str(target);
        printf("%d\n", max_appeared);
        for (int i=1;i<=n;i++)
            if (AC::count[i] == max_appeared)
                printf("%s\n", strs[i]);
    }
    return 0;
}
// AC: https://www.luogu.org/problem/P3796
