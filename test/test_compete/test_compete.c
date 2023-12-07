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
    
    k_thread_abort(&thread1);
    k_thread_abort(&thread2);
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
                    K_MSEC(1)); 
    
    
    k_thread_create(&thread2,
                    thread2_stack,
                    STACKSIZE,
                    (k_thread_entry_t) t2_entry,
                    NULL, NULL, NULL,
                    t2_prio,
                    0,
                    K_MSEC(10)); 
    
    k_thread_join(&supervisor, K_MSEC(150));
    
    k_thread_runtime_stats_get(&thread1, &thread1_rt_stats);
    k_thread_runtime_stats_get(&thread2, &thread2_rt_stats);
    k_thread_abort(&thread1);
    k_thread_abort(&thread2);
    printk("Test: %s\nThread 1 cycles: %llu\nThread 2 cycles: %llu\n", test, thread1_rt_stats.execution_cycles, thread2_rt_stats.execution_cycles);
}



void coop_busy_busy_equal(void){ 
    run_threads("preempt_busy_busy_equal",
        busy_busy, K_PRIO_COOP(1),
        busy_busy, K_PRIO_COOP(1)
    );
}
void coop_busy_yield_equal(void){ 
    run_threads("preempt_busy_yield_equal",
        busy_yield, K_PRIO_PREEMPT(1),
        busy_yield, K_PRIO_PREEMPT(1)
    );
} 
void preempt_busy_busy_equal(void){ 
    run_threads("preempt_busy_busy_equal",
        busy_busy, K_PRIO_PREEMPT(1),
        busy_busy, K_PRIO_PREEMPT(1)
    );
}
void preempt_busy_yield_equal(void){ 
    run_threads("preempt_busy_yield_equal",
        busy_yield, K_PRIO_PREEMPT(1),
        busy_yield, K_PRIO_PREEMPT(1)
    );
} 


void coop_busy_busy_diff_hipriFirst(void){ 
    run_threads("coop_busy_busy_diff_hipriFirst",
        busy_busy, K_PRIO_COOP(1),
        busy_busy, K_PRIO_COOP(2)
    );
}
void coop_busy_busy_diff_lopriFirst(void){ 
    run_threads("coop_busy_busy_diff_lopriFirst",
        busy_busy, K_PRIO_COOP(2),
        busy_busy, K_PRIO_COOP(1)
    );
}
void coop_busy_yield_diff_hipriFirst(void){ 
    run_threads("coop_busy_yield_diff_hipriFirst",
        busy_yield, K_PRIO_COOP(1),
        busy_yield, K_PRIO_COOP(2)
    );
}
void coop_busy_yield_diff_lopriFirst(void){ 
    run_threads("coop_busy_yield_diff_lopriFirst",
        busy_yield, K_PRIO_COOP(2),
        busy_yield, K_PRIO_COOP(1)
    );
}

 
void preempt_busy_busy_diff_hipriFirst(void){ 
    run_threads("preempt_busy_busy_diff_hipriFirst",
        busy_busy, K_PRIO_COOP(1),
        busy_busy, K_PRIO_COOP(2)
    );
}
void preempt_busy_busy_diff_lopriFirst(void){ 
    run_threads("preempt_busy_busy_diff_lopriFirst",
        busy_busy, K_PRIO_COOP(2),
        busy_busy, K_PRIO_COOP(1)
    );
}
void preempt_busy_yield_diff_hipriFirst(void){ 
    run_threads("preempt_busy_yield_diff_hipriFirst",
        busy_yield, K_PRIO_COOP(1),
        busy_yield, K_PRIO_COOP(2)
    );
}
void preempt_busy_yield_diff_lopriFirst(void){ 
    run_threads("preempt_busy_yield_diff_lopriFirst",
        busy_yield, K_PRIO_COOP(2),
        busy_yield, K_PRIO_COOP(1)
    );
}

void test_busy_sleep(void){ 
    run_threads("busy_sleep",
        busy_sleep, K_PRIO_COOP(1),
        busy_yield, K_PRIO_COOP(2)
    );
}

int main (void)
{
    
    UNITY_BEGIN();

    RUN_TEST(preempt_busy_busy_equal);
    RUN_TEST(coop_busy_yield_equal); 
    RUN_TEST(preempt_busy_busy_equal);
    RUN_TEST(preempt_busy_yield_equal);
    RUN_TEST(coop_busy_busy_diff_hipriFirst);
    RUN_TEST(coop_busy_busy_diff_lopriFirst);
    RUN_TEST(coop_busy_yield_diff_hipriFirst);
    RUN_TEST(coop_busy_yield_diff_lopriFirst);
    RUN_TEST(preempt_busy_busy_diff_hipriFirst);
    RUN_TEST(preempt_busy_busy_diff_lopriFirst);
    RUN_TEST(preempt_busy_yield_diff_hipriFirst);
    RUN_TEST(preempt_busy_yield_diff_lopriFirst);

    RUN_TEST(test_busy_sleep);
    return UNITY_END();
}