#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "snowid.h"

/* Driver code to show how snowid library can be used in a program */
int main(void) 
{
    puts("Test program for snowid...");

    snow_config_t config = {
        .interface = NULL,
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    snow_init(&config);

    snow_dump(NULL);

    snow_id_t snow_id;
    unsigned char out[16] = {0};

    for(int i = 1; i <= 1000; i++) {
        if (snow_get_id(&snow_id) == false) {
            puts("unable to generate snowid");
            break;
        }

        if (snow_get_id_as_binary(out) == false) {
            puts("unable to generate snowid as binary");
            break;
        }

        for (int8_t i = 0; i < 16; i++) {
            printf("%x", out[i]);
            if (i != 15) {
                printf(":");
            }
        }
        printf("\n");

        if (i == 500) {
            puts("sleeping for 4 sec...");
            sleep(4);
        }
    }

    snow_dump(NULL);

    snow_shutdown();
    
    return EXIT_SUCCESS;
}