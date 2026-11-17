/**
 * Lab 8 - CAN networking
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "can2040.h"



#define TRANSMITTER 1 //1 to send messages annd 0 to receive messages

#define MAIN_TASK_PRIORITY   (tskIDLE_PRIORITY + 1UL)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

static struct can2040 cbus;
static QueueHandle_t rx_queue;

// Receive
static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    if (notify == CAN2040_NOTIFY_RX) {
        xQueueSendToBack(rx_queue, msg, portMAX_DELAY);
    }
}

static void tx_task(void *params){
    struct can2040_msg msg = {0};
    msg.id = 0x200;
    msg.dlc = 4;

    while (1) {
        int return_value = can2040_transmit(&cbus, &msg);
        printf("sent %lu, return %d\n", msg.data32[0], return_value);
        msg.data32[0]++;
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

static void rx_task(void *params)
{
    struct can2040_msg msg;
    
    while (1) {
        xQueueReceive(rx_queue, &msg, portMAX_DELAY);
        printf("received %lu\n", msg.data[0]);
    }
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

int main(void){
    stdio_init_all();
    rx_queue = xQueueCreate(16, sizeof(struct can2040_msg));
    canbus_setup();

    if (TRANSMITTER) {
        xTaskCreate(tx_task, "TX", 512, NULL, 1, NULL);
    } else {
        xTaskCreate(rx_task, "RX", 512, NULL, 1, NULL);
    }

    vTaskStartScheduler();
    return 0;
}
