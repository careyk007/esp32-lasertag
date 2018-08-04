#ifndef _FREE_FOR_ALL_GAME_H_
#define _FREE_FOR_ALL_GAME_H_

#include "game.h"
#include "player.h"
#include "gun.h"
#include "ir_handler.h"

/**
 * \class FreeForAllGame
 * \brief Free for all game mode implementation.
 */
class FreeForAllGame : public Game {
public:
    /**
     * \brief Constructor for Free-for-All game mode.
     */
    FreeForAllGame(Player *player, Gun *gun);
    virtual ~FreeForAllGame(void);

    /*   
     *   Dont have to overwrite Game's handlers since they implement a FFA
     *   game architecture
     */
};

#endif // _FREE_FOR_ALL_GAME_H_