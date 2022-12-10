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
#ifndef __SNOWID_UTIL_H__
#define __SNOWID_UTIL_H__

#include <stdbool.h>
#include <stdint.h>

/**
 * Retrive a binary representation of the H/W (MAC) address either from the interface name
 * but if the interface name is not present then one of the H/W address from any of the
 * interfaces except loopback.
 * 
 * @param workerid Binary representation of the mac address of the interface.
 * @param interface Interface name to pull the address from.
 * @return bool - true if we could return hw address, else false.
 */
bool get_hw_addr_as_binary(uint64_t *workerid, char *interface);

/**
 * Get current timestamp
 * 
 * @param result - set the current timestamp to the value pointed by result.
 * @return bool - true if we can get current timestamp, else false.
*/
bool get_current_ts(uint64_t *result);

#endif /* __SNOWID_UTIL_H__ */