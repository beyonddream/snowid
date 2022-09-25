#ifndef __SNOWID_H__
#define __SNOWID_H__

struct snow_id;

typedef struct snow_config {
    char *interface; /* network interface name */
    char *timestamp_path; /* file location to save current time periodically */
    unsigned long allowable_downtime; /* time since snowid is called last */
} snow_config;

/**
 * Generates unique 128-bit id from current timestamp,worker_id,sequence_id.
 * sequence_id is incremented as many times as the function is called within the
 * same timestamp.
 * 
 * @return snow_id - 128 bit unique id.
 */
struct snow_id get_id(void);

/**
 * Initializes the snowid engine with the config.
 * 
 * @arg config The snowid configuration
 * @return void on success, exit() on failure.
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