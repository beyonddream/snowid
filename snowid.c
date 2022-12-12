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
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include <inttypes.h>

#include "snowid.h"
#include "snowid_util.h"
#include "snowid_checkpoint.h"

/* private internal snowid state representation */
typedef struct snow_state {
    /* Id will be generated only if true */
    bool enabled;
    /**
     * `checkpoint` is last time when an id was generated.
     * If we detect current time is < checkpoint, then
     * clock has moved backward and we refuse to generate
     * the id.
     */
    uint64_t checkpoint;
    /* 48-bits MAC address */
    uint64_t worker_id;
    /* 16 bit increments within same millisecond */
    uint16_t sequence_id;
} snow_state_t;

/**
 * Global variable to store the state.
 * Client should use some form of mutex if multiple threads are going to access the API's.
 */
static snow_state_t *state;

#define DEFAULT_CHECKPOINT_FILE_PATH "/data/snowid/timestamp.out"

static bool get_checkpoint_mutable(uint64_t *out, char *timestamp_path);
static bool get_worker_id_from_nw_if(uint64_t *out, char *interface);
static bool get_snowid_to_binary(snow_id_binary_t out, const snow_id_t *snowid);

static bool get_worker_id_from_nw_if(uint64_t *workerid, char *interface)
{

    if (workerid == NULL) {
        return false;
    }

    return get_hw_addr_as_binary(workerid, interface);
}

static bool get_checkpoint_mutable(uint64_t *checkpoint, char *timestamp_path)
{
    
    if (checkpoint == NULL) {
        return false;
    }

    *checkpoint = 0;

    if (timestamp_path == NULL) {
        timestamp_path = DEFAULT_CHECKPOINT_FILE_PATH;
    }

    bool success = true;
    FILE *file = fopen(timestamp_path, "r");
    
    if (file == NULL) {
        /* create a new file at timestamp_path */
        file = fopen(timestamp_path, "w");
        if (file == NULL) {
            fprintf(stderr, "Couldn't open timestamp_path for write.\n");
            success = false;
        } else {
            if (get_current_ts(checkpoint) == false) {
                fprintf(stderr, "Couldn't read current timestamp.\n");
                success = false;
            }
            if (*checkpoint == 0) {
                fprintf(stderr, "Checkpoint value seem to be zero.\n");
                success = false;
            }
            int ret = fwrite(checkpoint, sizeof(uint64_t), 1, file);
            if (ret != 1) {
                fprintf(stderr, "Couldn't write to timestamp_path.\n");
                success = false;
            }
            fclose(file);
        }
    } else {
        /* read from the existing file at timestamp_path */
        int ret = fread(checkpoint, sizeof(uint64_t), 1, file);
        if (ret != 1) {
            fprintf(stderr, "Couldn't read from timestamp_path.\n");
            success = false;
        }
        if (*checkpoint == 0) {
            fprintf(stderr, "Checkpoint value seem to be zero.\n");
            success = false;
        }
        fclose(file);
    }
    
    return success;
}

bool snow_get_id(snow_id_t *dest)
{
    uint64_t current_time;

    if (dest == NULL) {
        return false;
    }

    if (state == NULL || state->enabled == false) {
        return false;
    }
    
    if (get_current_ts(&current_time) == false) {
        return false;
    }

    if (state->checkpoint > current_time) {
        fprintf(stderr, "Clock is running backwards.\n");
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

static bool get_snowid_to_binary(snow_id_binary_t out, const snow_id_t *snowid)
{
    int idx = 0;

    uint64_t timestamp   = snowid->timestamp;
    uint64_t workerid    = snowid->worker_id;
    uint16_t sequenceid  = snowid->sequence_id;

    /* convert the timestamp into 64 bits */
    for(int8_t i = 7; i >= 0; i--) {
        out[idx++] = (timestamp >> (CHAR_BIT * i)) & 0xff;
    }

    /* convert the worker id into 48 bits */
    for(int8_t i = 5; i >= 0; i--) {
        out[idx++] = (workerid >> (CHAR_BIT * i)) & 0xff;
    }

    /* convert the sequence id into 16 bits */
    for(int8_t i = 1; i >= 0; i--) {
        out[idx++] = (sequenceid >> (CHAR_BIT * i)) & 0xff;
    }

    return true;
}

bool snow_id_convert(snow_id_binary_t out, const snow_id_t *snowid) {

    if (out == NULL || snowid == NULL) {
        return false;
    }

    return get_snowid_to_binary(out, snowid);
}

bool snow_get_id_as_binary(snow_id_binary_t dest_as_bin)
{
    
    if (dest_as_bin == NULL) {
        return false;
    }

    snow_id_t dest;

    if (snow_get_id(&dest) == false) {
        return false;
    }

    return get_snowid_to_binary(dest_as_bin, &dest);
}

void snow_dump(FILE *stream)
{

    if (state == NULL) {
        return;
    }

    if (stream == NULL) {
        stream = stdout;
    }

#define LOG(KEY, VALUE)                  \
        do {                                 \
            fprintf(stream, (KEY), (VALUE)); \
        } while(0)
        
    LOG("%s", "\n{");
    LOG("\"enabled\":%d,", state->enabled);
    LOG("\"worker_id\":%" PRIu64 ",", state->worker_id);
    LOG("\"checkpoint\":%" PRIu64 ",", state->checkpoint);
    LOG("\"sequence_id\":%hu", state->sequence_id);
    LOG("%s", "}\n");
#undef LOG

    return;
}

void snow_init(snow_config_t *config)
{
    uint64_t current_time;
    uint64_t checkpoint;
    uint64_t allowable_downtime;

    state = malloc(sizeof(snow_state_t));

    if (state == NULL) {
        fprintf(stderr, "malloc of snow_state_t failed.\n");
        return;
    }

    state->enabled = false;
    
    if (config == NULL) {
        fprintf(stderr, "snow config is NULL.\n");
        return;
    }

    /* if timestamp_path is new, then checkpoint can be current timestamp */
    if (get_checkpoint_mutable(&checkpoint, config->timestamp_path) == false) {
        return;
    }

    /* get the current timestamp again now */
    if (get_current_ts(&current_time) == false) {
        return;
    }
    
    if (checkpoint > current_time) {
        fprintf(stderr, "Clock is running backwards, failing to generate id.\n");
        return;
    }
    
    allowable_downtime = config->allowable_downtime;
    
    if ((current_time - checkpoint) > allowable_downtime) {
        fprintf(stderr, "Clock is too far advanced, failing to generate id.\n");
        return;
    }
    
    state->checkpoint = current_time;
    state->sequence_id = 0;

    if (get_worker_id_from_nw_if(&state->worker_id, config->interface) == false) {
        return;
    }

    /* start a child process to periodically save current time in `timestamp_path`*/
    snow_checkpoint_start(config->timestamp_path);

    /* init has succeeded */
    state->enabled = true;

    return;
}

void snow_shutdown(void)
{

    free(state);
    state = NULL;

    return;
}