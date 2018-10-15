#include <Arduino.h>

#include "player.h"
#include "gun.h"
#include "game_modes/game.h"
#include "bluetooth_handler.h"
#include "ir_handler.h"

#include "gpio.h"

#include "tasks/game_engine_task.h"
#include "tasks/bluetooth_task.h"
#include "tasks/reload_task.h"
#include "tasks/shoot_task.h"
#include "tasks/shot_receive_task.h"

#if defined(ENABLE_LCD_TASK)
#include "tasks/lcd_task.h"
#endif

#include "game_message.h"
#include "game_modes/free_for_all_game.h"

#include "config.h"

void setup() {
    Serial.begin(115200);

    Serial.println("Starting program");

    player = new Player();
    ir_handler = new IRHandler();
    gun = new Gun(ir_handler);
    game_engine = new FreeForAllGame(player, gun);
    ble = new BluetoothHandler();

    lowLevelInit();

    trigger_state_queue = 
        xQueueCreate(trigger_state_queue_size, sizeof(boolean));

    reload_state_queue = 
        xQueueCreate(reload_state_queue_size, sizeof(boolean));

    game_engine_queue = 
        xQueueCreate(game_engine_queue_size, sizeof(GameMessage*));

    shoot_task_queue =
        xQueueCreate(shoot_task_queue_size, sizeof(GameMessage*));

    reload_task_queue =
        xQueueCreate(reload_task_queue_size, sizeof(GameMessage*));

    shot_receive_task_queue = 
        xQueueCreate(shot_receive_task_queue_size, sizeof (GameMessage*));

#if defined(ENABLE_LCD_TASK)
    lcd_task_queue = 
        xQueueCreate(lcd_task_queue_size, sizeof(GameMessage*));
#endif

    bluetooth_task_queue = 
        xQueueCreate(bluetooth_task_queue_size, sizeof(GameMessage*));

    if ((trigger_state_queue == NULL)
        || (reload_state_queue == NULL)
        || (game_engine_queue == NULL)
        || (shoot_task_queue == NULL)
        || (reload_task_queue == NULL)
        || (shot_receive_task_queue == NULL)
#if defined(ENABLE_LCD_TASK)
        || (lcd_task_queue == NULL)
#endif
        || (bluetooth_task_queue == NULL)) {
        Serial.println("Error creating the queue");
        while(1) {}
    }

    xTaskCreate(gameEngineTask,
                "Game Engine",
                4096,
                NULL,
                2,
                NULL);

    xTaskCreate(shotReceiveTask,
                "Receive Shot",
                2048,
                NULL,
                1,
                NULL);

    xTaskCreate(reloadTask,
                "Reload Gun",
                2048,
                NULL,
                1,
                NULL);

    xTaskCreate(shootTask,
                "Shoot Gun",
                2048,
                NULL,
                1,
                NULL);

    /* This task relies upon I2C communication to the LCD display. When no
        device is connected, this task casues problems. Therefore, it should
        only be run when the LCD is connected (currently only on the
        breadboard). */
#if defined(ENABLE_LCD_TASK)
    xTaskCreate(LCDTask,
                "LCD",
                2048,
                NULL,
                1,
                NULL);
#endif

    xTaskCreate(bluetoothTask,
                "Bluetooth",
                2048,
                NULL,
                1,
                NULL);
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(100);
}
