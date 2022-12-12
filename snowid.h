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
#ifndef __SNOWID_H__
#define __SNOWID_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct snow_config {
    char *interface; /* network interface name */
    char *timestamp_path; /* file location to save current time periodically */
    uint64_t allowable_downtime; /* time since snowid is called last - default 0 */
} snow_config_t;

/* public type that represents a snowid at any given time */
struct snow_id {
    uint64_t timestamp;
    uint64_t worker_id;
    uint16_t sequence_id;
};

typedef struct snow_id snow_id_t;

typedef unsigned char snow_id_binary_t[16];

/**
 * Generates unique 128-bit id from current timestamp,worker_id,sequence_id.
 * sequence_id is incremented as many times as the function is called within the
 * same timestamp.
 * 
 * @warning Not multi-thread safe
 * @param snowid - If able to generate an id, set the value of id to `snow_id`.
 * @return bool - true if successfully able to generate an id, false if not.
 */
bool snow_get_id(snow_id_t *snowid);

/**
 * Dump snow state and config to stdout for debugging.
 * 
 * @warning Not multi-thread safe
 * @param stream - any file descriptor (stdout if NULL)
 * @return void
 */ 
void snow_dump(FILE *stream);

/**
 * Initializes the snowid engine with the config. It has to be called only once before
 * calling any other API functions.
 * 
 * @warning Not multi-thread safe
 * @param config - The snowid configuration
 * @return void
 */
void snow_init(snow_config_t *config);

/**
 * Deinitializes the snowid engine. It has to be called only once at the end
 * and no other API functions should be called after it except snow_init().
 * 
 * @warning Not multi-thread safe
 * @param void
 * @return void
 */
void snow_shutdown(void);

/**
 * Generate a new snow_id as binary - unsigned char[16] representing 128 bits.
 * Represented as:
 * <unix timestamp (64 bits) + MAC address (48 bits) + sequence number (16 bits)>
 * 
 * @param snowid_as_bin - set to snowid as binary
 * @return bool - true if success, else false.
*/
bool snow_get_id_as_binary(snow_id_binary_t snowid_as_bin);

/**
 * Convert an already generated snow_id as binary (represented as byte array). 
 * 
 * @param snowid_as_bin - set the converted snowid as binary
 * @param snowid - pass in already generated snowid
 * @return bool - true if success, else false.
*/
bool snow_id_convert(snow_id_binary_t snowid_as_bin, const snow_id_t *snowid);

#endif /* __SNOWID_H__ */