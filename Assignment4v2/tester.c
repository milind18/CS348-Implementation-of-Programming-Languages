#include "hash_table.h"

int main()
{
    hash_table* x = create_new_table(1);
    insert(x,"milind");
    insert(x,"prabhu");
    print_all_entries(x);
    destroy(x);

}