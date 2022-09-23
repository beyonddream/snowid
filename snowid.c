
#include <sys/time.h>
#include <stdint.h>

#include "snowid.h"

typedef struct snow_state {
    /**
     * `checkpoint` is last time when an id was generated.
     * If we detect current time is < checkpoint, then
     * clock has moved backward and we refuse to generate
     * the id.
     */
    struct timeval *checkpoint;
    uint64_t worker_id;
    uint16_t sequence_id;
} snow_state;


void init(snow_config *config)
{
    (void)config;

}

void shutdown()
{

}