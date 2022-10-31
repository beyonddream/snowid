#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "snowid.h"

#define TEST_PRE_SETUP(config) \
    snow_init(&(config))
    
#define TEST_POST_SETUP()  \
    snow_shutdown()

#define TEST_CHECK_RESULT(desc, result)                 \
    do {                                                \
        if ((result) != true) {                         \
            fprintf(stderr, "%s: NOT_OK\n", (desc));    \
            exit(1);                                    \
        } else {                                        \
            fprintf(stdout, "%s: OK\n", (desc));        \
        }                                               \
    } while(0)

bool test_snow_get_id() 
{
    snow_config_t config = {
        .interface = "en0",
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    TEST_PRE_SETUP(config);
    snow_id_t snow_id;
    bool expected = snow_get_id(&snow_id);
    TEST_POST_SETUP();
    
    return expected;
}

bool test_snow_get_id_as_binary() 
{
    snow_config_t config = {
        .interface = "en0",
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    TEST_PRE_SETUP(config);
    snow_id_t snow_id;
    unsigned char out[16] = {0};
    bool expected = snow_get_id_as_binary(out, &snow_id);
    TEST_POST_SETUP();

    return expected;
}

bool test_snow_get_id_for_unknown_interface() 
{
    snow_config_t config = {
        .interface = "xxx",
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    TEST_PRE_SETUP(config);
    snow_id_t snow_id;
    bool expected = snow_get_id(&snow_id);
    TEST_POST_SETUP();
    
    return expected;
}

bool test_snow_get_id_as_binary_for_unknown_interface() 
{
    snow_config_t config = {
        .interface = "xxx",
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    TEST_PRE_SETUP(config);
    snow_id_t snow_id;
    unsigned char out[16] = {0};
    bool expected = snow_get_id_as_binary(out, &snow_id);
    TEST_POST_SETUP();
    
    return expected;
}

bool test_snow_get_id_for_zero_downtime() 
{
    snow_config_t config = {
        .interface = "xxx",
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 0,
    };

    TEST_PRE_SETUP(config);
    snow_id_t snow_id;
    bool expected = snow_get_id(&snow_id);
    TEST_POST_SETUP();
    
    return (expected == false);
}

int main(void) 
{

    TEST_CHECK_RESULT("test_snow_get_id", test_snow_get_id());
    TEST_CHECK_RESULT("test_snow_get_id_as_binary", test_snow_get_id_as_binary());
    TEST_CHECK_RESULT("test_snow_get_id_for_garbage_interface",
     test_snow_get_id_for_unknown_interface());
    TEST_CHECK_RESULT("test_snow_get_id_as_binary_for_garbage_interface",
     test_snow_get_id_as_binary_for_unknown_interface());
    TEST_CHECK_RESULT("test_snow_get_id_for_zero_downtime",
     test_snow_get_id_for_zero_downtime());
    return EXIT_SUCCESS;
}