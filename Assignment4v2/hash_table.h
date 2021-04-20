#include <stdlib.h>
#include <string.h>

typedef struct table_entry {
	struct table_entry* next; 
	char key[]; 	
} 
table_entry;

typedef struct {
	int slots;	
	table_entry** table;	
}
hash_table;


int compute_hash(char *a);
hash_table* create_new_table(int slots);
void insert(hash_table* hasht, char* key);
int is_present(hash_table* hasht, char* key);
void print_all_entries(hash_table* hasht);
void destroy(hash_table* hasht);