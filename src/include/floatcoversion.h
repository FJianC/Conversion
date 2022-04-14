#pragma once

#include <string>
#include <cmath>

namespace coversion
{
    class floating
    {
    public:
    
        /**
         * 参考《深入理解计算机系统》第二章第四节
         * 
         * IEEE 浮点标准 V = (-1)^s * M * 2^E
         * s : 符号，占1位，决定这个数是负数(s=1)还是正数(s=0)
         * E : 阶码，占k位，对浮点数加权，权重是2的E次幂，E=e-Bais，e:指数偏移，Bais:固定偏移值=(2^(k-1)-1)
         * M : 尾数，占n位，二进制小数，M=1+f，f:小数值，有效数字表示时，第一位总是1，所以隐藏第一位以获得额外的精度
         * 
         * 单精度32位
         * s|e       |f
         * 0|00000000|00000000000000000000000
         * s : 占1位
         * e : k=8, 因 E=e-Bais，e=E+(2^(8-1)-1)
         * f : 小数值，n=23，且范围 0<=f<1
         * 
         * 双精度64位
         * s|e          |f
         * 0|00000000000|0000000000000000000000000000000000000000000000000000
         * s : 占1位
         * e : k=11, 因 E=e-Bais，e=E+(2^(11-1)-1)
         * f : 小数值，n=52，且范围 0<=f<1
         * 
         * 公式展开 V = (-1)^s * (1+f) * 2^(2^(k-1)-1)
         * 
         * 舍入：向偶数舍入法，二进制小数最低有效数字是0，则可以近似舍入
         * 
         * 规范化值：阶码非全0和非全1
         * 非规范化值：阶码全0，小数值接近于0，表示逐渐溢出
         * 特殊值：阶码全1，小数值全0，表示无穷，符号1为正无穷，符号0为负无穷
         * 特殊值：阶码全0，小数非0，表示NaN
         * 
         * 关键字：s，E，k，e，Bais，M，n，f
        */

        /**
         * 使用 union 实现
         * @param in : 输入浮点数
         * @return 返回二进制表示的字符串
        */
        template<typename T>
        std::string ToBit(const T& in)
        {
            static_assert(std::is_floating_point<std::decay<T>::type>::value, "must floating"); // 浮点型
            constexpr auto SIZE = sizeof(T) * 8;        // 类型长度
            constexpr auto S_BIT = 1;                   // 符号域长度
            constexpr auto E_BIT = SIZE == 64 ? 11 : 8; // 阶码域长度
            constexpr auto M_BIT = SIZE - E_BIT - 1;    // 尾数域长度
            union 
            {
                T v = 0.0;
                struct 
                {
#if '\x01\x02\x03\x04' == 0x01020304    // little-endian
                    uint64_t s : S_BIT;
                    uint64_t e : E_BIT;
                    uint64_t f : M_BIT;
#elif '\x01\x02\x03\x04' == 0x04030201  // big-endian
                    uint64_t f : M_BIT;
                    uint64_t e : E_BIT;
                    uint64_t s : S_BIT;
#else
#error "??? What endian do I meet?"
#endif
                } raw;
            } _float;
            _float.v = in;
            
            std::string ret;
            for (auto i = S_BIT - 1; i >= 0 && i <= S_BIT - 1; --i) 
                ret += (_float.raw.s >> i) & 1 ? '1' : '0';
            for (auto i = E_BIT - 1; i >= 0 && i <= E_BIT - 1; --i) 
                ret += (_float.raw.e >> i) & 1 ? '1' : '0';
            for (auto i = M_BIT - 1; i >= 0 && i <= M_BIT - 1; --i) 
                ret += (_float.raw.f >> i) & 1 ? '1' : '0';
            return ret;
        }

        /**
         * 使用 union 实现
         * @param in : 输入二进制表示的字符串
         * @return 返回浮点数
        */
        template<typename T>
        T BitTo(const std::string& in)
        {
            static_assert(std::is_floating_point<std::decay<T>::type>::value, "must floating"); // 浮点型
            constexpr auto SIZE = sizeof(T) * 8;        // 类型长度
            constexpr auto S_BIT = 1;                   // 符号域长度
            constexpr auto E_BIT = SIZE == 64 ? 11 : 8; // 阶码域长度
            constexpr auto M_BIT = SIZE - E_BIT - 1;    // 尾数域长度
            union 
            {
                T v = 0.0;
                struct 
                {
#if '\x01\x02\x03\x04' == 0x01020304    // little-endian
                    uint64_t s : S_BIT;
                    uint64_t e : E_BIT;
                    uint64_t f : M_BIT;
#elif '\x01\x02\x03\x04' == 0x04030201  // big-endian
                    uint64_t f : M_BIT;
                    uint64_t e : E_BIT;
                    uint64_t s : S_BIT;
#else
#error "??? What endian do I meet?"
#endif
                } raw;
            } _float;
            
            const char* p = in.c_str();
            for (auto i = S_BIT - 1; i >= 0 && i <= S_BIT - 1 && *p; --i, ++p) 
                _float.raw.s |= *p == '1' ? (uint64_t)1 << i : 0;
            for (auto i = E_BIT - 1; i >= 0 && i <= E_BIT - 1 && *p; --i, ++p) 
                _float.raw.e |= *p == '1' ? (uint64_t)1 << i : 0;
            for (auto i = M_BIT - 1; i >= 0 && i <= M_BIT - 1 && *p; --i, ++p) 
                _float.raw.f |= *p == '1' ? (uint64_t)1 << i : 0;
            
            return _float.v;
        }

        /**
         * 按照 IEEE 标准实现
         * @param in : 输入浮点数
         * @return 返回二进制表示的字符串
        */
        template<typename T>
        std::string ToBitByIEEE(T in)
        {
            static_assert(std::is_floating_point<std::decay<T>::type>::value, "must floating"); // 浮点型
            constexpr auto SIZE = sizeof(T) * 8;        // 类型长度
            constexpr auto S_BIT = 1;                   // 符号域长度
            constexpr auto E_BIT = SIZE == 64 ? 11 : 8; // 阶码域长度
            constexpr auto M_BIT = SIZE - E_BIT - 1;    // 尾数域长度

            // s 符号
            const auto& sign = in >= 0 ? "0" : "1";

            // 已记录符号，之后以正数处理
            in = std::abs(in);

            // 整数部分转二进制
            std::string int_str;
            for (int64_t i = (int64_t)in; i; i /= 2)
                int_str += i % 2 + '0';
            std::reverse(int_str.begin(), int_str.end());

            // 小数部分转二进制
            std::string f_str = "0";
            for (T i = (in - (int64_t)in) * 2; f_str.size() < SIZE; i *= 2, f_str += '0')
            {
                f_str[f_str.size() - 1] += (int8_t)i;
                i -= (int64_t)i;
            }

            // 计算小数值有效数字指数偏移
            int64_t E = 0;
            if (int_str.find('1') != std::string::npos)
            {
                E = int_str.size() - int_str.find('1') - 1;
                int_str = int_str.substr(int_str.find('1') + 1);
            }
            else if (f_str.find('1') != std::string::npos)
            {
                E -= f_str.find('1') + 1;
                f_str = f_str.substr(f_str.find('1') + 1);
            }

            // 阶码部分转二进制
            std::string e_str;
            for (int64_t e = E + (int64_t)(std::pow(2, E_BIT - 1) - 1); e_str.size() < E_BIT; e /= 2)
                e_str += e % 2 + '0';
            std::reverse(e_str.begin(), e_str.end());

            // 尾数部分转二进制
            f_str = int_str + f_str;
            f_str.resize(M_BIT, '0');

            return sign + e_str + f_str;
        }

        /**
         * 按照 IEEE 标准实现
         * @param in : 输入二进制表示的字符串
         * @return 返回浮点数
        */
        template<typename T>
        T BitToByIEEE(std::string in)
        {
            static_assert(std::is_floating_point<std::decay<T>::type>::value, "must floating"); // 浮点型
            constexpr auto SIZE = sizeof(T) * 8;        // 类型长度
            constexpr auto S_BIT = 1;                   // 符号域长度
            constexpr auto E_BIT = SIZE == 64 ? 11 : 8; // 阶码域长度
            constexpr auto M_BIT = SIZE - E_BIT - 1;    // 尾数域长度

            in.resize(SIZE, '0');

            // 符号
            const auto& s = in[S_BIT - 1] - '0';

            // 阶码
            int64_t e = 0;
            for (auto i = 0; i < E_BIT; ++i)
                e += (in[i + S_BIT] - '0') * (int64_t)std::pow(2, E_BIT - 1 -i);

            // 尾数
            T f = 0.0;
            for (auto i = 0; i < M_BIT; ++i)
                f += in[i + 1 + E_BIT] == '1' ? (T)std::pow(2, -i - 1) : 0;
            
            return (T)(std::pow(-1, s) * (1 + f) * std::pow(2, e - (std::pow(2, E_BIT - 1) - 1)));
        }
    };
};
