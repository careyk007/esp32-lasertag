#ifndef _RELOAD_TASK_H_
#define _RELOAD_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/* Queue for communicating reload button state */
extern QueueHandle_t reload_state_queue;
extern int reload_state_queue_size;

/* Queue for communication with Reload Task */
extern QueueHandle_t reload_task_queue;
extern int reload_task_queue_size;

extern SemaphoreHandle_t reload_mutex;

/**
 * \brief Reload Task Loop
 * 
 * The Reload Task Loop waits for a reload button press before letting the
 * Game Engine know that it is performing a reload operation on the gun.
 * 
 * The Reload Task then begins reloading the gun and delays for the gun's
 * reload time.
 * 
 *  \param parameter Specific parameters for this instance of the task.
 */
extern void reloadTask(void *parameter);

#endif // _RELOAD_TASK_H_