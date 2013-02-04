#ifndef RESOURCE_H
#define RESOURCE_H

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUMBER_TABLES 20
typedef struct _GAsyncQueue CustQueue;

#define PRINT(fmt, ...) \
    do { printf("\n%s:%d: " fmt,\
        __FILE__, __LINE__, __VA_ARGS__);} while(0);



typedef struct
{
    int t_no;
    int capacity;
    int is_occupied;
} Table;

typedef struct
{
    int count_members;
    int eat_time;
    int wait_time;
} CustomerGroup;

typedef struct
{
    Table** tables;
    int index;
} TableCollection;

typedef struct
{
    Table** tables;
    GHashTable* map_capacity_tables;

} SeatingManager;
void free_table_collection(void* ptr);
void initialize_seating_manager(SeatingManager** s, int n);

#endif
