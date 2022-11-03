# snowid

A Decentralized, K-Ordered Unique ID Generator library in C.

It generates 128-bit k-ordered id's (time-ordered lexically). It can be run on each node in your
infrastructure and it will generate conflict-free id's on-demand without coordination. Here, k-ordered means that id's generated within a given millisecond will be within a millisecond of one another in the id space as well.

## Design

#### UUID format (128 bits)

```
{timestamp:64, worker_id:48, seq: 16}
```

* `timestamp` - unix timestamp (millisecond timestamp resolution)
* `worker_id` - pulled from a network interface a 48-bit MAC address
* `seq` - incremented each time id is requested in the same millisecond. This means each server can produce 2^16 - 1 unique id's per millisecond without overflow.

#### Config

`struct snow_config` takes below fields:

* `interface` - Set to available network interface to pull a 48-bit mac address as worker id.
* `timestamp_path` - To periodically save the current time. When snow_init() is called and if it detects
that this file contains timestamp in the future or distant past then it refuses to generate id. This prevents problematic id's (that might break time-orderness) from getting distributed.
* `allowable_downtime` - Safeguard to prevent id generation if it detects it hasn't been run in a long time
since this might be an indication that the system clock has been skewed far into the future.

Example:
```
interface="en0"
timestamp_path="/data/snowid/timestamp.out"
allowable_downtime=1665373570
```

## API/Usage

Two main functions to generate id are provided:

A)
```c
/*
* Attempts to set `dest_as_binary` with a 128-bit id (represented as byte 
* array). Return true if operation succeeds. 
*/
bool snow_get_id_as_binary(snow_id_binary_t snowid_as_bin);
```
B)
```c
/*
* Attempts to set `dest` with a `struct snow_id` that consists of individual 
* components of the id. Return true if operation succeeds.
*/
bool snow_get_id(snow_id_t *snowid);
```

The third function to convert A) from B).
```c
/*
* Convenience function that attempts to convert `snowid` of type `struct 
* snow_id` into binary `dest_as_bin`. Return true if operation succeeds.
*/
bool snow_id_convert(snow_id_binary_t snowid_as_bin, const snow_id_t *snowid);
```

For full usage of reset of the public API's (`snowid.h`), checkout `main.c` that provides a driver program on how to use the library from an application (e.g. a (web)service).

## Dependencies

* Tested on macOS and GNU/Linux (via Docker). PR welcome to make it work on Windows.
* C99 compatible C compiler (e.g. GCC or Clang).
* This library doesn't depend on any external libraries.  

## Build/Test/Benchmark

#### Build and Run

```
$ make
```
This will create a binary by the name `snowid` which is an example driver program and run it like this:
```
$ ./snowid
```
and it should print a series of time ordered unique id's.

#### Test

```
$ make test
```
This should run a series of unit tests to verify the basic functionality of the library.

Debug mode build and test can be enabled like below:

```
$ DEBUGBUILD=true make test
```

#### Benchmark

Timing benchmark below on how long it takes to generate 100,000 unique ids on my MacBook 2.2 GHz 6-Core Intel Core i7 (note this may vary based on your machine):

```
$ time make bench

real    0m0.613s
user    0m0.089s
sys     0m0.066s
```

If you have python3 in your system, then u can run it like below:

```
$ python3 -m timeit -n 3 "__import__('os').system('make bench')"
...
3 loops, best of 5: 509 msec per loop
```

#### Docker build

There is a Dockerfile which provides a way to build the code in GNU/Linux.

```
1) docker build -t snowid -f Dockerfile .
2) docker run -it --rm snowid
```

#### Docker test

```
1) docker build -t unit -f Dockerfile.test .
2) docker run -it --rm unit
```

#### Docker bench

```
1) docker build -t benchmark -f Dockerfile.bench .
2) docker run -it --rm benchmark
```

## Credits

* General inspiration is from Twitter snowflake [id generator](https://blog.twitter.com/engineering/en_us/a/2010/announcing-snowflake).

* The idea/design of `snowid` is borrowed from BMC's (formerly Boundary) flake server written in Erlang.
More details are in the original [post](http://archive.is/2015.07.08-082503/http://www.boundary.com/blog/2012/01/flake-a-decentralized-k-ordered-unique-id-generator-in-erlang/).


## License

MIT License
Copyright (c) 2022 beyonddream