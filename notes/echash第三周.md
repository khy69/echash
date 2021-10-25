# echash第三周

EChash

~~~c
struct ECHash_st
{
    uint32_t total_num_server;
    struct memcached_ring_st rings[RING_SIZE]; //Hash Ring
    uint32_t chunk_list_size;
    struct chunk_st *chunk_list; //Chunk Index

    uint32_t stripe_list_size;
    struct chunk_info_st (*stripe_list)[RING_SIZE]; //Stripe Index

    struct encode_st encode;
    struct balance_st *balance_arr;
};
~~~

### memcached

预申请大内存，按预定的大小划分->不灵活->浪费内存

Memcached is an in-memory key-value store for **small arbitrary data** (strings, objects) from **results of database** calls, API calls, or page rendering.



add **standard document** in the source of function

Run_balance:

~~~c++
static void run_balance(struct ECHash_st *ptr)
{
    uint32_t arr_index = 0;
    for(uint32_t ring_index = 0; ring_index < RING_SIZE; ring_index++)
    {
        for(uint32_t pointer_index = 0; pointer_index < RING_VIRTUAL; pointer_index++)
        {
            char sort_ring[100] = {0};
            uint32_t sort_ring_len = 0;
            //write (which ring,which bucket) to sort_ring,return strlen(string)
            sort_ring_len = sprintf(sort_ring, "%s:%u-%u", "Ring", ring_index, pointer_index);
            uint32_t value = hashkit_digest(&(ptr->rings[0].ring->hashkit), sort_ring, (size_t)sort_ring_len);
            ptr->balance_arr[arr_index].index = ring_index;
            ptr->balance_arr[arr_index++].value = value;

        }
    }
~~~

## Io.cpp

multiple threads：no use

