#include "team_deathmatch_game.h"
#include "player.h"
#include "gun.h"
#include "game_message.h"

TeamDeathmatchGame::TeamDeathmatchGame(Player *player, Gun *gun)
    : Game(player, gun) {

}

TeamDeathmatchGame::~TeamDeathmatchGame(void) {

}

GameMessage *TeamDeathmatchGame::ShotPacket(GameMessage *packet) {
    ReceiveShotMessage *msg = (ReceiveShotMessage *)packet;
    ReceiveShotResponseMessage *response = new ReceiveShotResponseMessage();

    uint8_t damage_divisor = 1;

    if (msg->team_number == this->player->getTeamNumber()) {
        damage_divisor *= 2;
        if (msg->player_number == this->player->getPlayerNumber()) {
            damage_divisor *= 2;
        }
    }

    this->player->takeDamage(msg->damage / damage_divisor);
    
    Serial.print("\t\tPlayer Shield:\t");
    Serial.println(this->player->getShield());
    Serial.print("\t\tPlayer Health:\t");
    Serial.println(this->player->getHealth());

    response->player_health = this->player->getHealth();
    response->player_shield = this->player->getShield();
    response->player_number = this->player->getPlayerNumber();
    response->player_team   = this->player->getTeamNumber();

    checkPlayerStatus();

    response->timestamp = xTaskGetTickCount();

    return (response);
}