#include "compete.h"


void busy_busy()
{
    for (int i = 0; ; i++);
}

void busy_yield()
{
    for (int i = 0; ; i++) {
        if (!(i & 0xFF)) {
            k_yield();
        }
    }
}

void busy_sleep()
{
    k_busy_wait(10000);
    k_sleep(K_MSEC(490));
}