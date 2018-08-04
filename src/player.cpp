#include "player.h"

Player::Player() {
    /* Default initialization for player fields */
    this->setHealth(100);
    this->setShield(20);
    this->setPlayerNumber(10);
    this->setTeamNumber(1);
}

Player::~Player() {
    /* Set all player fields to NULL */
}

void Player::restorePlayer(void) {
    this->setHealth(100);
    this->setShield(20);
}

void Player::takeDamage(int damage) {
    if (this->shield > 0) {
        if (damage < this->shield) {
            this->shield -= damage;
            damage = 0;
        } else {
            damage -= this->shield;
            this->shield = 0;
        }
    }

    if (damage > 0) {
        this->health -= damage;
    }
}

void Player::setPlayerNumber(uint16_t player_number) {
    this->player_number = player_number;
}

uint16_t Player::getPlayerNumber(void) {
    return (this->player_number);
}

void Player::setTeamNumber(uint16_t team_number) {
    this->team_number = team_number;
}

uint16_t Player::getTeamNumber(void) {
    return (this->team_number);
}

bool Player::canPlay() {
    return (this->health > 0) ? true : false;
}

void Player::setHealth(int health) {
    this->health = health;
}

int Player::getHealth(void) {
    return (this->health);
}

void Player::setShield(int shield) {
    this->shield = shield;
}

int Player::getShield(void) {
    return (this->shield);
}