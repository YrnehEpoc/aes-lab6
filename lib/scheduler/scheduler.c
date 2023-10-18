#include "scheduler.h"



void sema_grabber_thread(struct k_sem* semToGrab, int id, int *counter){
    while(1){
        int taken = k_sem_take(&semToGrab, K_FOREVER);
        if (taken == 0) counter++;
        printk("Thread %i has the semaphore",id);
    }
}