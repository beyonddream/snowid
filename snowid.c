
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "snowid.h"

typedef struct snow_state {
    bool enabled;
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

static void worker_id_init(void);

/**
 * Global variable to store the state.
 * Client should use some form of mutex if multiple threads are going to access the API's.
 */
static snow_state state;

bool get_id(snow_id *dest)
{
    bool success;

    snow_id current = {
        .timestamp = 0,
        .worker_id = 0,
        .sequence_id = 0
    };

    if (state.enabled == true) {
        *dest = current;
        success = true;
    }

    return success;
}

void init(snow_config *config)
{

    if (config == NULL) {
        fprintf(stderr, "snow config is NULL.");
        state.enabled = false;
        return;
    }

    state.enabled = true;
    


}

void shutdown()
{

}