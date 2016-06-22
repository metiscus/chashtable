#include "htable.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct HashBucket
{
    uint32_t hash;
    char orig[HashMaxStringLength];
    void *data;
};

struct HashTable
{
    uint32_t size;
    struct HashBucket *buckets;
};

static uint32_t hash_string(const char* str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int hashtable_init(struct HashTable** pptr, uint32_t size)
{
    assert(pptr);
    assert(size > 1);
    *pptr = (struct HashTable*)malloc(sizeof(struct HashTable));
    struct HashTable* ptr = *pptr;

    printf("hashtable_init: memory used: %lu\n", sizeof(struct HashBucket) * size + sizeof(struct HashTable));

    if(ptr)
    {
        ptr->size = size;
        ptr->buckets = (struct HashBucket*)calloc(size, sizeof(struct HashBucket) * size);
        return 1;
    }
    
    return 0;
}

void hashtable_free(struct HashTable** ptr)
{
    if(ptr && *ptr)
    {
        struct HashTable* table = *ptr;
        free(table->buckets);
    }
}

int hashtable_insert(struct HashTable* ptr, const char* string, const void* value)
{
    assert(ptr && string);
    uint32_t hash = hash_string(string);
    uint32_t bucket_slot = hash % ptr->size;
    
    //printf("hashtable_insert: %s => %u [%u]\n", string, hash, bucket_slot);
    
    uint32_t max_retry = ptr->size;
    while(*ptr->buckets[bucket_slot].orig != '\0' && max_retry > 0)
    {
        max_retry--;
        bucket_slot = (bucket_slot + 1) % ptr->size; // linear probing
    }
    
    if(*ptr->buckets[bucket_slot].orig == '\0')
    {
        strncpy(ptr->buckets[bucket_slot].orig, string, HashMaxStringLength);
        ptr->buckets[bucket_slot].hash = hash;
        ptr->buckets[bucket_slot].data = (void*)value;
        return 1;
    }
    else
    {
        return 0;
    }
}

void* hashtable_lookup(struct HashTable* ptr, const char* string)
{
    assert(ptr && string);
    uint32_t hash = hash_string(string);
    uint32_t bucket_slot = hash % ptr->size;
    
    uint32_t max_retry = ptr->size;
    while(max_retry-- > 0)
    {
        struct HashBucket *bucket = &ptr->buckets[bucket_slot];
        if(strncmp(bucket->orig, string, HashMaxStringLength) == 0)
        {
            return bucket->data;
        }
    }
    return 0x0;
}
