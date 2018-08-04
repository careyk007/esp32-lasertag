#include "bluetooth_task.h"

#include "bluetooth_handler.h"
#include "game_message.h"

BluetoothHandler *ble;

/* Queue for communication with Bluetooth Task */
QueueHandle_t bluetooth_task_queue;
int bluetooth_task_queue_size = 2;

void bluetoothTask(void *parameter)
{
    ble->init();
    BluetoothResponseMessage *msg;

    while (1)
    {
        /* Block execution until a message has arrived */
        xQueueReceive(bluetooth_task_queue, &msg, portMAX_DELAY);

        /* Always re-write characteristics for simplicity */
        ble->set(msg->service, msg->characteristic, msg->value);

        /* Make sure to free the memory */
        delete msg;
    }

    vTaskDelete(NULL);
}