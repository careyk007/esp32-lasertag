#ifndef _GUN_H_
#define _GUN_H_

#include <stdint.h>
#include "ir_handler.h"

/**
 * \class Gun
 * \brief Representation of the user's gun.
 
 * Gun class handles everything related to the gun's functionality, including
 * shooting and reloading. It is managed by an RTOS function that waits for
 * GPIO interrupts to trigger events. The class also manages the internal state
 * of the gun.
 */
class Gun {
public:
    /**
     * \brief Constructor for Gun object.
     * 
     *  \param handler Pointer to the global IRHandler object.
     */
    Gun(IRHandler *handler);
    virtual ~Gun(void);
    
    /**
     * \brief Send a shot packet.
     * 
     * Instructs the IRHandler to send a shot packet.
     * 
     *  \return True if shot was sent, False if shot was not sent.
     */
    bool shoot(void);

    /**
     * \brief Checks if the gun is able to shoot.
     * 
     * Determines if the gun is able to shoot depending on remaining ammo
     * and whether or not the gun is currently reloading.
     * 
     *  \return True if the gun can shoot, False if the gun cannot shoot.
     */
    bool canShoot(void);

    /**
     * \brief Begin reloading.
     */
    void startReloading(void);

    /**
     * \brief End reloading.
     */
    void finishReloading(void);
    
    /**
     * \brief Set the damage done per shot.
     * 
     *  \param damage The new damage value.
     */
    void setDamage(uint16_t damage);

    /**
     * \brief Get the damage done per shot.
     * 
     *  \return The damage value.
     */
    uint16_t getDamage(void);
    

    /**
     * \brief Set the current ammo in the gun.
     * 
     *  \param ammo The new ammo value.
     */
    void setAmmo(int ammo);

    /**
     * \brief Get the current ammo in the gun.
     * 
     *  \return The ammo value.
     */
    uint16_t getAmmo(void);


    /**
     * \brief Set the maximum clip size for the gun.
     * 
     *  \param clip_size The new maximum clip size.
     */
    void setClipSize(int clip_size);

    /**
     * \brief Get the maximum clip size for the gun.
     * 
     *  \return The maximum clip size.
     */
    int getClipSize(void);


    /**
     * \brief Set the minimum number of milliseconds between shots.
     * 
     *  \param fire_rate The new minimum number of milliseconds between shots.
     */
    void setFireRate(int fire_rate);

    /**
     * \brief Get the minimum number of milliseconds between shots.
     * 
     *  \return The minimum number of milliseconds between shots.
     */
    int getFireRate(void);


    /**
     * \brief Set the number of milliseconds required to reload the gun.
     * 
     *  \param reload_time The new number of milliseconds required to reload.
     */
    void setReloadTime(int reload_time);

    /**
     * \brief Get the number of milliseconds required to reload the gun.
     * 
     *  \return The number of milliseconds required to reload.
     */
    int getReloadTime(void);

    /**
     * \brief Restores the gun to a known state.
     * 
     * This should only be called when starting a new game.
     */
    void restoreGun(void);

private:
    volatile int ammo;
    volatile int clip_size;
    volatile uint16_t damage = 10;
    volatile int fire_rate;
    volatile int reload_time;
    volatile bool reloading;
    IRHandler *ir_handler;
};

#endif // _GUN_H_