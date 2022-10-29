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
#include <stdio.h>
#include <unistd.h>

#include "snowid_checkpoint.h"

static void snow_checkpoint_periodic(char *timestamp_path);

static void snow_checkpoint_periodic(char *timestamp_path)
{

    (void)timestamp_path;

    return;
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