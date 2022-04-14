#include <iostream>

#include "numbercoversion.h"

int main()
{
    char out[33] = {0};
    for (int i = 2; i <= 36; ++i)
    {
        coversion::number::any32("10000", 10, out, i);
        printf("10000 to %d => %s\n", i, out);
        memset(out, 0, sizeof(out));
    }

    return 0;
}