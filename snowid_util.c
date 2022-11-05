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
#include <ifaddrs.h>
#include <limits.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "snowid_util.h"

#ifdef __linux__
    #include <linux/if_link.h>
    #include <linux/if_packet.h>
    #define IF_HW_FAMILY AF_PACKET
#else
    #include <net/if_dl.h>
    #define IF_HW_FAMILY AF_LINK   /* MacOS/BSD */
#endif

bool get_hw_addr_as_binary(uint64_t *workerid, char *interface)
{
    struct ifaddrs *ifaddr;
    int family;
    bool found = false;

    if (getifaddrs(&ifaddr) == -1) {
        perror("get_all_hw_ifs():Call to getifaddrs failed");
        goto fail;
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) {
            continue;
        }

        family = ifa->ifa_addr->sa_family;
        
        /* skip if family is not link layer type */
        if (family != IF_HW_FAMILY) {
            continue;
        }

        /* skip loopback - anything starting with lo */
        if ((ifa->ifa_name == NULL) || strncmp(ifa->ifa_name, "lo", 2) == 0) {
            continue;
        }

        #ifdef __linux__
            struct sockaddr_ll *sock_addr = (struct sockaddr_ll*)ifa->ifa_addr;
            for (int8_t i = 5; i >=0; --i) {
                *workerid |= (uint64_t)sock_addr->sll_addr[i] << (CHAR_BIT * i);
            }
        #else
            struct sockaddr_dl *sock_addr = (struct sockaddr_dl*)ifa->ifa_addr;
            unsigned char *ptr = (unsigned char *)LLADDR(sock_addr);
            for (int8_t i = 5; i >= 0; --i) {
                *workerid |= (uint64_t)*ptr++ << (CHAR_BIT * i);
            }
        #endif

        found = true;
        
        if (interface != NULL && strncmp(ifa->ifa_name, interface, strlen(interface) + 1) == 0) {
            break;
        }
    }

    freeifaddrs(ifaddr);
    
fail:
    return found;
}

bool get_current_ts(uint64_t *result)
{
    struct timeval now;

    if (result == NULL) {
        return false;
    }

    if (gettimeofday(&now, NULL) == -1) {
        return false;
    }

    *result = (uint64_t)((now.tv_sec * 1000) + (now.tv_usec / 1000));

    return true;
}