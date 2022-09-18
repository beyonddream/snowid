#ifndef __SNOWID_H__
#define __SNOWID_H__

typedef struct snow_config {
    char *interface; /* network interface name */
    char *timestamp_path; /* file location to save current time periodically */
    unsigned long allowable_downtime; /* time since snowid is called last */
} snow_config;


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