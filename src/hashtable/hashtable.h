#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include "basehashtable.h"
#include "../string.h"

/// @brief HashTable for str key and int value
typedef HashTable HashTableStrInt;

typedef struct
{
    String key;
    int value;
} EntryStrInt;

void hash_table_strint_init(HashTableStrInt *table, size_t size);

void hash_table_strint_add(HashTableStrInt *table, String key, int value);

bool hash_table_strint_get_value(HashTableStrInt *table, String key, EntryStrInt *value);

void hash_table_strint_free(HashTableStrInt *table);

#endif
