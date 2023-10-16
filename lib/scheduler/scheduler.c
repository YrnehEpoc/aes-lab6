#include "scheduler.h"



void sema_grabber_thread(struct k_sem* semToGrab, int id){
    while(1){
        k_sem_take(&semToGrab, K_FOREVER);
        printk("Thread %i has the semaphore",id);
    }
}