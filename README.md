# snowid
A Decentralized, K-Ordered Unique ID Generator library in C


## Design

#### UUID format (128 bits)

```
{timestamp:64, worker_id:48, seq: 16}
```

* `timestamp` - unix timestamp (millisecond timestamp resolution)
* `worker_id` - pulled from a network interface a 48-bit MAC address
* `seq` - incremented each time id is requested in the same millisecond. This means each server can produce 2^16 - 1 unique id's per millisecond without overflow.
