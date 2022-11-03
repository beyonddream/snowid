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
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "snowid_checkpoint.h"
#include "snowid_util.h"

#define CHECKPOINT_PERIOD_SECS 2

static void snow_checkpoint_periodic(char *timestamp_path);

static void snow_checkpoint_periodic(char *timestamp_path)
{
    int fd;
    uint64_t buf[1];

    if ((fd = open(timestamp_path, O_WRONLY)) == -1) {
        _exit(1);
    }

    uint64_t checkpoint;

    for(;;) {
        /* check if child is reparented to init(1), if so then exit. */
        /* XXX - may not guarenteed to work if reparent is not pid == 1 */
        if (getppid() == 1) {
            _exit(0);
        }

        if (get_current_ts(&checkpoint) == false) {
            continue;
        }

        buf[0] = checkpoint;

        /* XXX - ignore lseek() errors - it may recover during next try (: */
        if (lseek(fd, 0, SEEK_SET) == -1) {
            continue;
        }

        /* XXX - ignore write() errors - it may recover during next try (: */
        if (write(fd, buf, 1) != -1) {
            /* XXX - ignore fsync() errors - it may recover during next try (: */
            fsync(fd);
        }

        sleep(CHECKPOINT_PERIOD_SECS);
    }

    _exit(1);
}

bool snow_checkpoint_start(char *timestamp_path)
{
    
    if (timestamp_path == NULL) {
        fprintf(stderr, "snow_checkpoint_start():timestamp_path is NULL.");
        return false;
    }
    
    if (access(timestamp_path, W_OK) != 0) {
        perror("snow_checkpoint_start():error while checking access to `timestamp_path`.");
        return false;
    }

    int pid = fork();

    if (pid < 0) {
        perror("snow_checkpoint_start():Call to fork failed.");
        return false;
    }

    /* child */
    if (pid == 0) {
        snow_checkpoint_periodic(timestamp_path);
    }

    return true;
}