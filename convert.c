#include<limits.h>
// 版本1
// int 
// str_to_int(char* s)
// {
//     int sum = 0;
//     char *p = s;
//     while (*p != 0) {
//         sum = sum * 10 + *p -'0';
//         p++;
//     }
//     return sum;
// }

// 版本2
int 
str_to_int(char* s)
{
    int     sign = 1;
    char    *p = s;
    long    sum = 0;
    
    // 指针判空
    if (!s) {
        return 0;
    }
    
    // 排除空格
    while (*p == ' ')
        p++;
    //控制符号
    if (*p == '-') {
        sign = -1;
        p++;
    } else if (*p == '+') {
        sign = 1;
        p++;
    }

    while (*p >= '0' && *p <= '9') {
        // 左移比乘法效率高,注意优先级
        sum = (sum << 3) + (sum << 1) + *p -'0';
        // 处理溢出
        if ((sum > INT_MAX && sign == 1) || sum > (long)INT_MAX + 1 && sign == -1) {
            sum = 0;
            break;
        }
        p++;
    }
    return (int)sum * sign;
}