These pomodori are crafted to fix the streategies to work with the new api.


[x] checkup on api and make pomodori for linked list fix
[x] create fn on api for zone_internal_size(size_t amount, size_t max_size)
this does the full calculation for size required in the zone with the strategy selected.
[x] Figure out option for overflow checking on zone_internal_size (dont)
[] add zone_internal_size in the zone management implementation and deprecate alloc_header_size

### Linked-alloc
[x] Fix zone_internal_size
[x] Fix setup_zone
[x] make up mind about linked list type
    (for now stick to the old libft implementation for linked lists.)
[] Fix create_alloc
[] Fix cleanup_alloc
[] Fix zone_is_empty
[] Fix resize_alloc
[] Fix print_info

### Array-alloc
[] Fix zone_internal_size
[] Fix setup_zone
[] Fix create_alloc
[] Fix cleanup_alloc
[] Fix zone_is_empty
[] Fix resize_alloc
[] Fix print_info

### Array-plusplus-alloc
[] implement zone_internal_size
[] implement setup_zone
[] implement create_alloc
[] implement cleanup_alloc
[] implement zone_is_empty
[] implement resize_alloc
[] implement print_info
