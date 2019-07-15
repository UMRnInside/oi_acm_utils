#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>

void mergesort(int* a, int l, int r)
{
    if (l == r) return;
    int mid = (l+r) / 2;
    
    mergesort(a, l, mid);
    mergesort(a, mid+1, r);
    
    int* temp = new int[2*(r-l+1)];
    int temptop = 0;
    
    int i=l, j=mid+1;
    while (i<=mid && j<=r)
    {
        // operator< !
        if (a[i]<a[j])
            temp[temptop++] = a[i++]; // ++i?
        else
            temp[temptop++] = a[j++]; // ++j?
        /* 
         * See https://www.luogu.org/problemnew/solution/P1309
         * Somebody said that only ++i can work properly
         * But it seemed that i++ also works
         */
    }
    
    while (i <= mid) temp[temptop++] = a[i++];
    while (j <= r) temp[temptop++] = a[j++];
    for (int k=0;k<temptop;k++) a[k+l] = temp[k];
    
    delete [] temp;
    /*
    printf("%d-%d:", l, r);
    for (int i=l;i<=r;i++)
        printf("%d ", a[i]);
    printf("\n"); */
    return;
}

int main()
{
    int n;
    scanf("%d", &n);
    
    int* a = new int[n];
    for (int i=0;i<n;i++)
        scanf("%d", a+i);
    
    mergesort(a, 0, n-1);
    
    for (int i=0;i<n;i++)
        printf("%d ", a[i]);
    
    return 0;
}
