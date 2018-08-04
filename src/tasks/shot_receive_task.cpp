#include "shot_receive_task.h"

#include "ir_handler.h"
#include "game_engine_task.h"

IRHandler *ir_handler;

void shotReceiveTask(void *parameter)
{
    // rx_packet_t packet;
    ReceiveShotMessage *msg;

    while (1)
    {
        /* It is the responsibility of the receiver to free this memory */
        msg = new ReceiveShotMessage();
        if (ir_handler->getRxPacket(msg))
        {
            xQueueSend(game_engine_queue, &msg, pdMS_TO_TICKS(10));

            /* No reponse generated from Game Engine */
        }
        else
        {
            /* Didn't send message, have to delete it ourselves */
            delete msg;
        }
    }

    vTaskDelete(NULL);
}