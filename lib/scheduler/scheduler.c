#include "scheduler.h"



void sema_grabber_thread(struct k_sem* semToGrab, int id, int *counter){
        int taken = k_sem_take(&semToGrab, K_MSEC(30));
        if (taken == 0) 
            {(*counter)++;
            printk("Thread %i has the semaphore\n",id);}
    
}