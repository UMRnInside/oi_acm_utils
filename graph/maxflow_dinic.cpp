#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <functional>
#include <vector>
#include <queue>
#define ll long long
#define INF 0x3f3f3f3f
#define MAXN 10007
/*
 * For : https://www.luogu.org/problemnew/show/P3376
 */
struct edge_t
{
    int dest, cap_left, rev_index;
};

int n, m, start, end;
// input i, output i+n
std::vector<edge_t> graph[MAXN];
int dist[MAXN];

inline void add_edge(int s, int t, int cap);
bool bfs();
int dfs(int cur, int value);
int maxflow();

int main()
{
    scanf("%d%d%d%d", &n, &m, &start, &end);
    // start/end cap should be INF
    for (int i=1;i<=m;i++)
    {
        int a, b, f;
        scanf("%d%d%d", &a, &b, &f);
        add_edge(a, b, f);
    }
    printf("%d", maxflow());
    return 0;
}

inline void add_edge(int s, int t, int cap)
{
    graph[s].push_back( edge_t{t, cap, (int)graph[t].size()} );
    // reverse 0-cap edges, required by dinic
    graph[t].push_back( edge_t{s, 0, (int)graph[s].size()-1} );
}

bool bfs()
{
    // Init - Phase 1
    memset(dist, 0x3f, sizeof(dist[0]) * MAXN);
    std::queue<int> q;
    // Init - Phase 2
    q.push(start), dist[start] = 0;

    while (!q.empty())
    {
        int cur = q.front();q.pop();
        if (cur == end)
            return true;
        for (auto e : graph[cur])
        {
            if (dist[e.dest] == INF && e.cap_left > 0)
            {
                dist[e.dest] = dist[cur] + 1;
                if (e.dest == end)
                    return true;
                q.push(e.dest);
            }
        }
    }
    return false;
}

int dfs(int cur, int flow)
{
    int answer = 0;
    if (cur == end) return flow;
    for (auto& e : graph[cur])
    {
        if (dist[e.dest] == dist[cur]+1 && e.cap_left > 0)
        {
            int tmp = dfs(e.dest, std::min(flow, e.cap_left) );

            e.cap_left -= tmp;
            // residual graph
            graph[e.dest][e.rev_index].cap_left += tmp;
            flow -= tmp, answer += tmp;
            if (flow == 0) break;
        }
    }
    return answer;
}

int maxflow()
{
    int answer = 0;
    while( bfs() )
        answer += dfs(start, INF);
    return answer;
}
