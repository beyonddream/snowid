/**
* 
* MIT License
* 
* Copyright (c) 2022 beyonddream

* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 

#include "snowid.h"

typedef struct snow_state {
    bool enabled;
    /**
     * `checkpoint` is last time when an id was generated.
     * If we detect current time is < checkpoint, then
     * clock has moved backward and we refuse to generate
     * the id.
     */
    uint64_t checkpoint;
    uint64_t worker_id;
    uint16_t sequence_id;
} snow_state_t;

/**
 * Global variable to store the state.
 * Client should use some form of mutex if multiple threads are going to access the API's.
 */
static snow_state_t *state;

#define DEFAULT_CHECKPOINT_FILE_PATH "/data/snowid/timestamp.out"

static bool get_current_ts(uint64_t *);
static uint64_t get_checkpoint_mutable(char *);
static uint64_t get_worker_id_from_nw_if(char *);

static uint64_t get_worker_id_from_nw_if(char *interface)
{

    (void)interface;

    return 0;
}

static uint64_t get_checkpoint_mutable(char *timestamp_path)
{
    
    if (timestamp_path == NULL) {
        timestamp_path = DEFAULT_CHECKPOINT_FILE_PATH;
    }

    FILE *file = fopen(timestamp_path, "r");
    uint64_t checkpoint = 0;

    if (file == NULL) {
        /* create a new file at timestamp_path */
        file = fopen(timestamp_path, "w");
        if (get_current_ts(&checkpoint) == false) {
            fprintf(stderr, "Couldn't read current timestamp.");
        }
        int ret = fwrite(&checkpoint, sizeof(checkpoint), 1, file);
        if (ret != 1) {
            fprintf(stderr, "Couldn't write to timestamp_path.");
        }
    } else {
        int ret = fread(&checkpoint, sizeof(checkpoint), 1, file);
        if (ret != 1) {
            fprintf(stderr, "Couldn't read from timestamp_path.");
        }
    }
    
    fclose(file);

    return checkpoint;
}

static bool get_current_ts(uint64_t *result)
{
    time_t t;

    t = time(NULL);

    if (t == (time_t)-1) {
        return false;
    }

    *result = (uint64_t)t;

    return true;
}

bool snow_get_id(snow_id_t *dest)
{
    
    if (state == NULL || state->enabled == false) {
        return false;
    }
        
    uint64_t current_time;
    
    if (get_current_ts(&current_time) == false) {
        return false;
    }

    if (state->checkpoint > current_time) {
        fprintf(stderr, "Clock is running backwards.");
        state->enabled = false;
        return false;
    }
    
    if (state->checkpoint == current_time) {
        state->sequence_id++;
    } else {
        state->sequence_id = 0;
    }

    state->checkpoint = current_time;

    snow_id_t current = {
        .timestamp = state->checkpoint,
        .worker_id = state->worker_id,
        .sequence_id = state->sequence_id
    };

    *dest = current;

    return true;
}

void snow_dump(FILE *stream)
{

    if (state == NULL) {
        return;
    }

    if (stream == NULL) {
        stream = stdout;
    }

#define FPRINTF(KEY, VALUE) \
        do {    \
            fprintf(stream, (KEY), (VALUE)); \
        } while(0);
    FPRINTF("%s", "\n{")
    FPRINTF("\"enabled\":%d,", state->enabled)
    FPRINTF("\"worker_id\":%llu,", state->worker_id)
    FPRINTF("\"checkpoint\":%llu,", state->checkpoint)
    FPRINTF("\"sequence_id\":%hu", state->sequence_id)
    FPRINTF("%s", "}\n")
#undef FPRINTF

    return;
}

void snow_init(snow_config_t *config)
{

    state = malloc(sizeof(snow_state_t));

    if (state == NULL) {
        fprintf(stderr, "malloc of snow_state_t failed.");
        return;
    }

    state->enabled = false;

    if (config == NULL) {
        fprintf(stderr, "snow config is NULL.");
        return;
    }

    /* get the current timestamp */
    uint64_t current_time;
    if (get_current_ts(&current_time) == false) {
        return;
    }

    uint64_t checkpoint = get_checkpoint_mutable(config->timestamp_path);
    uint64_t allowable_downtime = config->allowable_downtime;

    if (checkpoint > current_time) {
        fprintf(stderr, "Clock is running backwards, failing to generate id.");
        return;
    }

    if ((current_time - checkpoint) > allowable_downtime) {
        fprintf(stderr, "Clock is too far advanced, failing to generate id.");
        return;
    }

    state->checkpoint = checkpoint;
    state->worker_id = get_worker_id_from_nw_if(config->interface);
    state->sequence_id = 0;

    /* init has succeeded */
    state->enabled = true;

    return;
}

void snow_shutdown()
{
    free(state);
}