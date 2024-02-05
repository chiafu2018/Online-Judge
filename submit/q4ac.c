#include <stdio.h>
#include <stdlib.h>


int bwmax(int l, int r) { return l ^ ((l ^ r) & -(l < r)); }
 
void solve()
{
    int n, prod = 1, zero = 0, r = 0;
    scanf("%d", &n);
    int *a = malloc(sizeof(int) * n);
    for (int *ptr = a; ptr < a + n; ptr++)
    {
        scanf("%d", ptr);
        if (*ptr)
            prod *= *ptr;
        else
            ++zero;
    }
    if (zero >= 1)
    {
        printf("%d\n", zero == 1 ? prod : 0);
        return;
    }
    for (int *ptr = a; ptr < a + n; ptr++)
        r = bwmax(r, prod / *ptr * (*ptr + 1));
    printf("%d\n", r);
    free(a);
}
 
int main()
{
    int t;
    scanf("%d", &t);
    while (t--)
        solve();
    return 0;
}