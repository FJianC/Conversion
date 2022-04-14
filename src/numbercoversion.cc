#include "numbercoversion.h"

#include <memory>

void coversion::number::any32(const char* in, int s_in, char* out, int s_out)
{
    int v = 0, n = 31;
    for (; *in; ++in)
        v = v * s_in + *in - (*in >= 'a' ? 'a' - 10 : '0');
    for (; v && n; v /= s_out, --n)
        out[n] = (v % s_out < 10 ? '0' : 'a' - 10) + v % s_out;
    memmove(out, out + n + 1, 32 - n - 1);
    out[32 - n - 1] = 0;
}
