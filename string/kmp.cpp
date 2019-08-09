#include <cstdio>
#include <cstdlib>
#include <cstring>

void get_next(char* str, int* next)
{
    int len = strlen(str);
    next[0] = -1;

    int k=-1, j=0;
    while (j < len - 1)
    {
        if (k==-1 || str[j] == str[k])
        {
            j++, k++;
            if (str[j] != str[k])
                next[j] = k;
            else
                next[j] = next[k];
        }
        else
            k = next[k];
    }
}

int kmp_search(char* str, char* pattern)
{
    int i=0, j=0;
    int slen = strlen(str), plen = strlen(pattern);
    int* next = new int[plen+2];
    get_next(pattern, next);

    while (i<slen && j<plen)
    {
        if (j == -1 || str[i] == pattern[j])
            i++, j++;
        else
            j = next[j];
    }
    delete [] next;
    if (j == plen)
        return i-j;
    else
        return -1;
}

int main()
{
    char s[4096], p[3096];
    scanf("%s", s);
    scanf("%s", p);
    int offset = kmp_search(s, p);

    printf(" %s\n", s);

    for (int i=0;i<=offset;i++)
        printf(" ");
    printf("^\n");
    return 0;
}
