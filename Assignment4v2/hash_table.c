#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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


int compute_hash(char *a)
{
    int val = 0;
    for(int i = 0; i < strlen(a); i++)
    {
        val += (a[i] - 'A');
    }
    return val;
}



hash_table* create_new_table(int slots)
{
    //printf("Creating hash table with %d slots\n",slots);
	hash_table* hasht = malloc(sizeof(hash_table));
    hasht->slots = slots;

	if(!hasht)
		return NULL;
    hasht->table = malloc(slots*sizeof(table_entry*));

	if(hasht -> table == NULL)
	{
		free(hasht->table);
		return NULL;
	}
	for(int i = 0; i < slots; i++)
		hasht->table[i] = NULL;
	return hasht;
}

void insert(hash_table* hasht, char* key)
{
	int h = compute_hash(key) % hasht->slots;
	table_entry* e = hasht->table[h];

	while(e != NULL)
	{
		if(!strcmp(e->key, key))
		{
			return;
		}
		e = e->next;
	}


	if((e = malloc(sizeof(table_entry)+strlen(key)+1)) == NULL)
		return;
	strcpy(e->key, key);

	e->next = hasht->table[h];
	hasht->table[h] = e;

}

int is_present(hash_table* hasht, char* key)
{
	unsigned int h = compute_hash(key) % hasht->slots;
	table_entry* e = hasht->table[h];
	while(e != NULL)
	{
		if(!strcmp(e->key, key))
			return 1;
		e = e->next;
	}
	return 0;
}

int print_all_entries(hash_table* hasht)
{
	//printf("Printing all hashtable entries: ");
	for(int i = 0; i < hasht ->slots; i++)
	{
		table_entry* e = hasht->table[i];
		while(e != NULL)
		{
			printf("%s ",e->key);
			e = e->next;
		}
	}
	printf("\n");
	
}
void destroy_list(table_entry* element)
{
	if(element == NULL) return;
	
	if(element->next != NULL) destroy_list(element->next);

	element->next = NULL;
	free(element);
	
}
void destroy(hash_table* hasht)
{
	for(int i  = 0; i < hasht->slots; i++) 
		destroy_list(hasht->table[i]);
	free(hasht);
}

