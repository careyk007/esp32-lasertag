#include "gpio.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "pins.h"

#include "tasks/reload_task.h"
#include "tasks/shoot_task.h"
#include "tasks/shot_receive_task.h"
#include "tasks/game_engine_task.h"

#include <Arduino.h>

#define ESP_INTR_FLAG_DEFAULT   0


#define GPIO_INPUT_PIN_MASK     ((1 << GPIO_TRIGGER_PIN) | (1 << GPIO_RELOAD_PIN))

// Both the trigger pin and the reload pin are active high
// They need to have the internal pulldowns enabled
volatile uint8_t trigger_pin_status = 0;
volatile uint8_t reload_pin_status  = 0;

void IRAM_ATTR gpio_isr_handler(void* arg) 
{
  // Not necessarily the best way of doing this
  // Should have trigger press release mutex
  // and trigger release consume mutex
  // Infrared_nec.c would then consume the mutex before
  // shooting and release it immediately after firing
  // That way, whenever the user releases the trigger,
  // Infrared_nec.c can't consume the mutex and must
  // wait until the user presses the trigger again

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  BaseType_t result;
  boolean button_state;

  uint32_t pin_number = (uint32_t) arg;
  if (pin_number == GPIO_TRIGGER_PIN) 
  {
    trigger_pin_status = gpio_get_level((gpio_num_t)pin_number);

    if (trigger_pin_status) 
    {
      button_state = true;
    } 
    else 
    {
      button_state = false;
    }

    result = xQueueOverwriteFromISR(trigger_state_queue, &button_state,
                                    &xHigherPriorityTaskWoken);
    if (result == errQUEUE_FULL)
    {
      /* Error */
    }
  } 
  else if (pin_number == GPIO_RELOAD_PIN) 
  {
    reload_pin_status = gpio_get_level((gpio_num_t)pin_number);

    if (reload_pin_status)
    {
      button_state = true;
    }
    else
    {
      button_state = false;
    }

    result = xQueueOverwriteFromISR(reload_state_queue, &button_state,
                                    &xHigherPriorityTaskWoken);
    if (result == errQUEUE_FULL)
    {
      /* Error */
    }
  }
}

void initialize_gpio_inputs(void) 
{
  // GPIO Configuration struct
  gpio_config_t io_conf;

  // Interrupt on both rising and falling edge
  io_conf.intr_type = GPIO_INTR_ANYEDGE;

  // Set pins affected by configuration settings
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_MASK;

  // Set as inputs
  io_conf.mode = GPIO_MODE_INPUT;

  // Enable pullups
  io_conf.pull_up_en = (gpio_pullup_t)0;

  // Apply the configurations
  gpio_config(&io_conf);

  // Install GPIO ISR service
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

  // Hook ISR for individual pins
  gpio_isr_handler_add((gpio_num_t)GPIO_TRIGGER_PIN, gpio_isr_handler, (void*)GPIO_TRIGGER_PIN);
  gpio_isr_handler_add((gpio_num_t)GPIO_RELOAD_PIN,  gpio_isr_handler, (void*)GPIO_RELOAD_PIN );
}

void lowLevelInit(void)
{
    gpio_config_t io_conf;

    /* Disable interrputs */
    io_conf.intr_type = (gpio_int_type_t)GPIO_PIN_INTR_DISABLE;
    /* Output mode */
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1 << RED_LED) |
                           (1 << YELLOW_LED) |
                           (1 << GREEN_LED) |
                           (1 << BLUE_LED);
    /* Disable pull downs */
    io_conf.pull_down_en = (gpio_pulldown_t)0;
    /* Disable pull ups */
    io_conf.pull_up_en = (gpio_pullup_t)0;

    gpio_config(&io_conf);

    ledcSetup(0, 10000, 10);
    ledcAttachPin(BUZZER, 0);

    initialize_gpio_inputs();

    ir_handler->txInit((gpio_num_t)IR_TX_PIN, (rmt_channel_t)IR_TX_CHANNEL);
    ir_handler->setupPayload(player->getPlayerNumber(),
                             gun->getDamage(),
                             player->getTeamNumber());

    ir_handler->rxInit((gpio_num_t)IR_RX_PIN, (rmt_channel_t)IR_RX_CHANNEL);
}