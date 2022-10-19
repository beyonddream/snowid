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

#include <stdio.h>
#include <stdbool.h>

typedef struct snow_config {
    char *interface; /* network interface name */
    char *timestamp_path; /* file location to save current time periodically */
    uint64_t allowable_downtime; /* time since snowid is called last - default 0 */
} snow_config_t;

typedef struct snow_id {
    uint64_t timestamp: 64;
    uint64_t worker_id: 48;
    uint16_t sequence_id: 16;
} snow_id_t;

/**
 * Generates unique 128-bit id from current timestamp,worker_id,sequence_id.
 * sequence_id is incremented as many times as the function is called within the
 * same timestamp.
 * 
 * @arg id - If able to generate an id, set the value of id to `snow_id`.
 * @return true if successfully able to generate an id, false if not.
 */
bool snow_get_id(snow_id_t *id);

/**
 * Dump snow state and config to stdout for debugging.
 * @arg stream - any file descriptor (stdout if NULL)
 */ 
void snow_dump(FILE *stream);

/**
 * Initializes the snowid engine with the config.
 * 
 * @arg config - The snowid configuration
 * @return void
 */
void snow_init(snow_config_t *config);

/**
 * Deinitializes the snowid engine
 * 
 * @arg void
 * @return void
 */
void snow_shutdown(void);


#endif /* __SNOWID_H__ */