#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "snowid.h"

int main(void) {
    
    puts("Test program for snowid...");

    snow_config_t config = {
        .interface = "en0",
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    snow_init(&config);

    snow_dump(NULL);

    snow_id_t snow_id;
    unsigned char out[16] = {0};

    for(int i = 1; i <= 1000; i++) {
        if (snow_get_id(&snow_id) == false) {
            printf("unable to generate snowid\n");
            break;
        }

        get_snowid_as_binary(out, &snow_id);
        for (int8_t i = 0; i < 16; i++) {
            printf("%x", out[i]);
            if (i != 15) {
                printf(":");
            }
        }
        printf("\n");

        if (i == 500) {
            printf("sleeping for 4 sec...\n");
            sleep(4);
        }
    }

    snow_dump(NULL);

    snow_shutdown();

    return EXIT_SUCCESS;
}