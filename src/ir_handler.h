#ifndef _IR_HANDLER_H_
#define _IR_HANDLER_H_

#include "remote.h"
#include "player.h"
#include "game_message.h"

/**
 * \class IRHandler
 * \brief Container class for underlying IR peripheral.
 */
class IRHandler {
public:
    IRHandler(void);
    ~IRHandler(void);

    /**
     * \brief Initialize RX and TX peripheral.
     * 
     * Initializes the two peripherals used for sending and receiving the
     * IR packets that are central to Laser Tag.
     * 
     *  \param tx_pin The hardware pin used for transmitting.
     *  \param tx_channel The peripheral channel used for transmitting.
     *  \param rx_pin The hardware pin used for receiving.
     *  \param rx_channel The peripheral channel used for receiving.
     */
    void init(gpio_num_t tx_pin, rmt_channel_t tx_channel,
              gpio_num_t rx_pin, rmt_channel_t rx_channel);


    /**
     * \brief Initialize the RX peripheral.
     * 
     * Initializes the hardware peripheral used for receiving the IR packets
     * that are central to Laser Tag.
     * 
     *  \param rx_pin The hardware pin used for receiving.
     *  \param rx_channel The peripheral channel used for receiving.
     */
    void rxInit(gpio_num_t rx_pin, rmt_channel_t rx_channel);

    /**
     * \brief Initialize the TX peripheral.
     * 
     * Initializes the hardware peripheral used for sending the IR packets
     * that are central to Laser Tag.
     * 
     *  \param tx_pin The hardware pin used for transmitting.
     *  \param tx_channel The peripheral channel used for transmitting.
     */
    void txInit(gpio_num_t tx_pin, rmt_channel_t tx_channel);

    /**
     * \brief Get an RX packet.
     * 
     * Get the next RX packet. This function blocks until a new packet is
     * received.
     * 
     *  \param msg A pointer to the packet that will hold the received data.
     *  \return True is the received packet is valid, False if it is not.
     */
    bool getRxPacket(ReceiveShotMessage *msg);


    /**
     * \brief Setup the TX peripheral to send the correct packet.
     * 
     * Sets up the TX packet to contain the player's number and team, as well
     * as the gun's damage.
     * 
     *  \param player_number The player's number.
     *  \param damage The gun's damage value.
     *  \param team_number The player's team number.
     */
    void setupPayload(uint16_t player_number, uint16_t damage, uint16_t team_number);

    /**
     * \brief Sends the TX packet.
     */
    void sendPayload(void);
private:
    Remote *tx;
    Remote *rx;
};

#endif // _IR_HANDLER_H_