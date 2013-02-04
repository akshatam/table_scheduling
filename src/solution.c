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

void go_wait_customer(CustQueue* my_q, CustomerGroup* g)
{
        g_async_queue_lock(my_q);
        g_async_queue_push_unlocked(my_q, g);
        g_async_queue_unlock(my_q);
        g_async_queue_unref(my_q);
}

void* eating(void* ptr)
{
    HungryCustomerEating* hce = (HungryCustomerEating*)ptr;
    PRINT("Start Eating @ %d table..", hce->t->t_no);
    sleep(hce->g->eat_time);
    PRINT("ATE @ Table %d..", hce->t->t_no);
    free(hce->g);
    hce->g = NULL;

    hce->t->is_occupied = 0;
}

void go_eat_customer(CustomerGroup* g, Table* t)
{
    t->is_occupied = 1;
    PRINT("Occupying table %d for %d time", t->t_no, g->eat_time);
    HungryCustomerEating* hce = malloc(sizeof(HungryCustomerEating));
    hce->g = g;
    hce->t = t;
    pthread_t t1;
    pthread_create(&t1, NULL, eating, (void*)hce);
}

int main()
{
    SeatingManager* s;
    CustQueue* q = g_async_queue_new();
    CustQueue* wait_q = g_async_queue_new();
    initialize_seating_manager(&s, NUMBER_TABLES);
    srand ( time(NULL) );

    pthread_t customer_gen;
    pthread_create(&customer_gen, NULL, generate_customers, (void*)q);

    while(1)
    {
        CustomerGroup* g = NULL;
        g = g_async_queue_try_pop(wait_q);
        if(!g)
        {
            PRINT("Wait Queue is empty: %d", 0);

            g = g_async_queue_pop(q);
        }

        Table* t = get_most_optimum_table(&s, g->count_members);
        if(t)
        {
            PRINT("Got table %d", t->t_no);
            go_eat_customer(g, t);
        }
        else
        {
            go_wait_customer(wait_q, g);
        }

    }

    return 0;
}
