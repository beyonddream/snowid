# snowid

A Decentralized, K-Ordered Unique ID Generator library in C.

It generates 12b-bit k-ordered id's (time-ordered lexically). It can be run on each node in your
infrastructure and it will generate conflict-free id's on-demand without coordination.

## Design

#### UUID format (128 bits)

```
{timestamp:64, worker_id:48, seq: 16}
```

* `timestamp` - unix timestamp (millisecond timestamp resolution)
* `worker_id` - pulled from a network interface a 48-bit MAC address
* `seq` - incremented each time id is requested in the same millisecond. This means each server can produce 2^16 - 1 unique id's per millisecond without overflow.

## Credits

The idea/design is borrowed from BMC's (formerly Boundary) flake server written in Erlang.
More details are in the original [post](http://archive.is/2015.07.08-082503/http://www.boundary.com/blog/2012/01/flake-a-decentralized-k-ordered-unique-id-generator-in-erlang/).