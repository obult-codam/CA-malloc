[x] allign memory in
    [x] multi_cell
    [x] array

[x] separate large allocs into their own zone.

[x] fix show alloc mem per zone type.

[x] thread save bonus
    _thread local storage? (needs a readme mention too!)

[x] fix normal show_alloc_mem
    [x] calculate total allocation size
     x  -> create api for zone which returns total size used.
[x] debug info bonus (show_alloc_mem_ex())

[x] project info
    [x] readme
        [x] usage
        [x] testing
        [x] inner workings
            [x] new_api and its modularity
            [x] pthread safety
            [x] global visibility

Do i defragment the memory enough?
    -> it is waay better than yolo..