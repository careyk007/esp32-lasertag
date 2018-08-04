#ifndef _TEAM_DEATHMATCH_GAME_H_
#define _TEAM_DEATHMATCH_GAME_H_

#include "game.h"
#include "player.h"
#include "gun.h"
#include "game_message.h"

/**
 * \class TeamDeathmatchGame
 * \brief Team Deathmatch game mode implementation.
 */
class TeamDeathmatchGame : public Game {
public:
    /**
     * \brief Constructor for Team Deathmatch game mode.
     */
    TeamDeathmatchGame(Player *player, Gun *gun);
    virtual ~TeamDeathmatchGame(void);

    /**
     * \brief Overrides default shot packet handler.
     * 
     * With team death match, players receive half damage from other team
     * members and quarter damage from themselves.
     */
    virtual void ShotPacket(GameMessage *packet);

    /*
     * Don't need to override any other default game modes.
     */
};

#endif // _TEAM_DEATHMATCH_GAME_H_