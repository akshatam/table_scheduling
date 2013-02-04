#include "resource.h"

void* generate_customers(void* ptr)
{
    CustQueue *my_q = (CustQueue*)ptr;
    int interval = 0;
    while(1)
    {
        CustomerGroup* c = malloc(sizeof(CustomerGroup)); 
        c->count_members = 1 + rand()%6;
        c->eat_time = rand()%60;
        c->wait_time = rand()%30;

        g_async_queue_lock(my_q);
        g_async_queue_push_unlocked(my_q, c);
        g_async_queue_unlock(my_q);
        g_async_queue_unref(my_q);

        PRINT("Just Pushed: %d member family", c->count_members);
        int sleep_t = rand()%4;
        PRINT("Sleeping for %d", sleep_t);
        sleep(sleep_t);
    }
}

int main()
{
    SeatingManager* s;
    CustQueue* q = g_async_queue_new();
    initialize_seating_manager(&s, NUMBER_TABLES);
    srand ( time(NULL) );

    pthread_t customer_gen;
    pthread_create(&customer_gen, NULL, generate_customers, (void*)q);

    while(1)
    {
        CustomerGroup* g = g_async_queue_pop(q);
        PRINT("Processed customer: %d", g->count_members);

        free(g);
    }

    return 0;
}
