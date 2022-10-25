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
#include <netdb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

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
    int family;

    if (getifaddrs(&ifaddr) == -1) {
        perror("get_all_hw_ifs():Call to getifaddrs failed");
        return NULL;
    }

    const char hwaddr_fmt[] = "%02x:%02x:%02x:%02x:%02x:%02x";
    int hwaddr_fmt_sz = strlen(hwaddr_fmt) + 1;
    char hwaddr[hwaddr_fmt_sz];
    bool found = false;

    char *hwaddr_selected = malloc(sizeof(hwaddr)); 
    
    if (hwaddr_selected == NULL) {
        fprintf(stderr, "get_all_hw_ifs:Malloc failed.");
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

        #if defined(linux)
            struct sockaddr_ll *sock_addr = (struct sockaddr_ll*)ifa->ifa_addr;
            snprintf(hwaddr, hwaddr_fmt_sz, hwaddr_fmt
                , sock_addr->sll_addr[0], sock_addr->sll_addr[1]
                , sock_addr->sll_addr[2], sock_addr->sll_addr[3]
                , sock_addr->sll_addr[4], sock_addr->sll_addr[5]);
        #else
            struct sockaddr_dl *sock_addr = (struct sockaddr_dl*)ifa->ifa_addr;
            unsigned char *ptr = (unsigned char *)LLADDR(sock_addr);
            snprintf(hwaddr, hwaddr_fmt_sz, hwaddr_fmt
                , *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
        #endif

        memcpy(hwaddr_selected, hwaddr, sizeof(hwaddr));
        found = true;

        if (strncmp(ifa->ifa_name, interface, strlen(interface)) == 0) {
            break;
        }
    }

    if (found == false) {
        free(hwaddr_selected);
        return NULL;
    }

    return hwaddr_selected;
}