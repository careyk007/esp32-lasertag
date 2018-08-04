#ifndef _SHOOT_TASK_H_
#define _SHOOT_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/* Queue for communicating trigger button state */
extern QueueHandle_t trigger_state_queue;
extern int trigger_state_queue_size;

/* Queue for communication with Shoot Task */
extern QueueHandle_t shoot_task_queue;
extern int shoot_task_queue_size;

extern SemaphoreHandle_t trigger_mutex;

/**
 * \brief Shoot Task Loop
 * 
 * The Shoot Task waits for a trigger pull before asking the Game Engine to
 * perform a shot. If the Game Engine does not allow a shot, the Shoot Task
 * Loop must inform the user why the shot was denied.
 * 
 *  \param parameter Specific parameters for this instance of the task.
 */
extern void shootTask(void *parameter);

#endif // _SHOOT_TASK_H_