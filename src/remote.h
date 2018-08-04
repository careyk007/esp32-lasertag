#ifndef _REMOTE_H_
#define _REMOTE_H_
#include <driver/rmt.h>
#include <vector>
#include <stdint.h>

#include "game_message.h"

#define CLK_DIV			(100)
#define TICK_10_US		(80000000/CLK_DIV/100000)

/**
 * \class Remote
 * \brief Drive the remote (IR) peripheral.
 */
class Remote {
public:
	/**
	 * \brief Constructor for Remote object.
	 * 
	 * 	\param pin The hardware pin associated with the remote peripheral.
	 * 	\param receive Whether the peripheral should be set up to receive.
	 * 	\param channel Which peripheral channel should be used by the remote.
	 */
	Remote(gpio_num_t pin, bool receive, rmt_channel_t channel=RMT_CHANNEL_0);
	virtual ~Remote();

	/**
	 * \brief Generate the start of packet header.
	 */
	void generateHeader(void);

	/**
	 * \brief Generate the end of packet signifier.
	 */
	void generateEnd(void);

	/**
	 * \brief Add 16 bits to the packet.
	 * 
	 * 	\param data The data to be added to the packet.
	 */
	void add(uint16_t data);

	/**
	 * \brief reset packet information.
	 */
	void clear();

	/**
	 * \brief Start receiving.
	 */
	void rxStart();

	/**
	 * \brief Stop receiving.
	 */
	void rxStop();

	/**
	 * \\breif Start transmitting.
	 */
	void txStart();

	/**
	 * \brief Stop transmitting.
	 */
	void txStop();

	/**
	 * \brief Send one packet.
	 */
	void write();

	/**
	 * \brief Return the handle to the receiving RingBuf.
	 * 
	 * 	\param channel The peripheral channel to get the RingBuf from.
	 */
	void getRingBufHandle(rmt_channel_t channel);

	/**
	 * \brief Get the next received packet.
	 * 	
	 * This function will block until a new packet is received.
	 * 
	 * 	\param packet Pointer to the memory location to store the data.
	 * 	\return True if the packet is valid, False if the packet is not valid.
	 */
	bool receivePacket(ReceiveShotMessage *msg);

	/**
	 * \brief Determine if the received packet is valid.
	 * 
	 * 	\param item Pointer to received data.
	 * 	\param numWords The number of received bits.
	 * 	\param msg Pointer to the memory location to store the data.
	 * 	\return True if the packet is valid, False if the packet is not valid.
	 */
	bool parse_items(rmt_item32_t *item, size_t numWords, ReceiveShotMessage *msg);


private:
	rmt_channel_t channel;
	// rx_packet_t packet;
	RingbufHandle_t rb;
	std::vector<rmt_item32_t> items;
	int bitCount = 0;
	void add(bool level, uint32_t duration);
};

#endif // _REMOTE_H_