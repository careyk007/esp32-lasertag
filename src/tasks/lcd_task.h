#ifndef _LCD_TASK_H_
#define _LCD_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/* Queue for communication with LCD Task */
extern QueueHandle_t lcd_task_queue;
extern int lcd_task_queue_size;

/**
 * \brief LCD Task Loop
 * 
 * The LCD Task Loop is in charge of more detailed user feedback
 * including displaying status information such as current health
 * and shield levels, the amount of remaining ammo, and player name.
 * 
 *  \param parameter Specific parameters for this instance of the task.
 */
extern void LCDTask(void *parameter);

#endif // _LCD_TASK_H_