#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "FreeRTOS.h"
#include "task.h"

#define SUPERVISOR_PRIORITY (tskIDLE_PRIORITY + 1UL)

void setUp(void) {}
void tearDown(void) {}

static void test_placeholder(void)
{
    TEST_ASSERT_TRUE(1);
}

static void supervisor_task(void *params)
{
    (void)params;
    while (1) {
        sleep_ms(5000); // give time to open the serial monitor
        UNITY_BEGIN();
        RUN_TEST(test_placeholder);
        UNITY_END();
        sleep_ms(10000);
    }
}

int main(void)
{
    stdio_init_all();
    xTaskCreate(supervisor_task, "Supervisor", configMINIMAL_STACK_SIZE * 2,
                NULL, SUPERVISOR_PRIORITY, NULL);
    vTaskStartScheduler();
    return 0;
}
