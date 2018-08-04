#include "reload_task.h"

#include "game_engine_task.h"
#include "../pins.h"

/* Queue for communicating reload button state */
QueueHandle_t reload_state_queue;
int reload_state_queue_size = 1;

/* Queue for communication with Reload Task */
QueueHandle_t reload_task_queue;
int reload_task_queue_size = 2;

void reloadTask(void *parameter)
{
    ReloadMessage *msg;
    ReloadResponseMessage *resp;
    bool button;

    while (1)
    {
        /* Initially block for reload state to change */
        xQueueReceive(reload_state_queue, &button, portMAX_DELAY);

        /* Check state change */
        while (button)
        {
            /* Pass message to Game Engine */
            /* It is the responsibility of the receiver to free this memory */
            msg = new ReloadMessage();
            msg->timestamp = xTaskGetTickCount();

            xQueueSend(game_engine_queue, &msg, pdMS_TO_TICKS(10));

            /* There may be a game mode with limited magazines, so wait for
                a response from the Game Engine */
            xQueueReceive(reload_task_queue, &resp, pdMS_TO_TICKS(1000));

            switch (resp->reload_valid)
            {
            case RELOAD_RESPONSE_OK:
                gpio_set_level((gpio_num_t)YELLOW_LED, 1);
                ledcWriteNote(0, NOTE_Bb, 1);

                gun->startReloading();
                vTaskDelay(pdMS_TO_TICKS(gun->getReloadTime()));
                gun->finishReloading();

                gpio_set_level((gpio_num_t)YELLOW_LED, 0);
                ledcWriteTone(0, 0);
                break;
            case RELOAD_RESPONSE_NO_MAG:
                /* Some user feedback please */
                Serial.println("Can't reload: No magazines");
                vTaskDelay(pdMS_TO_TICKS(500));
                break;
            case RELOAD_RESPONSE_DEAD:
                /* Some user feedback please */
                Serial.println("Can't reload: Player is DEAD");
                vTaskDelay(pdMS_TO_TICKS(1000));
                break;
            default:
                break;
            }

            /* Make sure to free the message from the game engine */
            delete resp;

            /* Update trigger status */
            xQueuePeek(reload_state_queue, &button, 0);
        }
    }

    vTaskDelete(NULL);
}