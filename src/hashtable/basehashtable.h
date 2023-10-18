#ifndef _BASE_HASH_TABLE_H_
#define _BASE_HASH_TABLE_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _HashTableEntry
{
    void *data;
    struct _HashTableEntry *next;
} HashTableEntry;

/// @brief Base HashTable Struct
typedef struct
{
    HashTableEntry **entries;
    size_t size;
} HashTable;

/// @brief Initialize the base hash table first to use the hast table
/// @param table table object
/// @param size  capacity of the table entry array
void hash_table_base_init(HashTable *table, size_t size);

/// @brief Hash Function
/// @return return the hash of the key
size_t hash_table_base_hash(char *byteKey, size_t keySize);

/// @brief add the key and object to the table
/// @param entryObject reference of the entry object
/// @param entryObjectSize size of the object
/// @return Return status if addition is successful or not
bool hash_table_base_add(HashTable *table, void *key, size_t keySize, void *entryObject, size_t entryObjectSize);

/// @brief Alias for the comparing function pointer
typedef bool comparator(void *currdata, void *key, size_t keySize);

/// @brief Get the value of entry from the table if present else return null
void *hash_table_base_get(HashTable *table, void *key, size_t keySize, comparator *compareFunction);

/// @brief Free the table
void hash_table_base_free(HashTable *table);

#endif