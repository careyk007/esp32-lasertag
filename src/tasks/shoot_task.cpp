#include "shoot_task.h"

#include "../game_message.h"
#include "freertos/FreeRTOS.h"
#include "game_engine_task.h"

/* Queue for communicating trigger button state */
QueueHandle_t trigger_state_queue;
int trigger_state_queue_size = 1;

/* Queue for communication with Shoot Task */
QueueHandle_t shoot_task_queue;
int shoot_task_queue_size = 2;

void shootTask(void *parameter)
{
    ShootMessage *msg;
    ShootResponseMessage *resp;
    bool button;

    /* Half second delay should be long enough to allow others to initialize */
    vTaskDelay(pdMS_TO_TICKS(500));

    while (1)
    {
        /* Initially block for trigger state to change */
        xQueueReceive(trigger_state_queue, &button, portMAX_DELAY);

        /* Check state change */
        while (button) 
        {

            /* Request shot from game engine */
            /* It is the responsibility of the receiver to free this memory */
            msg = new ShootMessage();
            msg->damage = gun->getDamage();
            msg->player_number = player->getPlayerNumber();
            msg->team_number = player->getTeamNumber();
            msg->timestamp = xTaskGetTickCount();

            xQueueSend(game_engine_queue, &msg, pdMS_TO_TICKS(10));

            /* Wait for game engine response and take appropriate action */
            xQueueReceive(shoot_task_queue, &resp, pdMS_TO_TICKS(1000));
            switch (resp->shot_valid) {
            case SHOOT_RESPONSE_OK:
                Serial.println("Shot was valid");
                vTaskDelay(pdMS_TO_TICKS(gun->getFireRate()));
                break;
            case SHOOT_RESPONSE_DEAD:
                Serial.println("Shot not valid: Player Dead");
                vTaskDelay(pdMS_TO_TICKS(1000));
                break;
            case SHOOT_RESPONSE_NO_AMMO:
                Serial.println("Shot not valid: No Ammo");
                vTaskDelay(pdMS_TO_TICKS(500));
                break;
            default:
                break;
            };

            /* Make sure to free the message from the game engine */
            delete resp;

            /* Update trigger status */
            xQueuePeek(trigger_state_queue, &button, 0);
        }
    }

    vTaskDelete(NULL);
}