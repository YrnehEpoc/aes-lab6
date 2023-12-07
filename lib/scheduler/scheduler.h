#pragma once
#include <stdio.h>
#include <zephyr.h>
#include <unity.h>


void sema_grabber_thread(struct k_sem* semToGrab, int id, int *counter);