#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define PRINT_SNOWID_AS_BINARY(desc, snowid)\
    do {                                    \
        printf("%s", desc);               \
        for(int8_t i = 0; i < 16; i++) {    \
            printf("%x", snowid[i]);        \
            if (i != 15) {                  \
                printf(":");                \
            } else {                        \
                printf("\n");               \
            }                               \
        }                                   \
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
    unsigned char out[16] = {0};
    bool expected = snow_get_id_as_binary(out);
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
    unsigned char out[16] = {0};
    bool expected = snow_get_id_as_binary(out);
    TEST_POST_SETUP();
    
    return expected;
}

bool test_snow_get_id_as_binary_multiple() 
{
    snow_config_t config = {
        .interface = "eno",
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    TEST_PRE_SETUP(config);

#define NO_OF_IDS 5

    snow_id_t arr_out[NO_OF_IDS] = {{0}, {0}, {0}, {0}, {0}};
    snow_id_t out;
    bool expected;
    for (int8_t i = 0; i < NO_OF_IDS; i++) {
        expected = snow_get_id(&out);
        if (expected == false) {
            goto fail;
        }
        arr_out[i] = (snow_id_t) {
            .timestamp = out.timestamp,
            .worker_id = out.worker_id,
            .sequence_id = out.sequence_id
        };
    }

    int8_t first, second;
    first = 0;
    second = 1;
    while (first < NO_OF_IDS && second < NO_OF_IDS) {
        snow_id_t f = arr_out[first];
        snow_id_t s = arr_out[second];
        /* verify ordering */
        if ((f.timestamp > s.timestamp) 
            || (f.worker_id != s.worker_id)
            || (f.sequence_id > s.sequence_id)) {
            expected = false;
            goto fail;
        } 

        ++first;
        ++second;
    }

fail:
    
    TEST_POST_SETUP();
    
    return expected;
}

int main(void) 
{
    TEST_CHECK_RESULT("test_snow_get_id", test_snow_get_id());
    TEST_CHECK_RESULT("test_snow_get_id_as_binary", test_snow_get_id_as_binary());
    TEST_CHECK_RESULT("test_snow_get_id_for_garbage_interface",
     test_snow_get_id_for_unknown_interface());
    TEST_CHECK_RESULT("test_snow_get_id_as_binary_for_garbage_interface",
     test_snow_get_id_as_binary_for_unknown_interface());
    TEST_CHECK_RESULT("test_snow_get_id_as_binary_multiple",
     test_snow_get_id_as_binary_multiple());

    return EXIT_SUCCESS;
}