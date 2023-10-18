#include "hashtable.h"

void hash_table_strint_init(HashTableStrInt *table, size_t size)
{
    hash_table_base_init(table, size);
}

void hash_table_strint_add(HashTableStrInt *table, String key, int value)
{
    EntryStrInt newEntryStrInt = {.key = key, .value = value};
    hash_table_base_add(table, newEntryStrInt.key.str, newEntryStrInt.key.size, &newEntryStrInt, sizeof(EntryStrInt));
}

bool hash_table_strint_compare(void *currdata, void *key, size_t keySize)
{
    EntryStrInt *EntryStrIn = (EntryStrInt *)currdata;
    char *keyToSearch = (char *)key;

    if (keySize != EntryStrIn->key.size)
        return false;

    return strncmp(EntryStrIn->key.str, keyToSearch, keySize) == 0;
}

bool hash_table_strint_get_value(HashTableStrInt *table, String key, EntryStrInt *value)
{
    EntryStrInt *result = (EntryStrInt *)hash_table_base_get(table, key.str, key.size, hash_table_strint_compare);
    if (result == NULL)
        return false;
    

    *value = *result;
    return true;
}

void hash_table_strint_free(HashTableStrInt *table)
{
    hash_table_base_free(table);
}