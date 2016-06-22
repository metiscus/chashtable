#pragma once

#include <stdint.h>

struct HashTable;

#define HashMaxStringLength 16
// Note: that strings that are less than HashMaxStringLength will be considered
// equal if they collide

int   hashtable_init(struct HashTable** ptr, uint32_t size);
void  hashtable_free(struct HashTable** ptr);
int   hashtable_insert(struct HashTable* ptr, const char* string, const void* value);
void* hashtable_lookup(struct HashTable* ptr, const char* string);
