#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdint.h>

/**
 * \class Player
 * \brief Representation of the human player.
 */
class Player {
public:
    Player(void);
    virtual ~Player(void);

    /**
     * \brief Receive damage.
     * 
     *  \param damage The amount of damage to take.
     */
    void takeDamage(int damage);


    /**
     * \brief Set the player number.
     * 
     *  \param player_number The new player number.
     */
    void setPlayerNumber(uint16_t player_number);

    /**
     * \brief Get the player number.
     * 
     *  \return The player number.
     */
    uint16_t getPlayerNumber(void);


    /**
     * \brief Set the player's team number.
     * 
     *  \param team_number The new team number.
     */
    void setTeamNumber(uint16_t team_number);

    /**
     * \brief Get the player's team number.
     * 
     *  \return The player's team number.
     */
    uint16_t getTeamNumber(void);


    /**
     * \brief Set the player's health.
     * 
     *  \param health The player's new health value.
     */
    void setHealth(int health);

    /**
     * \brief Get the player's health.
     * 
     *  \return The player's health value.
     */
    int getHealth(void);


    /**
     * \brief Set the player's shield.
     *  
     *  \param shield The player's new shield value.
     */
    void setShield(int shield);

    /**
     * \brief Get the player's shield.
     *  
     *  \return The player's shield value.
     */
    int getShield(void);


    /**
     * \brief Determine if the player is able to continue playing.
     * 
     *  \return True if the player can play, False if the player cannot play.
     */
    bool canPlay(void);

    /**
     * \brief Restores the player's health and shield to a known state.
     * 
     * This should only be called when a new game starts.
     */
    void restorePlayer(void);

private:
    uint16_t player_number = 1;
    uint16_t team_number = 2;
    int health;
    int shield;
    int max_health;
    int max_shield;
};

#endif // _PLAYER_H_