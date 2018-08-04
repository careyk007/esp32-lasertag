#ifndef _GAME_ENGINE_TASK_H_
#define _GAME_ENGINE_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "../game_modes/game.h"

extern Game *game_engine;
extern Gun *gun;
extern Player *player;

/* Queue for communication with Game Handler loop */
extern QueueHandle_t game_engine_queue;
extern int game_engine_queue_size;

/**
 * \brief Game Engine Task Loop
 * 
 * The Game Engine Task waits for notifications from the other loops about
 * changes to their status, then forwards that status change to the Game
 * Engine controller. If responses are necessary, the Game Engine Loop
 * sends the response back to the appropriate task.
 * 
 *  \param parameter Specific parameters for this instance of the task.
 */
extern void gameEngineTask(void *parameter);

#endif // _GAME_ENGINE_TASK_H_