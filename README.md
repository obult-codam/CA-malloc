This project is made for learning purposes.

## Goals

The goals of the project are to recreate malloc in a modular way.
There is no sense in creating a slower version of malloc but it is also quite hard to beat.
By working with a modular aproach you can switch out strategies for each project.
This way you can choose if you want to use a time or space efficient allocator.

## Achieving modularity

In order to create a completely modular allocator I had to choose where to devide the responsibilities.
For me it felt natural to completely separate the general manager, later refered to as **manager**, which keeps track of the various zones we have allocated using mmap();
And the internal manager, later reffered to as **strategy**, which keeps track of individual allocations in the zone.
In this project they are completely separated which means that the manager has no idea what is going on inside the zones and the strategy has no knowledge of anything happening outside of its zone.

### Quick strategy overview

- linked, the allocations are strung together by a linked list
the nodes of the linked list are inbetween allocations returned to the client.
This creates a rist but is the most space effecient method, while maintaining data about the allocations.

- array, the allocations are located in predefined blocks. There is a separate array
which keeps track of the size used in each block. This is faster but less space efficient.

- multi_cell, is a more sophisticated implementation of array. A single allocation
is allowed to use up multiple cells to make the strategy more space efficient.

- yolo, is created to show how far you can go. Extremely space efficient but does not
retain enough data to free single allocations.

### Quick manager overview

- core, regular implementation using linked list to string together the zones.

- tls, like core but the global list head is *thread local storage* meaning
each thread uses its own zones. The allocations need to be freed in their own thread.

### Large allocations

Large allocations are given their own zone and when freed the zone is also dealocated with them.
They are quite boring, if you would want to squice in more large alloctions int he zone the linked strategy should have been used.

## Testing

The tests had to be created to test all the strategies available,
this makes for a messy makefile.

There is coverage available in ./tests/temp/index.html after making and running the tests.

``` bash
# running test-all auto builds, runs and makes coverage.
cd tests
make tests-all
open temp
# Open the index with a browser

# Other test run options
make full-<module_to_test>
# example:
make full-array

# You can also make a single unit test, but need ot run it yourself
# and then make coverage.
# example:
make yolo.tst
./yolo.tst
make coverage
```

## Using this allocator in your project.

The allocator can replace a regular malloc in a project.
1. Compile this project
2. compile your project with this library
```bash
# link the library:
# Add the library to your compilation.
```
3. set the DYDL_FALLBACK_LIBRARY_PATH=[libft_malloc.so directory]
4. Run your project

## Extra notes

- The global *g_head* in the core manager is only visible in each program instance.
This is to prevent two programs from accessing the same zone list while the
library is shared in memory.

## Malloc debug variables

- set MALLOCDEBUG for a safer malloc which will not fail when a pointer is freed which has not been allocated.