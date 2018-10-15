#include "game.h"
#include "gun.h"
#include "player.h"

#include "Arduino.h"
#include "pins.h"
#include "driver/gpio.h"

Game::Game(Player *player, Gun *gun) {
    this->player = player;
    this->gun = gun;

    player->restorePlayer();
    gun->restoreGun();
}

Game::~Game(void) {
    this->player = nullptr;
    this->gun = nullptr;
}

void Game::checkPlayerStatus(void) {
    if (!this->player->canPlay()) {
        /* Should probably kill all tasks and enter low-power mode? */
        Serial.println("Player is dead");
    }
}

GameMessage *Game::ShotPacket(GameMessage *packet) {
    ReceiveShotMessage *msg = (ReceiveShotMessage *)packet;
    ReceiveShotResponseMessage *response = new ReceiveShotResponseMessage();

    this->player->takeDamage(msg->damage);
    
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

GameMessage *Game::ShootPacket(GameMessage *packet) {
    ShootResponseMessage *response = new ShootResponseMessage();
    if (player->canPlay()) {
        if (gun->canShoot()) {
            response->shot_valid = SHOOT_RESPONSE_OK;
        } else {
            response->shot_valid = SHOOT_RESPONSE_NO_AMMO;
        }
    } else {
        response->shot_valid = SHOOT_RESPONSE_DEAD;
    }

    response->timestamp = xTaskGetTickCount();

    /* Game needs to respond to shoot task for user feedback */
    return (response);
}

GameMessage *Game::ReloadPacket(GameMessage *packet) {
    ReloadResponseMessage *response = new ReloadResponseMessage();

    if (player->canPlay())
    {
        response->reload_valid = RELOAD_RESPONSE_OK;
    }
    else
    {
        response->reload_valid = RELOAD_RESPONSE_DEAD;
    }

    response->timestamp = xTaskGetTickCount();

    /* Game needs to respond to reload task for user feedback */
    return (response);
}

GameMessage *Game::BLEPacket(GameMessage *packet) {
    BluetoothResponseMessage *resp = new BluetoothResponseMessage();
    BluetoothMessage *msg = (BluetoothMessage *)packet;

    switch(msg->service)
    {
    case PLAYER_SERVICE:
        switch(msg->characteristic)
        {
        case PLAYER_NUMBER:
            player->setPlayerNumber(msg->value);
            break;
        case TEAM_NUMBER:
            player->setTeamNumber(msg->value);
            break;
        case HEALTH:
            player->setHealth(msg->value);
            break;
        case SHIELD:
            player->setShield(msg->value);
            break;
        }
        break;
    case GUN_SERVICE:
        switch(msg->characteristic)
        {
        case AMMO:
            gun->setAmmo(msg->value);
            break;
        case CLIP_SIZE:
            gun->setClipSize(msg->value);
            break;
        case DAMAGE:
            gun->setDamage(msg->value);
            break;
        case FIRE_RATE:
            gun->setFireRate(msg->value);
            break;
        }
        break;
    case GAME_SERVICE:
        switch(msg->characteristic)
        {
        case GAME_TYPE:

            break;
        case GAME_STATUS:

            break;
        }
        break;
    default:
        break;
    }

    resp->service = msg->service;
    resp->characteristic = msg->characteristic;
    resp->value = msg->value;
    resp->timestamp = xTaskGetTickCount();

    return (resp);
}