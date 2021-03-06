# 进制转换

## 整形进制转换

```cpp
#include "numbercoversion.h"
```

## 浮点数进制转换

```cpp
#include "numbercoversion.h"
```

## 浮点型数据存储原理

现代计算机中，一般以 IEEE 754 标准存储浮点数  

参考 **《深入理解计算机系统》** 第二章第四节  

### IEEE 浮点标准

`V = (-1)^s * M * 2^E`

- **s** : 符号，占 **1** 位，决定这个是数是负数`s=1`还是正数`s=0`
- **E** : 阶码，占 **k** 位，对浮点数加权，权重是2的E次幂，`E=e-Bais`，**e** : 指数偏移，**Bais** : 固定偏移值`=(2^(k-1)-1)`
- **M** : 尾数，占 **n** 位，二进制小数，`M=1+f`，**f** : 小数值，有效数字表示时，第一位总是 **1**，所以隐藏第一位以获得额外的精度

### 单精度表示

| 符号 | 阶码 | 尾数 |
| :- | :- | :- |
| sign | exponent | fraction |
| 0 | 00000000 | 00000000000000000000000 |

- **s** : 占 **1** 位
- **e** : 占 **8** 位，`k=8, E=e-(2^(k-1)-1) => e=E+127`
- **f** : 占 **23** 位，`n=23, M=1+f, => 0<=f<1`

### 双精度表示

| 符号 | 阶码 | 尾数 |
| :- | :- | :- |
| sign | exponent | fraction |
| 0 | 00000000000 | 0000000000000000000000000000000000000000000000000000 |

- **s** : 占 **1** 位
- **e** : 占 **11** 位，`k=8, E=e-(2^(k-1)-1) => e=E+127`
- **f** : 占 **52** 位，`n=23, M=1+f, => 0<=f<1`

### 表示公式

`V = (-1)^s * (1+f) * 2^(e-(2^(k-1)-1))`

### 舍入

转换可选择使用 **向偶数舍入法**
- 二进制小数最低有效数字是`"....100"`的形式，可以近似舍入

### 特殊表示

#### 规范化值

- 阶码**非全0**和**非全1**

| 符号 | 阶码 | 尾数 |
| :- | :- | :- |
| sign | exponent | fraction |
| 0 or 1 | &ne;0 and &ne;255 | f |

#### 非规范化值

- 阶码**全0**，小数值接近于0，表示**逐渐溢出**

| 符号 | 阶码 | 尾数 |
| :- | :- | :- |
| sign | exponent | fraction |
| 0 or 1 | 00000000 | f |

#### 特殊值

- 阶码**全1**，小数值**全0**，表示**无穷&infin;**，符号0为+&infin;，符号1为-&infin;

| 符号 | 阶码 | 尾数 |
| :- | :- | :- |
| sign | exponent | fraction |
| 0 or 1 | 11111111 | 00000000000000000000000 |

- 阶码**全0**，小数**非0**，表示`NaN`

| 符号 | 阶码 | 尾数 |
| :- | :- | :- |
| sign | exponent | fraction |
| 0 or 1 | 11111111 | &ne;0 |
