#include <stdio.h>
#include <stdlib.h>

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
    
    snow_shutdown();

    return EXIT_SUCCESS;
}