#include "lcd_task.h"

#include "game_message.h"
#include "lcd.h"

/* Queue for communication with LCD Task */
QueueHandle_t lcd_task_queue;
int lcd_task_queue_size = 2;

void LCDTask(void *parameter)
{
    LCD lcd;

    vTaskDelay(pdMS_TO_TICKS(2000));

    lcd.init();
    lcd.runTest(); /* For verification purpose only */

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(NULL);
}