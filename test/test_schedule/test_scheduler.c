#include <stdio.h>
#include <zephyr.h>
#include <unity.h>
#include "scheduler.h"

#define STACKSIZE 2000  

struct k_thread thread1;
struct k_thread thread2;
K_THREAD_STACK_DEFINE(thread1_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACKSIZE);

int takenNum = 0;

void setUp(void)
{

}

void test_schedule(void){

    struct k_sem semaphore;
    k_sem_init(&semaphore, 0, 1);
 
    k_thread_create(&thread1,
                    thread1_stack,
                    STACKSIZE,
                    (k_thread_entry_t) sema_grabber_thread,
                    &semaphore, 1, &takenNum,
                    K_PRIO_PREEMPT(1),
                    0,
                    K_MSEC(15)); 

    k_thread_create(&thread2,
                    thread2_stack,
                    STACKSIZE,
                    (k_thread_entry_t) sema_grabber_thread,
                    &semaphore, 2, &takenNum,
                    K_PRIO_PREEMPT(2),
                    0,
                    K_NO_WAIT);  

    TEST_ASSERT_EQUAL(1,takenNum);
}   

void tearDown(void)
{
     

}
 

int main (void)
{
    
    UNITY_BEGIN();

    RUN_TEST(test_schedule);
    printk("made it to this point");
    return UNITY_END();
}