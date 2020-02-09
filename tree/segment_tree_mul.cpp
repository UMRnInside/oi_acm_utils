#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <functional>
// MACROS!
// TODO: Faster/Shorter expression
#define lch(_x) ( (_x)<<1 )
#define rch(_x) ( ((_x)<<1) | 1)

/* NOTE: 
 * Marcos needed: lch(_x), rch(_x)
 * See https://www.luogu.org/problemnew/show/P3373
 */

long long int modn;

namespace segment_tree
{
    typedef long long DATATYPE;
    size_t limit;
    struct stnode_t
    {
        int l, r;
        DATATYPE value;
        DATATYPE addv=0, mulv=1; // lazytag
    };
    inline void push_up_to(stnode_t* tree, int cur)
    {
        tree[cur].value = (tree[lch(cur)].value + tree[rch(cur)].value) % modn;
    }

    void autobuild(stnode_t* tree, int cur, int l, int r, DATATYPE* a)
    {
        tree[cur].l = l, tree[cur].r = r;
        if (l == r)
        {
            tree[cur].value = a[l] % modn;
            return;
        }
        // WARNING!
        int mid = (l+r)>>1;
        autobuild(tree, lch(cur), l, mid, a);
        autobuild(tree, rch(cur), mid+1, r, a);
        // remember to push up!
        push_up_to(tree, cur);
    }

    void cast_lazytag(stnode_t* tree, int cur)
    {
        // range add
        // WARNING: tree[rch(cur)] may be OUT OF RANGE!
        stnode_t* parent = tree+cur;
        stnode_t *lchild = tree+lch(cur), *rchild=tree+rch(cur);
        if (parent->mulv != 1)
        {
            lchild->value = (lchild->value * parent->mulv) % modn;
            rchild->value = (rchild->value * parent->mulv) % modn;

            lchild->mulv = (lchild->mulv * parent->mulv) % modn; 
            rchild->mulv = (rchild->mulv * parent->mulv) % modn;

            lchild->addv = (lchild->addv * parent->mulv) % modn;
            rchild->addv = (rchild->addv * parent->mulv) % modn;

            parent->mulv = 1;
        }

        if (parent->addv)
        {
            lchild->value += parent->addv * (lchild->r - lchild->l + 1);
            rchild->value += parent->addv * (rchild->r - rchild->l + 1);
            
            lchild->addv += parent->addv, rchild->addv += parent->addv;
            parent->addv = 0;
            lchild->value %= modn, rchild->value %= modn;
        }
    }
    void range_modify(stnode_t* tree, int cur, int l, int r, DATATYPE delta)
    {
        stnode_t* parent = tree+cur;

        if (l <= parent->l && parent->r <= r) // fully covered
        {
            parent->value += delta * (parent->r - parent->l + 1);
            parent->addv += delta;
            return;
        }
        // if not fully covered...
        cast_lazytag(tree, cur);
        int mid = (parent->l + parent->r) >> 1;
        if (l <= mid  ) range_modify(tree, lch(cur), l, r, delta);
        if (r >= mid+1) range_modify(tree, rch(cur), l, r, delta);

        push_up_to(tree, cur);
    }

    void range_multiply(stnode_t* tree, int cur, int l, int r, DATATYPE mult)
    {
        stnode_t* parent = tree+cur;

        if (l <= parent->l && parent->r <= r) // fully covered
        {
            parent->value = (parent->value * mult) % modn;
            parent->mulv = (parent->mulv * mult) % modn;
            parent->addv = (parent->addv * mult) % modn;
            return;
        }
        // if not fully covered...
        cast_lazytag(tree, cur);
        int mid = (parent->l + parent->r) >> 1;
        if (l <= mid  ) range_multiply(tree, lch(cur), l, r, mult);
        if (r >= mid+1) range_multiply(tree, rch(cur), l, r, mult);

        push_up_to(tree, cur);
    }

    DATATYPE range_query(stnode_t* tree, int cur, int l, int r)
    {
        stnode_t* parent = tree+cur;

        if (l <= parent->l && parent->r <= r) // fully covered
            return parent->value;

        cast_lazytag(tree, cur);

        DATATYPE result = 0;
        int mid = (parent->l + parent->r) >> 1;

        if (l <= mid  ) result = (result + range_query(tree, lch(cur), l, r) ) % modn;
        if (r >= mid+1) result = (result + range_query(tree, rch(cur), l, r) ) % modn;
        return result;
    }
}

const size_t MAXN = 100007;
int n, m;
long long a[MAXN];
segment_tree::stnode_t tree[MAXN*4 + 1];

int main()
{
    scanf("%d%d%lld", &n, &m, &modn);
    for (int i=1;i<=n;i++)
        scanf("%lld", a+i);

    segment_tree::autobuild(tree, 1, 1, n, a);
    //printf("build success.\n");
    while ( m --> 0 )
    {
        int q, l, r, z;
        scanf("%d", &q);
        if (q == 1)
        {
            scanf("%d%d%d", &l, &r, &z);
            segment_tree::range_multiply(tree, 1, l, r, z);
        }
        else if (q == 2)
        {
            scanf("%d%d%d", &l, &r, &z);
            segment_tree::range_modify(tree, 1, l, r, z);
        }
        else if (q == 3)
        {
            scanf("%d%d", &l, &r);
            printf("%lld\n", segment_tree::range_query(tree, 1, l, r) % modn );
        }
    }
    return 0;
}
