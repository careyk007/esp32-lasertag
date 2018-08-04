#ifndef _GAME_H_
#define _GAME_H_

#include "gun.h"
#include "player.h"
#include "game_message.h"

/**
 * \class Game
 * \brief An abstract game class defining functions that all game types 
 *          should override.
 * 
 * This class takes care of managing the internal game state and making sure
 *          that everything runs smoothly. 
 */
class Game {
public:
    /**
     * \brief Constructor for Game Object.
     * 
     *  \param player Pointer to the global player object
     *  \param gun Pointer to the global gun object
     */
    Game(Player *player, Gun *gun);
    virtual ~Game(void);

    /**
     * \brief Handler for shot packets.
     * 
     * This function contains the specific game mode logic for responding to an
     * incoming shot packet.
     * Default:
     *  - Every incoming shot deals damage (even self-shots)
     *  - Shields never regenerate
     *  - Health never regenerates
     *  - Player is permanently dead once health reaches zero
     * 
     *  \param packet The packet containing the shot information.
     */
    virtual void ShotPacket(GameMessage *packet);

    /**
     * \brief Handler for shoot request packets.
     * 
     * This function contains the specific game mode logic for responding to a 
     * shot request packet.
     * 
     * Default:
     *  - When the trigger is pulled, the shot task sends a request to
     *      the game engine to send a shot
     *  - The game engine responds with its blessing if a shot is allowed
     *  - If a shot is not allowed, the game engine needs to provide a reason
     *  - It is the responsibility of the shot routine to take appropriate
     *      response to let the user know why a shot was not fired
     * 
     *  \param packet The packet containing the shot request information.
     *  \return A packet to send to the Shoot Task.
     */
    virtual GameMessage *ShootPacket(GameMessage *packet);

    /**
     * \brief Handler for reload packets.
     * 
     * This function contains the specific game mode logic for responding to a
     * reload notification packet.
     * 
     *  \param packet The packet containing the reload information. 
     *  \return A packet to send to the Reload Task
     */
    virtual GameMessage *ReloadPacket(GameMessage *packet);

    /**
     * \brief Handler for BLE write packets.
     * 
     * This function contains the specific game mode logic for responding to a
     * BLE write packet.
     * 
     *  \param packet The packet containing the BLE information.
     *  \return A packet to send to the BLE Task.
     */
    virtual GameMessage *BLEPacket(GameMessage *packet);

protected:
    /**
     * \brief Checks on the Player's status to verify that the player can
     *          still play.
     */
    void checkPlayerStatus(void);
    Player *player = nullptr;   /*!< Pointer to the player. */
    Gun *gun = nullptr;         /*!< Pointer to the gun. */
};


#endif // _GAME_H_