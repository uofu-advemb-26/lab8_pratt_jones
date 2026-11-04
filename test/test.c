#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "FreeRTOS.h"
#include "task.h"



void setUp(void) {}
void tearDown(void) {}


int main(void)
{
    stdio_init_all();
    xTaskCreate(supervisor_task, "Supervisor", configMINIMAL_STACK_SIZE * 2,
                NULL, SUPERVISOR_PRIORITY, NULL);
    vTaskStartScheduler();
    return 0;
}