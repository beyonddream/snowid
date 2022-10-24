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
#include <ifaddrs.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#if defined(linux)
    #include <linux/if_link.h>
    #define IF_HW_FAMILY AF_PACKET
#else 
    #include <net/if_dl.h>
    #define IF_HW_FAMILY AF_LINK   /* MacOS/BSD */
#endif

#include "snowid_util.h"

char *get_all_hw_ifs(char *interface)
{
    struct ifaddrs *ifaddr;
    char host[NI_MAXHOST];
    int family;

    (void)interface;
    (void)host;

    if (getifaddrs(&ifaddr) == -1) {
        perror("get_all_hw_ifs():Call to getifaddrs failed");
        return NULL;
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) {
            continue;
        }

        family = ifa->ifa_addr->sa_family;
        
        /* skip if socket doesn't support IPv6 */
        if (family != IF_HW_FAMILY) {
            continue;
        }

        /* skip loopback */
        if ((ifa->ifa_name == NULL) || strncmp(ifa->ifa_name, "lo", 2) == 0) {
            continue;
        }
    }

    return NULL;
}