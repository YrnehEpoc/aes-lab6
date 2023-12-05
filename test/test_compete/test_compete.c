#include <stdio.h>
#include <zephyr.h>
#include <unity.h>
#include "compete.h"

#define STACKSIZE 2000 
 

 
struct k_thread thread1, thread2, supervisor;




K_THREAD_STACK_DEFINE(supervisor_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(thread1_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACKSIZE);



void setUp(void)
{

}   

void tearDown(void)
{

}


void super_entry()
{
    printk("Suspending threads\n");
    k_thread_suspend(&thread1);
    k_thread_suspend(&thread2);
}

void run_threads(
    char* test,
    k_thread_entry_t t1_entry, int t1_prio,
    k_thread_entry_t t2_entry, int t2_prio
)
{
    k_thread_runtime_stats_t thread1_rt_stats, thread2_rt_stats; 

    k_thread_create(&supervisor,
                    supervisor_stack,
                    STACKSIZE,
                    (k_thread_entry_t) super_entry,
                    NULL, NULL, NULL,
                    -CONFIG_NUM_COOP_PRIORITIES,
                    0,
                    K_MSEC(75)); 
    
    
    k_thread_create(&thread1,
                    thread1_stack,
                    STACKSIZE,
                    (k_thread_entry_t) t1_entry,
                    NULL, NULL, NULL,
                    t1_prio,
                    0,
                    K_MSEC(10)); 
    
    
    k_thread_create(&thread2,
                    thread2_stack,
                    STACKSIZE,
                    (k_thread_entry_t) t2_entry,
                    NULL, NULL, NULL,
                    t2_prio,
                    0,
                    K_MSEC(15)); 
    
    k_thread_join(&supervisor, K_MSEC(150));
    
    k_thread_runtime_stats_get(&thread1, &thread1_rt_stats);
    k_thread_runtime_stats_get(&thread2, &thread2_rt_stats);
    k_thread_abort(&thread1);
    k_thread_abort(&thread2);
    printk("Test: %s\nThread 1 cycles: %llu\nThread 2 cycles: %llu\n", test, thread1_rt_stats.execution_cycles, thread2_rt_stats.execution_cycles);
}



void preempt_busy_busy_equal(void){ 
    run_threads("preempt_busy_busy_equal",
        busy_busy, K_PRIO_PREEMPT(1),
        busy_busy, K_PRIO_PREEMPT(1)
    );
}


int main (void)
{
    
    UNITY_BEGIN();

    RUN_TEST(preempt_busy_busy_equal);
    // RUN_TEST(coop_equal_busy_busy); 
    // RUN_TEST(coop_equal_busy_yield);
    // RUN_TEST(preempt_equal_busy_yield);
    // RUN_TEST(coop_high_busy_busy);
    // RUN_TEST(coop_low_busy_busy);
    // RUN_TEST(coop_high_busy_yield);
    // RUN_TEST(coop_low_busy_yield);
    // RUN_TEST(preempt_high_busy_busy);
    // RUN_TEST(preempt_low_busy_busy);
    // RUN_TEST(preempt_high_busy_yield);
    // RUN_TEST(preempt_low_busy_yield);
    return UNITY_END();
}