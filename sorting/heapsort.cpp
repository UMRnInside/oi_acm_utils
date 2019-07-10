#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
// requires 0 ... n
#define lchild(_x) ((_x)*2 +1)
#define rchild(_x) ((_x)*2 +2)

int a[100000];
int n;

// big root
void br_heapify(int* a, int start, int end)
{
    int parent = start;
    int child = lchild(parent);
    while (child <= end)
    {
        if (rchild(parent) <= end && \
            a[lchild(parent)] < a[rchild(parent)])
            child = rchild(parent);
        if (a[parent] > a[child])
            return;
        else
        {
            std::swap(a[parent], a[child]);
            parent = child;
            child = lchild(parent);
        }
    }
}

void heapsort(int* a, int len)
{
    for (int i=len/2 -1; i>=0; i--)
        br_heapify(a, i, len-1);
    
    for (int i=len-1; i>0; i--)
    {
        std::swap(a[0], a[i]);
        br_heapify(a, 0, i-1);
    }
}

int main()
{
    scanf("%d", &n);
    for (int i=0;i<n;i++)
        scanf("%d", a+i);
    
    heapsort(a, n);
    
    for (int i=0;i<n;i++)
        printf("%d ", a[i]);
    return 0;
}
