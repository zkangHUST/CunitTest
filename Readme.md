针对C语言的测试框架相比其他语言要少一些，本文简单介绍一下Cunit框架的基本使用方法，权当备忘吧。Cunit的组织框架如下图所示：
将单个测试用例打包到一个suite中，这些suite在Registry中注册。registry中的所有suite/tests可以使用单个函数调用运行，也可以运行选定的套件或测试。下面看一个例子

首先我们新建文件，写一个待测试函数，这里以一个字符串转换数字函数为例，这是我们的第一版实现

```C
// convert.c
// 版本1
int 
str_to_int(char* s)
{
    int sum = 0;
    char *p = s;
    while (*p != 0) {
        sum = sum * 10 + *p -'0';
        p++;
    }
    return sum;
}
```

显然这个函数是有很多问题的，我们写一个测试函数来测试它。

```C
void 
TEST_str_to_int()
{
    int ans;
    char *p;
    char *ps[] = { "123", "-123", "0", "siahideib",  "2147483648", "2147483647", "-1-2", "            -2147483647", "  -2147483648aaaa"};
    int real[] = {123,    -123,   0,   0,            0,            2147483647,   -1,     -2147483647,               -2147483648};

    int i = 0;
    for (i = 0; i < sizeof(real)/sizeof(real[0]); i++) {
        ans = str_to_int(ps[i]);
        CU_ASSERT_EQUAL(ans,real[i]);
    }
}
```
测试函数和被测函数写好之后，我们就可以定义一个registry 和一个suite，在suite中添加被测函数，并在registry中注册。
```C
addTestModule()
{
    CU_pSuite pSuite = NULL;
    // 添加suite
    pSuite = CU_add_suite("str_to_int测试模块", suite_success_init, suite_success_clean);  
    if (pSuite == NULL) {
        return -1;
    }
    // 在suite中添加被测函数
    if (NULL == CU_add_test(pSuite,"str_to_int",TEST_str_to_int)) {
		return -1;
	}
    return 0;
}
```
Cunit提供了三种模式查看单元测试结果，分别是控制台模式，基本模式和报表模式

```C
void 
run_test()
{
    if (CU_initialize_registry()) {
        printf("error");
        return ;
    } 
    assert(NULL != CU_get_registry());
    assert(!CU_is_test_running()); 
    if (0 != addTestModule()) {
		CU_cleanup_registry();
		return ;
	}
    
    // 报表模式
    // 设置输出文件名称
    CU_set_output_filename("str_to_int_test_report");
    CU_list_tests_to_file();
    CU_automated_run_tests();

    // 基本模式
    // CU_basic_set_mode(CU_BRM_VERBOSE);
    // CU_basic_run_tests();      
    // 控制台模式
    // CU_console_run_tests();       
    // CU_console_run_tests();
    CU_cleanup_registry();
}
```
最后写一个main函数来调用测试函数
```C
#include<stdio.h>
#include "test.h"
int 
main()
{
    run_test();
    return 0;
}
```
编译运行

```bash
gcc -g convert.c main.c test.c -lcunit
./a.out
```
运行完成后，在当前目录下可以看到生成了两个xml文件，这两个文件就是得到的测试结果。
```
str_to_int_test_report-Listing.xml
str_to_int_test_report-Results.xml
```
不过这两个文件不能直接打开，需要把cunit安装目录下的另外四个文件复制过来。如果默认安装的话，这四个文件一般在/usr/local/share/CUnit目录下
```
CUnit-List.dtd
CUnit-List.xsl  
CUnit-Run.dtd  
CUnit-Run.xsl
```
把这6个文件拷贝到同一路径下，就可以通过IE或者Edge浏览器查看输出结果。

可以看到，9个测试用例有3个没有通过。改进一下被测函数
```C
// convert.c
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
```
重新编译，运行，看一下测试结果，这一次9个case全部通过


