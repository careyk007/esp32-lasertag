#ifndef _SHOT_RECEIVE_TASK_H_
#define _SHOT_RECEIVE_TASK_H_

#include "../ir_handler.h"

extern IRHandler *ir_handler;

/**
 * \brief Shot Receive Task
 * 
 * Function overview:
 *  - Initialize IR Rx module
 *  - Wait for ringbuf element (Rx packet)
 *  - Parse Rx packet
 *  - Inform Game Engine of packet contents
 * 
 *  \param parameter Specific parameters for this instance of the task.
 */
extern void shotReceiveTask(void *parameter);

#endif // _SHOT_RECEIVE_TASK_H_