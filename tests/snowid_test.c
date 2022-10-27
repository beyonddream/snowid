#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "snowid.h"

int main(void) {
    
    puts("Test program for snowid...");

    snow_config_t config = {
        .interface = NULL,
        .timestamp_path = "./timestamp.out",
        .allowable_downtime = 2592000000,
    };

    snow_init(&config);

    snow_dump(NULL);

    snow_id_t snow_id;
    for(int i = 1; i <= 1000; i++) {
        if (snow_get_id(&snow_id) == false) {
            printf("unable to generate snowid");
            break;
        }
        snow_dump(NULL);
        if (i == 500) {
            sleep(2);
        }
    }

    snow_shutdown();

    return EXIT_SUCCESS;
}