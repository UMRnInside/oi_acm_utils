#include <cstdio>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#define DEBUG(_s, ...) fprintf(stderr, _s, ##__VA_ARGS__);
#define MAXN 107

std::vector<int> graph[MAXN];
int blk_degin[MAXN];
int n, m;

// for tarjan
std::stack<int> tstack;
int dfsid;
int dfn[MAXN], low[MAXN];
int blkid[MAXN], blksize[MAXN], blkcount;

void tarjan(int cur)
{
    dfn[cur] = low[cur] = ++dfsid;
    tstack.push(cur);
    for (auto dst : graph[cur] )
    {
        if (dfn[dst])
        {
            if (!blkid[dst]) // !
                low[cur] = std::min(low[cur], dfn[dst]);
        }
        else
        {
            tarjan(dst);
            low[cur] = std::min(low[cur], low[dst]);
        }
    }
    if (low[cur] == dfn[cur])
    {
        //int now = tstack.top();tstack.pop();
        blkid[cur] = ++blkcount;
        blksize[ blkcount ]++;

        while (tstack.top() != cur)
        {
            blksize[ blkcount ]++;
            blkid[tstack.top()] = blkcount;
            tstack.pop();
        }
        // remove cur
        tstack.pop();
    }
}

// tarjan end

int main()
{
    scanf("%d", &n);
    for (int i=1;i<=n;i++)
    {
        int y = 1;
        while (y)
        {
            scanf("%d", &y);
            if (y == 0)
                break;
            graph[i].push_back(y);
        }
    }

    for (int i=1;i<=n;i++)
        if (!dfn[i])
            tarjan(i);
    // cows
    for (int i=1;i<=n;i++)
        for (auto dst : graph[i])
            if (blkid[i] != blkid[dst])
                blk_degin[ blkid[dst] ]++;

    int no_indeg=0;
    std::set<int> no_outs;
    for (int i=1;i<=n;i++)
        if (graph[i].size() == (unsigned)0)
            no_outs.insert(blkid[i]);

    for (int i=1;i<=blkcount;i++)
        if (blk_degin[i] == 0)
            no_indeg++;

    // debug
    /*
    DEBUG("blkids:\n");
    for (int i=1;i<=n;i++)
        DEBUG("%d ", blkid[i]);
    DEBUG("\nno_outs:\n");
    for (auto n : no_outs)
        DEBUG("%d ", n);
    DEBUG("\nno_indeg: %d\n", no_indeg);
    */

    if (blkcount == 1)
    {
        printf("1\n0");
    }
    else
    {
        int ans = std::max(no_indeg, (int)no_outs.size() );
        printf("%d\n%d", no_indeg, ans);
    }
    return 0;
}

// AC: https://www.luogu.org/problem/P2746
