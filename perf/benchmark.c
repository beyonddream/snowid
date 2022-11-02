#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "snowid.h"

static inline bool bench()
{
    snow_config_t config = {
        .interface = "en0",
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    snow_init(&config);

    unsigned char out[16] = {0};

    for(int i = 1; i <= 100000; i++) {

        if (snow_get_id_as_binary(out) == false) {
            puts("unable to generate snowid as binary");
            return false;
        }
        /* print the last byte as hex just so the compiler doesn't optimize `out` away. */
        printf("%x\n", out[15]);
    }

    snow_shutdown();

    return true;
}

int main(void) 
{
    if (bench() == false) {
        printf("calling benchmark failed...");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}