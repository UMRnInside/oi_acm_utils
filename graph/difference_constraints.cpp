#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <algorithm>
#include <vector>
#define INF 0x3f3f3f3f
#define MAXN 20007
/* AC: https://www.luogu.org/problem/P1993 */

struct edge_t
{
    int dest, len;
};

int n, m;
std::vector<edge_t>* graph;

int mdist[MAXN];
bool visited[MAXN];

bool dspfa(int cur)
{
    visited[cur] = true;
    for (auto e : graph[cur])
    {
        if (mdist[e.dest] < mdist[cur] + e.len)
        {
            mdist[e.dest] = mdist[cur] + e.len;
            if (visited[e.dest])
                return false;
            if (!dspfa(e.dest))
                return false;
        }
    }
    visited[cur] = false;
    return true;
}

int main()
{
    scanf("%d%d", &n, &m);
    graph = new std::vector<edge_t> [n+1];

    while ( m --> 0 )
    {
        int type, a, b, c;
        scanf("%d%d%d", &type, &a, &b);
        if (type == 1)
        {
            scanf("%d", &c);
            graph[b].push_back( edge_t{a, c} );
        }
        else if (type == 2)
        {
            scanf("%d", &c);
            graph[a].push_back( edge_t{b, -c});
        }
        else if (type == 3)
        {
            graph[a].push_back( edge_t{b, 0} );
            graph[b].push_back( edge_t{a, 0} );
        }
    }
    for (int i=1;i<=n;i++)
        graph[0].push_back(edge_t{i, 0}), mdist[i] = -INF;

    printf("%s\n", dspfa(0) ? "Yes" : "No");
    return 0;
}
