#include <stdio.h>
#include <assert.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h> 
#include "test.h"

int 
suite_success_init(void)
{ 
    return 0; 
}

int 
suite_success_clean(void)
{
     return 0; 
}
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

int 
addTestModule()
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("str_to_int测试模块", suite_success_init, suite_success_clean);  
    if (pSuite == NULL) {
        return -1;
    }
    if (NULL == CU_add_test(pSuite,"str_to_int",TEST_str_to_int)) {
		return -1;
	}
    return 0;
}

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