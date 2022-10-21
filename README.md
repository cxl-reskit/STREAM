
# STREAM benchmark

This version has been modified by Micron to allow testing specific
memory.  This benchmark operates on three arrays of double.  In the
original benchmark, the arrays are declared as global.

This hack of the benchmark changes the following:

* The arrays (a, b, c) are mmapped from an appropriate source, rather than
  globally declared
  * if `-d device` is supplied, memory is mapped from device
  * if `-o offset` is supplied, memory is mapped from offset on device
* The size of the arrays can be passed in from the command line:
  * if `-a <n_elements>` is supplied, the arrays will have n_elements
* getopt_long is used to harvest the command line.
* Default behavior is "the same" as the baseline benchmark.
  (default n_elem is 10000000)

## Building

```text
make
```

## Bugs

The current version calculates the size to be mapped (allocated) based
on the size of the arrays (rounded for each to start on a page boundary,
and with the overall size rounded to a 2MiB boundary to be
dev-dax-compatible).

But it does not check to see whether the resulting size fits within the
size of the memory being mapped.

This only applies if you're specifying a device to map from (/dev/mem or
/dev/dax).

If the size is exceeded, one would hope that the mmap will fail (but this
has not been tested as of now).

NOTE: the default size uses a bit less than 256MiB of memory, so if you
have more than that you should be good.

## Running stream

#### Run against regular memory

```text
./stream
```

#### Run against devdax memory

```text
# grep dax /proc/iomem
    880000000-107fffffff : dax0.0

# ./stream -d /dev/dax0.0
```

#### Run against devdax memory with a small array

```text
# grep dax /proc/iomem
    880000000-107fffffff : dax0.0

# ./stream -a 100 -d /dev/dax0.0
```

#### Run against /dev/mem at a specific offset


```text
# ./stream -a 100 -d /dev/mem -o 0x880000000
```
