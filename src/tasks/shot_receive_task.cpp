#include "shot_receive_task.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "ir_handler.h"
#include "game_engine_task.h"
#include "bluetooth_task.h"

IRHandler *ir_handler;

/* Queue for communicating with Shot Receive Task */
QueueHandle_t shot_receive_task_queue;
int shot_receive_task_queue_size = 2;

void shotReceiveTask(void *parameter)
{
    ReceiveShotMessage *msg;
    ReceiveShotResponseMessage *resp;
    BluetoothMessage *btmsg;

    while (1)
    {
        /* It is the responsibility of the receiver to free this memory */
        msg = new ReceiveShotMessage();
        if (ir_handler->getRxPacket(msg))
        {
            xQueueSend(game_engine_queue, &msg, pdMS_TO_TICKS(10));

            /* Wait for the Game Engine to process the shot */
            if (xQueueReceive(shot_receive_task_queue, &resp, pdMS_TO_TICKS(1000)) == pdTRUE)
            {
                /* The Bluetooth Task will clean up all message pointers, so we can
                 * continue to create new pointers. */
                btmsg = new BluetoothMessage();
                btmsg->service = PLAYER_SERVICE;
                btmsg->characteristic = PLAYER_NUMBER;
                btmsg->value = static_cast<uint32_t>(resp->player_number);
                xQueueSend(bluetooth_task_queue, &btmsg, pdMS_TO_TICKS(10));

                btmsg = new BluetoothMessage();
                btmsg->service = PLAYER_SERVICE;
                btmsg->characteristic = TEAM_NUMBER;
                btmsg->value = static_cast<uint32_t>(resp->player_team);
                xQueueSend(bluetooth_task_queue, &btmsg, pdMS_TO_TICKS(10));

                btmsg = new BluetoothMessage();
                btmsg->service = PLAYER_SERVICE;
                btmsg->characteristic = HEALTH;
                btmsg->value = static_cast<uint32_t>(resp->player_health);
                xQueueSend(bluetooth_task_queue, &btmsg, pdMS_TO_TICKS(10));

                btmsg = new BluetoothMessage();
                btmsg->service = PLAYER_SERVICE;
                btmsg->characteristic = SHIELD;
                btmsg->value = static_cast<uint32_t>(resp->player_shield);
                xQueueSend(bluetooth_task_queue, &btmsg, pdMS_TO_TICKS(10));
            }
        }
        else
        {
            /* Didn't send message, have to delete it ourselves */
            delete msg;
        }
    }

    vTaskDelete(NULL);
}