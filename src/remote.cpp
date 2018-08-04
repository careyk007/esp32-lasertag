#include <esp_log.h>
#include <driver/rmt.h>
#include <driver/gpio.h>
#include "stdint.h"
#include "remote.h"

#include "game_message.h"
#include "pins.h"

#include <Arduino.h>

#define HEADER_HIGH_US    2400 /* LASERTAG header: positive 2.4ms        */
#define HEADER_LOW_US     600  /* LASERTAG header: negative 0.6ms        */
#define BIT_ONE_HIGH_US   1200 /* LASERTAG data bit 1: positive 1.2ms    */
#define BIT_ONE_LOW_US    600  /* LASERTAG data bit 1: negative 0.6ms    */
#define BIT_ZERO_HIGH_US  600  /* LASERTAG data bit 0: positive 0.6ms    */
#define BIT_ZERO_LOW_US   600  /* LASERTAG data bit 0: negative 0.6ms    */
#define BIT_END           560  /* LASERTAG end: positive 0.56ms          */
#define BIT_MARGIN        100  /* acceptable timing margin */

#define RX_ACTIVE_LEVEL	0

#define ITEM_DURATION(d) ((d & 0x7fff)*10/TICK_10_US)

#define DATA_ITEM_NUM 50

Remote::Remote(gpio_num_t pin, bool receive, rmt_channel_t channel) {
	this->channel = channel;

	rmt_config_t config;
	
	config.channel                   = channel;
	config.gpio_num                  = pin;
	config.mem_block_num             = 1;
	config.clk_div                   = CLK_DIV;
	
	if (receive) {
		config.rmt_mode 			 		  = RMT_MODE_RX;
		config.rx_config.filter_en			  = true;
		config.rx_config.filter_ticks_thresh  = 100;
		config.rx_config.idle_threshold		  = 950 * (TICK_10_US);
	}
	else {
		config.rmt_mode              	 	  = RMT_MODE_TX;
		config.tx_config.loop_en         	  = false;
		config.tx_config.carrier_en      	  = 1;
		config.tx_config.idle_output_en  	  = true;
		config.tx_config.idle_level      	  = (rmt_idle_level_t)0;
		config.tx_config.carrier_freq_hz 	  = 56000;
		config.tx_config.carrier_level   	  = (rmt_carrier_level_t)1;
		config.tx_config.carrier_duty_percent = 50;
	}


	ESP_ERROR_CHECK(rmt_config(&config));
	if (receive) {
		ESP_ERROR_CHECK(rmt_driver_install(this->channel, 1000, 0));
	}
	else {
		ESP_ERROR_CHECK(rmt_driver_install(this->channel, 0, 0));
	}
}

Remote::~Remote() {
	ESP_ERROR_CHECK(::rmt_driver_uninstall(this->channel));
}

void Remote::rxStart() {
	ESP_ERROR_CHECK(::rmt_rx_start(this->channel, 1));
}

void Remote::rxStop() {
	ESP_ERROR_CHECK(::rmt_rx_stop(this->channel));
}

void Remote::txStart() {
	ESP_ERROR_CHECK(::rmt_tx_start(this->channel, true));
}

void Remote::txStop() {
	ESP_ERROR_CHECK(::rmt_tx_stop(this->channel));
}

void Remote::write() {
  gpio_set_level((gpio_num_t)RED_LED, 1);
  ledcWriteNote(0, NOTE_C, 3);
	ESP_ERROR_CHECK(::rmt_write_items(this->channel, &items[0], items.size(), true));
  ESP_ERROR_CHECK(::rmt_wait_tx_done(this->channel, portMAX_DELAY));
	gpio_set_level((gpio_num_t)RED_LED, 0);
  ledcWriteTone(0, 0);
}

void Remote::generateHeader(void) {
	add(true, 2400);
	add(false, 600);
}

void Remote::generateEnd(void) {
	add(true, 560);
	add(false, 0x7fff);
}

void Remote::add(uint16_t data) {
	for (int index = 0; index < 16; index++) {
		/* Write a '1' */
		if (data & (1 << index)) {
			add(true, 1200);
			add(false, 600);
		}
		/* Write a '0' */
		else {
			add(true, 600);
			add(false, 600);
		}
	}
}

void Remote::add(bool level, uint32_t duration) {
	if (bitCount%2 == 0) {
		rmt_item32_t item;
		item.level0 = level;
		item.duration0 = (duration) / 10 * TICK_10_US;// Convert to uS
		items.push_back(item);
	} else {
		items.at(bitCount/2).level1 = level;
		items.at(bitCount/2).duration1 = (duration) / 10 * TICK_10_US; //to uS
	}
	bitCount++;
}

void Remote::clear() {
	items.clear();
	bitCount = 0;
}

void Remote::getRingBufHandle(rmt_channel_t channel) {
	rmt_get_ringbuf_handle(channel, &(this->rb));
}


bool Remote::receivePacket(ReceiveShotMessage *msg) {
	size_t rx_size = 0;
	while (this->rb) {
		/* Wait indefinitely for rx packet */
		rmt_item32_t *item = (rmt_item32_t*) xRingbufferReceive(this->rb,
			&rx_size,
			portMAX_DELAY);

		/* Parse received packet */
    msg->timestamp = xTaskGetTickCount();
		uint8_t success = this->parse_items(item, (rx_size >> 2), msg);

    vRingbufferReturnItem((this->rb), (void *)item);
		if (success) {
			return (true);
		}
	}
	return false;
}

inline bool check_in_range(int duration_ticks, int target_us, int margin_us) {
  if(( ITEM_DURATION(duration_ticks) < (target_us + margin_us))
      && ( ITEM_DURATION(duration_ticks) > (target_us - margin_us))) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}

bool header_if(rmt_item32_t *item) {
	if((item->level0 == RX_ACTIVE_LEVEL && item->level1 != RX_ACTIVE_LEVEL)
      && check_in_range(item->duration0, HEADER_HIGH_US, BIT_MARGIN)
      && check_in_range(item->duration1, HEADER_LOW_US, BIT_MARGIN)) 
  {
    return true;
  }
  return false;
}

bool bit_one_if(rmt_item32_t* item)
{
  if((item->level0 == RX_ACTIVE_LEVEL && item->level1 != RX_ACTIVE_LEVEL)
      && check_in_range(item->duration0, BIT_ONE_HIGH_US, BIT_MARGIN)
      && check_in_range(item->duration1, BIT_ONE_LOW_US, BIT_MARGIN)) 
  {
    return true;
  }
  return false;
}

bool bit_zero_if(rmt_item32_t* item)
{
  if((item->level0 == RX_ACTIVE_LEVEL && item->level1 != RX_ACTIVE_LEVEL)
      && check_in_range(item->duration0, BIT_ZERO_HIGH_US, BIT_MARGIN)
      && check_in_range(item->duration1, BIT_ZERO_LOW_US, BIT_MARGIN)) 
  {
    return true;
  }
  return false;
}

bool Remote::parse_items(rmt_item32_t *item, size_t numWords, ReceiveShotMessage *msg) {
 int w_len = numWords;
 msg->damage = 0;
 msg->player_number = 0;
 msg->team_number = 0;

  if(w_len < DATA_ITEM_NUM) 
  {
	/* Error condition */
	return false;
  }
  int i = 0, j = 0;
  if(!header_if(item++)) 
  {
    /* Error condition */
    return false;
  }

  for(j = 0; j < 16; j++) 
  {
    if(bit_one_if(item)) 
    {
      msg->player_number |= (1 << j);
    } 
    else if(bit_zero_if(item)) 
    {
      msg->player_number |= (0 << j);
    } 
    else 
    {
      /* Error condition */
      return false;
    }
    item++;
    i++;
  }

  for(j = 0; j < 16; j++) 
  {
    if(bit_one_if(item)) 
    {
      msg->damage |= (1 << j);
    } 
    else if(bit_zero_if(item)) 
    {
      msg->damage |= (0 << j);
    } 
    else 
    {
      /* Error condition */
      return false;
    }
    item++;
    i++;
  }

  for(j = 0; j < 16; j++) 
  {
    if(bit_one_if(item)) 
    {
      msg->team_number |= (1 << j);
    } 
    else if(bit_zero_if(item)) 
    {
      msg->team_number |= (0 << j);
    } 
    else 
    {
      /* Error condition */
      return false;
    }
    item++;
    i++;
  }

  return true;
}