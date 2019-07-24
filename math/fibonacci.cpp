#include <cstdio>
#include <cstdlib>

template <class INT>
void matrix_mult2(INT a[2][2], INT b[2][2], INT c[2][2])
{
    c[0][0] = c[0][1] = c[1][0] = c[1][1] = 0;
    for (int i=0;i<2;i++)
        for (int j=0;j<2;j++)
            for (int k=0;k<2;k++)
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
}

template <class INT>
INT fastfib(INT n)
{
    INT base[2][2] = { {1, 1}, {1, 0} };
    INT answer[2][2] = { {1, 0}, {0, 1} };
    INT tmp[2][2];
    for (;n>0;n>>=1)
    {
        if (n & 1)
        {
            matrix_mult2(answer, base, tmp);
            answer[0][0] = tmp[0][0], \
            answer[0][1] = tmp[0][1], \
            answer[1][0] = tmp[1][0], \
            answer[1][1] = tmp[1][1];
        }
        matrix_mult2(base, base, tmp);
        base[0][0] = tmp[0][0], \
        base[0][1] = tmp[0][1], \
        base[1][0] = tmp[1][0], \
        base[1][1] = tmp[1][1];
    }
    return answer[0][1];
}

int main()
{
    int n;
    scanf("%d", &n);
    printf("%d\n", fastfib(n) );
    return 0;
}
