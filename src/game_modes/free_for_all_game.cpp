#include "free_for_all_game.h"
#include "player.h"
#include "gun.h"
#include "ir_handler.h"

FreeForAllGame::FreeForAllGame(Player *player, Gun *gun) 
    : Game(player, gun) {

}

FreeForAllGame::~FreeForAllGame(void) {

}


/*   
 *   Dont have to overwrite Game's handlers since they implement a FFA
 *   game architecture
 */
// void FreeForAllGame::ShotPacket(GameMessage *packet, Player *player);
// void FreeForAllGame::ShootPacket(GameMessage *packet);
// void FreeForAllGame::ReloadPacket(GameMessage *packet);
// void FreeForAllGame::BLEPacket(GameMessage *packet);