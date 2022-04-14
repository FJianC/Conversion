#include <iostream>
#include <assert.h>

#include "floatcoversion.h"

int main()
{
    coversion::floating v;

    {
        constexpr auto a = 11.98f;
        printf("float                 %.20f\n", a);
        const auto& b = v.ToBit(a);
        printf("float      32 bit  => %s\n", b.c_str());
        const auto& c = v.BitTo<float>(b);
        printf("float      recover => %.20f\n", c);
        assert(a == c);
        const auto& d = v.ToBitByIEEE(a);
        printf("floatIEEE  32 bit  => %s\n", d.c_str());
        assert(b == d);
        const auto& e = v.BitToByIEEE<float>(d);
        printf("floatIEEE  recover => %.20f\n", e);
        assert(c == e);
    }
    printf("\n");
        {
        constexpr auto a = 11.98;
        printf("double                %.20f\n", a);
        const auto& b = v.ToBit(a);
        printf("double     32 bit  => %s\n", b.c_str());
        const auto& c = v.BitTo<double>(b);
        printf("double     recover => %.20f\n", c);
        assert(a == c);
        const auto& d = v.ToBitByIEEE(a);
        printf("doubleIEEE 32 bit  => %s\n", d.c_str());
        const auto& e = v.BitToByIEEE<double>(d);
        printf("doubleIEEE recover => %.20f\n", e);
        assert(a == e);
    }

    return 0;
}