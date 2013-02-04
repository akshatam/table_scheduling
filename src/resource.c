#include "resource.h"

void initialize_seating_manager(SeatingManager** s, int n)
{
    *s = malloc(sizeof(SeatingManager*));
    (*s)->tables = NULL;
    (*s)->map_capacity_tables = g_hash_table_new_full(g_int_hash, g_int_equal,
                                                NULL, free_table_collection);
    Table** t = NULL;
    t = malloc(sizeof(Table*) * n);

    for(int i = 0; i < n; i++)
    {
        t[i] = malloc(sizeof(Table));
        t[i]->t_no = i;
        t[i]->capacity = 1 + rand()%6;
        t[i]->is_occupied = 0;

        int *k = malloc(sizeof(int));
        *k = t[i]->capacity;

        GSList* l;
        if(g_hash_table_lookup((*s)->map_capacity_tables, k))
        {
            l = 
              (GSList*)g_hash_table_lookup((*s)->map_capacity_tables, k);
            l = g_slist_append(l, t[i]);
            PRINT("Cap: %d tables: %d", t[i]->capacity, g_slist_length(l));

        }
        else
        {
            GSList* l = g_slist_alloc();
            l = g_slist_append(l, t[i]);
            int* tmp_k = malloc(sizeof(int));
            *tmp_k = *k;

            g_hash_table_insert((*s)->map_capacity_tables, tmp_k, l);
            PRINT("Inserting 1st element of %d cap", t[i]->capacity);
        }

        free(k);

        PRINT("Initialized table #%d, with capacity: %d",
                    t[i]->t_no, t[i]->capacity);
    }
    if(t)
    {
        (*s)->tables = t;
        PRINT("Initialized %d tables", n);
    }
} 

void free_table_collection(void* ptr)
{
    TableCollection* t = (TableCollection*)ptr;
    if(t)
    {
        for(int i = 0; i < t->index; i++)
        {    
            free(t->tables[i]);
            t->tables[i] = NULL;
        }
        free(t);
        t = NULL;
    }
}
