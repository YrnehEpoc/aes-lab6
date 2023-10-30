#include <stdio.h>
#include <zephyr.h>
#include <unity.h>
#include "compete.h"

#define STACKSIZE 2000
#define HIPRIDELAY 1000
#define THREAD_COUNT 2

struct k_thread supervisor_thread;
K_THREAD_STACK_DEFINE(supervisor_stack, STACKSIZE);

struct k_thread worker_threads[THREAD_COUNT];
K_THREAD_STACK_ARRAY_DEFINE(worker_stacks, THREAD_COUNT, STACKSIZE);

int takenNum = 0;

void setUp(void)
{

}   

void tearDown(void)
{
    
    for (int t = 0; t < THREAD_COUNT; t++) {
        k_thread_abort(&worker_threads[t]);
    }

}

void preempt_busy_busy_equal(void){
    char myName[] = "Henry";
    k_thread_runtime_stats_t init_stats;
    k_thread_runtime_stats_all_get(&init_stats);
    for (int t = 0; t < THREAD_COUNT; t++) {
        if (t == 0){
        k_thread_create(&worker_threads[t],
                        worker_stacks[t],
                        STACKSIZE,
                        (k_thread_entry_t) busy_busy,
                        &myName, NULL, NULL,
                        K_PRIO_COOP(0),
                        0,
                        K_NO_WAIT);
        }
    }
    
    //k_thread_runtime_stats_get(k_current_get(),&rt_stats_thread);
    printk("Cycles: %llu\n", init_stats);
}


int main (void)
{
    
    UNITY_BEGIN();

    RUN_TEST(preempt_busy_busy_equal);
    return UNITY_END();
}