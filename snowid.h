#ifndef SNOWID_H
#define SNOWID_H

struct snow_config;


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


#endif /* SNOWID_H */