#include "compete.h"
#include <stdio.h>
#include <zephyr.h>
#include <unity.h>


#define STACKSIZE 2000
#define HIPRIDELAY 1000
#define THREAD_COUNT 2


void setUp(void)
{


    

}   

void tearDown(void)
{
    
    for (int t = 0; t < THREAD_COUNT; t++) {
        k_thread_abort(&worker_threads[t]);
    }

}

void test_coop_same_priority(void){

    for (int t = 0; t < THREAD_COUNT; t++) {
        if (t == 0){
        k_thread_create(&worker_threads[t],
                        worker_stacks[t],
                        STACKSIZE,
                        (k_thread_entry_t) busy_busy,
                        &semaphore, t, &takenNum,
                        K_PRIO_COOP(0),
                        0,
                        K_MSEC(HIPRIDELAY));
        }
        else
        {
        k_thread_create(&worker_threads[t],
                        worker_stacks[t],
                        STACKSIZE,
                        (k_thread_entry_t) busy_yield,
                        &semaphore, t, &takenNum,
                        K_PRIO_COOP(1),
                        0,
                        K_NO_WAIT);
        }
    }

    TEST_ASSERT_EQUAL(1,takenNum);
}

int main (void)
{
    
    UNITY_BEGIN();

    RUN_TEST(test_grab);
    return UNITY_END();
}