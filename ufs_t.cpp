// ufs, useful in various scernaios (e.g. kruskal)

struct ufs_t
{
    int* father;
    size_t limit;
    ufs_t(size_t sz)
    {
        limit = sz;
        father = new int[sz];
        for (size_t i=0;i<sz;i++)
            father[i] = (int)i;
    };
    ~ufs_t()
    {
        delete [] father;
    };
    int query(int x)
    {
        if (father[x] == x) 
            return x;
        else
            return father[x] = query(father[x]);
    };
    void merge(int a, int b)
    {
        int fa = query(a), fb = query(b);
        father[fb] = fa;
    };
};
