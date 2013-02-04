#include "resource.h"

void initialize_seating_manager(SeatingManager** s, int n)
{
    *s = malloc(sizeof(SeatingManager*));
    (*s)->tables = NULL;
    (*s)->map_capacity_tables = g_hash_table_new_full(g_int_hash, g_int_equal,
                                                NULL, NULL);
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
            GSList* l = NULL;
            l = g_slist_append(l, t[i]);
            int* tmp_k = malloc(sizeof(int));
            *tmp_k = *k;

            g_hash_table_insert((*s)->map_capacity_tables, tmp_k, l);
            PRINT("Inserting %d element of %d cap", g_slist_length(l), 
                                                            t[i]->capacity);
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

Table* get_most_optimum_table(SeatingManager** s, int capacity)
{
    int flag = 0;
    Table* t = NULL;
    for(int i = capacity; i <= 6; i++)
    {
        if(t) break;
        int* k = malloc(sizeof(int));
        *k = i;
        PRINT("Checking for Capacity %d tables", *k);

        if(g_hash_table_lookup((*s)->map_capacity_tables, k))
        {
            GSList* sl = g_hash_table_lookup((*s)->map_capacity_tables, k);
            int list_len = g_slist_length(sl);
            for(int j = 0; j < list_len; j++)
            {
                Table* m_table = g_slist_nth_data(sl, j);
                if(m_table->is_occupied == 0)
                {
                    t = m_table;
                    break;
                }
            }
        }

        free(k);
    }
    return t;
}
