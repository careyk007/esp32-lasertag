#ifndef _BLUETOOTH_TASK_H_
#define _BLUETOOTH_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "../bluetooth_handler.h"

extern BluetoothHandler *ble;

/* Queue for communication with Bluetooth Task */
extern QueueHandle_t bluetooth_task_queue;
extern int bluetooth_task_queue_size;

/**
 * \brief Bluetooth Task Loop
 * 
 * Function overview:
 *  - Setup Bluetooth
 *  - Wait for connection to be made
 *  - Update Game Engine with incoming Bluetooth packets
 *  - Update connected device with messages from Game Engine
 * 
 *  \param parameter Specific parameters for this instance of the task.
 */
extern void bluetoothTask(void *parameter);

#endif // _BLUETOOTH_TASK_H_