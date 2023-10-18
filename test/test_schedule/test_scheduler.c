#include <stdio.h>
#include <zephyr.h>
#include <unity.h>
#include "scheduler.h"

#define STACKSIZE 2000
#define HIPRIDELAY 500
#define THREAD_COUNT 2

struct k_thread supervisor_thread;
K_THREAD_STACK_DEFINE(supervisor_stack, STACKSIZE);

struct k_thread worker_threads[THREAD_COUNT];
K_THREAD_STACK_ARRAY_DEFINE(worker_stacks, THREAD_COUNT, STACKSIZE);

int takenNum = 0;

void setUp(void)
{

    struct k_sem semaphore;
    k_sem_init(&semaphore, 0, 1);

    for (int t = 0; t < THREAD_COUNT; t++) {
        if (t = 0){
        k_thread_create(&worker_threads[t],
                        worker_stacks[t],
                        STACKSIZE,
                        (k_thread_entry_t) sema_grabber_thread,
                        &semaphore, t, &takenNum,
                        K_PRIO_COOP(0),
                        0,
                        K_MSEC(HIPRIDELAY));
        }
        else{

        k_thread_create(&worker_threads[t],
                        worker_stacks[t],
                        STACKSIZE,
                        (k_thread_entry_t) sema_grabber_thread,
                        &semaphore, t, &takenNum,
                        K_PRIO_COOP(1),
                        0,
                        K_NO_WAIT);
        }
    }

    

}   

void tearDown(void)
{
    
    for (int t = 0; t < THREAD_COUNT; t++) {
        k_thread_abort(&worker_threads[t]);
    }

}

void test_grab(void){
    TEST_ASSERT_EQUAL(1,takenNum);
}

int main (void)
{
    
    UNITY_BEGIN();

    RUN_TEST(test_grab);
    return UNITY_END();
}