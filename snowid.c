
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

/**
 * Global variable to store the state.
 * Client should use some form of mutex if multiple threads are going to access the API's.
 */
static snow_state state;

static void worker_id_init(void);
static bool get_current_ts(uint64_t **);

static bool get_current_ts(uint64_t **result)
{
    time_t t;

    if (result == NULL) {
        return false;
    }

    t = time(*result);

    if (t == (time_t)-1) {
        *result = NULL;
        return false;
    }

    return true;
}

bool get_id(snow_id **dest)
{
    
    if (dest == NULL) {
        return false;
    }

    *dest = NULL;

    if (state.enabled == true) {
        
        uint64_t *timestamp = NULL;
        bool res = get_current_ts(&timestamp);

        if (res == false) {
            return false;
        }

        if (*timestamp == state.checkpoint) {
            state.sequence_id++;
        } else {
            state.sequence_id = 0;
        }

        snow_id current = {
            .timestamp = *timestamp,
            .worker_id = state.worker_id,
            .sequence_id = state.sequence_id
        };

        *dest = &current;

        return true;
    }

    return false;
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