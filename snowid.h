#ifndef __SNOWID_H__
#define __SNOWID_H__

typedef struct snow_config {
    char *interface; /* network interface name */
    char *timestamp_path; /* file location to save current time periodically */
    unsigned long allowable_downtime; /* time since snowid is called last */
} snow_config;

typedef struct snow_id {
    uint64_t timestamp: 64;
    uint64_t worker_id: 48;
    uint16_t sequence_id: 16;
} snow_id;

/**
 * Generates unique 128-bit id from current timestamp,worker_id,sequence_id.
 * sequence_id is incremented as many times as the function is called within the
 * same timestamp.
 * 
 * @arg id - If able to generate an id, set the value of id to `snow_id`ref. Else, NULL.
 * @return true if successfully able to generate an id, false if not.
 */
bool get_id(snow_id **id);

/**
 * Initializes the snowid engine with the config.
 * 
 * @arg config - The snowid configuration
 * @return void
 */
void init(snow_config *config);

/**
 * Deinitializes the snowid engine
 * 
 * @arg void
 * @return void
 */
void shutdown(void);


#endif /* __SNOWID_H__ */