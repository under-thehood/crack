#include "../src/hashtable/hashtable.h"

void test_hashtable()
{
    HashTableStrInt table;

    hash_table_strint_init(&table, 24);
    hash_table_strint_add(&table, string_create("saugat"), 5666);

    hash_table_strint_add(&table, string_create("something"), 5666);
    hash_table_strint_add(&table, string_create("soething"), 56);
    hash_table_strint_add(&table, string_create("somthing"), 566);
    hash_table_strint_add(&table, string_create("someing"), 166);
    hash_table_strint_add(&table, string_create("something1"), 335666);
    hash_table_strint_add(&table, string_create("something2"), 50);
    hash_table_strint_add(&table, string_create("something44"), 33131);
    hash_table_strint_add(&table, string_create("somethi33ng"), 44);
    hash_table_strint_add(&table, string_create("somethe12312ing"), 323);
    hash_table_strint_add(&table, string_create("somet23121321hing"), 453);

    EntryStrInt e;
    bool status = hash_table_strint_get_value(&table, string_create("saxcsct"), &e);

    if (status == false)
    {
        printf("Cannot get the value\n");
    }
    else
    {
        printf("Value is %d\n", e.value);
    }
    hash_table_strint_free(&table);
}

int main(int argc, char const *argv[])
{
    test_hashtable();
    return 0;
}
