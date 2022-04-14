#pragma once

namespace coversion
{
    class number
    {
    public:

        /*
        * 32位的任意进制转换
        * @param in    : 输入字符串
        * @param s_in  : 输入进制
        * @param out   : 输出字符串
        * @param s_out : 输出进制
        */
        static void any32(const char* in, int s_in, char* out, int s_out);
    };
    
} // namespace coversion
