#include "game_engine_task.h"
#include "bluetooth_task.h"
#include "shot_receive_task.h"
#include "shoot_task.h"
#include "reload_task.h"

#include "../game_modes/game.h"
#include "../game_modes/free_for_all_game.h"
#include "../game_modes/team_deathmatch_game.h"

Game *game_engine;
Gun *gun;
Player *player;

/* Queue for communication with Game Handler loop */
QueueHandle_t game_engine_queue;
int game_engine_queue_size = 16;

void gameEngineTask(void *parameter)
{
    GameMessage *packet;
    GameMessage *response;
    while (1)
    {
        /********************************************************************** 
         *  Loop through all received packets                                 *
         *  Doing this as a while loop allows multiple packets                *
         *  to be parsed in one pass through                                  *
        **********************************************************************/
        while (pdPASS ==
               xQueueReceive(game_engine_queue, &packet, portMAX_DELAY))
        {
            /******************************************************************
             *  This is part of the core mechanics of the game                *
             *  There is a parent class, Game, that specific game modes must  *
             *  override. The response to a packet will depend on which game  *
             *  mode is currently active, and so the packet will need to be   *
             *  handed off to whichever the active game mode is.              *
             *****************************************************************/
            switch (packet->getType())
            {
            case RECEIVE_SHOT:
                game_engine->ShotPacket(packet);
                break;
            case SHOOT:
                response = game_engine->ShootPacket(packet);
                xQueueSend(shoot_task_queue, &response, pdMS_TO_TICKS(10));
                break;
            case RELOAD:
                response = game_engine->ReloadPacket(packet);
                xQueueSend(reload_task_queue, &response, pdMS_TO_TICKS(10));
                break;
            case BLUETOOTH:
                response = game_engine->BLEPacket(packet);
                xQueueSend(bluetooth_task_queue, &response, pdMS_TO_TICKS(10));
                break;
            case NEW_GAME:
                delete game_engine;
                switch (((NewGameMessage *)packet)->game_mode)
                {
                case FREE_FOR_ALL:
                    game_engine = new FreeForAllGame(player, gun);
                    break;
                case TEAM_DEATHMATCH:
                    game_engine = new TeamDeathmatchGame(player, gun);
                    break;
                default:
                    game_engine = new FreeForAllGame(player, gun);
                }
                break;
            default:
                break;
            }

            /* Must take care of releasing the packet */
            delete packet;
        }
    }

    /* Should never get here, but just in case delete this task */
    vTaskDelete(NULL);
}