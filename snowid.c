
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>

#include "snowid.h"

typedef struct snow_state {
    /**
     * `checkpoint` is last time when an id was generated.
     * If we detect current time is < checkpoint, then
     * clock has moved backward and we refuse to generate
     * the id.
     */
    struct timeval *checkpoint;
    uint64_t worker_id;
    uint16_t sequence_id;
} snow_state;

typedef struct snow_id {
    uint64_t timestamp: 64;
    uint64_t worker_id: 48;
    uint16_t sequence_id: 16;
} snow_id;

static void worker_id_init(void);

snow_id get_id(void)
{

    snow_id current = {
        .timestamp = 0,
        .worker_id = 0,
        .sequence_id = 0
    };

    return current;
}

void init(snow_config *config)
{

    if (config == NULL) {
        fprintf(stderr, "snow config is NULL.");
        return;
    }



}

void shutdown()
{

}