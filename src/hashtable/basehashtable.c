#include "basehashtable.h"

void hash_table_base_init(HashTable *table, size_t size)
{
    table->size = 0;
    table->entries = (HashTableEntry **)calloc(size, sizeof(typeof(table->entries)));
    if (table == NULL)
        return;

    table->size = size;
}

size_t hash_table_base_hash(char *byteKey, size_t keySize)
{
    size_t sum = 0;

    for (size_t i = 0; i < keySize; i++)
        sum = sum + byteKey[i];

    return sum;
}

bool hash_table_base_add(HashTable *table, void *key, size_t keySize, void *entryObject, size_t entryObjectSize)
{
    char *byteKey = (char *)key;
    size_t index = hash_table_base_hash(byteKey, keySize) % table->size;

    HashTableEntry *cursor = table->entries[index];
    HashTableEntry **entryAddressAdress = &table->entries[index];

    // if (cursor == NULL)
    //     printf("First Initialized\n");

    if (cursor != NULL)
    {
        // printf("Second Initialized\n");
        while (cursor->next != NULL)
            cursor = cursor->next;

        entryAddressAdress = &cursor->next;
    }

    *entryAddressAdress = (HashTableEntry *)calloc(1, sizeof(HashTableEntry));

    if (*entryAddressAdress == NULL)
    {
        printf("Error has occured\n");
        return false;
    }

    (*entryAddressAdress)->data = malloc(sizeof(entryObjectSize));

    if ((*entryAddressAdress)->data == NULL)
        return false;

    memcpy((*entryAddressAdress)->data, entryObject, entryObjectSize);

    return true;
}

void *hash_table_base_get(HashTable *table, void *key, size_t keySize, comparator *compareFunction)
{
    char *byteKey = (char *)key;

    // Index calculation
    size_t index = hash_table_base_hash(byteKey, keySize) % table->size;

    HashTableEntry *ptr = table->entries[index];
    while (ptr != NULL)
    {
        if (compareFunction(ptr->data, byteKey, keySize) == true)
            return ptr->data;

        ptr = ptr->next;
    }
    return NULL;
}

void hash_table_base_entry_free(HashTableEntry *entry)
{
    if (entry == NULL)
        return;

    hash_table_base_entry_free(entry->next);
    free(entry);
    return;
}
void hash_table_base_free(HashTable *table)
{
    HashTableEntry **entries = table->entries;
    for (size_t i = 0; i < table->size; i++)
    {
        hash_table_base_entry_free(entries[i]);
    }
}
