#include "gun.h"

#include "Arduino.h"
#include "ir_handler.h"

Gun::Gun(IRHandler *handler) {
    this->ammo = 10;
    this->clip_size = 10;
    this->damage = 10;
    this->fire_rate = 500;
    this->reload_time = 750;
    this->reloading = false;
    this->ir_handler = handler;
}

Gun::~Gun() {

}

void Gun::restoreGun(void) {
    this->ammo = 10;
    this->clip_size = 10;
    this->damage = 10;
    this->fire_rate = 500;
    this->reload_time = 750;
    this->reloading = false;
}

bool Gun::canShoot(void) {
    if ((this->ammo > 0) && (!this->reloading)) {
        return (true);
    } else {
        return (false);
    }
}

bool Gun::shoot(void) {
    if (this->canShoot()) {
        Serial.print("Shooting Gun with Ammo = ");
        Serial.println(this->ammo);
        ir_handler->sendPayload();
        this->ammo--;
        return (true);
    }
    return (false);
}

void Gun::startReloading(void) {
    this->reloading = true;
}

void Gun::finishReloading(void) {
    this->ammo = this->clip_size;
    this->reloading = false;
}

void Gun::setDamage(uint16_t damage) {
    this->damage = damage;
}

uint16_t Gun::getDamage(void) {
    return (this->damage);
}


void Gun::setAmmo(int ammo) {
    this->ammo = ammo;
}

uint16_t Gun::getAmmo(void) {
    return (this->ammo);
}

void Gun::setClipSize(int clip_size) {
    this->clip_size = clip_size;
}

int Gun::getClipSize(void) {
    return (this->clip_size);
}


void Gun::setFireRate(int fire_rate) {
    this->fire_rate = fire_rate;
}

int Gun::getFireRate(void) {
    return (this->fire_rate);
}

void Gun::setReloadTime(int reload_time) {
    this->reload_time = reload_time;
}

int Gun::getReloadTime(void) {
    return (this->reload_time);
}