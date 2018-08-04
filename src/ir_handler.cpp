#include "ir_handler.h"
#include "gun.h"
#include "player.h"
#include <stdint.h>
#include "pins.h"
#include "game_message.h"

IRHandler::IRHandler(void) {

}

IRHandler::~IRHandler(void) {
    tx->txStop();
    tx->clear();

    rx->rxStop();
    rx->clear();
}

void IRHandler::init(gpio_num_t tx_pin, rmt_channel_t tx_channel,
              gpio_num_t rx_pin, rmt_channel_t rx_channel) {
    this->tx = new Remote(tx_pin, false, tx_channel);
    this->rx = new Remote(rx_pin, true, rx_channel);
}

void IRHandler::rxInit(gpio_num_t rx_pin, rmt_channel_t rx_channel) {
    this->rx = new Remote(rx_pin, true, rx_channel);
    this->rx->getRingBufHandle(rx_channel);
    this->rx->rxStart();
}

void IRHandler::txInit(gpio_num_t tx_pin, rmt_channel_t tx_channel) {
    this->tx = new Remote(tx_pin, false, tx_channel);
}

bool IRHandler::getRxPacket(ReceiveShotMessage *msg) {
    /* Function under construction
        Needs to wait for rx->xRingbufferReceive() to receive a packet 
        then rx->parsePacket will break the packet into a useful struct
        the struct will then get passed back to the shotReceiveTask */
    return (rx->receivePacket(msg));
}

void IRHandler::setupPayload(uint16_t player_number, uint16_t damage, uint16_t team_number) {
    //uint16_t player_number = player->getPlayerNumber();
    //uint16_t damage = gun->getDamage();
    //uint16_t team_number = player->getTeamNumber();

    tx->txStop();
    tx->clear();

    tx->generateHeader();

    tx->add(player_number);
    tx->add(damage);
    tx->add(team_number);

    tx->generateEnd();

    tx->txStart();
}

void IRHandler::sendPayload(void) {
    //tx->txStop();
    //tx->txStart();
    tx->write();
}