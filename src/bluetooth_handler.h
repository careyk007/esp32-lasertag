#ifndef _BLUETOOTH_HANDLER_H_
#define _BLUETOOTH_HANDLER_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define PLAYER_SERVICE_UUID        "b12f5200-7716-47fe-a893-43a6c0c3fd63"
#define GUN_SERVICE_UUID           "cf3744f0-01c3-459b-bfbc-11dd7d212b5b"
#define GAME_SERVICE_UUID          "082dda50-36cf-4705-ba5d-a8ee5055f0b9"

const BLEUUID PLAYER_NUMBER_UUID("b12f5201-7716-47fe-a893-43a6c0c3fd63");
const BLEUUID TEAM_NUMBER_UUID  ("b12f5202-7716-47fe-a893-43a6c0c3fd63");
const BLEUUID HEALTH_UUID       ("b12f5203-7716-47fe-a893-43a6c0c3fd63");
const BLEUUID SHIELD_UUID       ("b12f5204-7716-47fe-a893-43a6c0c3fd63");

const BLEUUID AMMO_UUID         ("cf3744f1-01c3-459b-bfbc-11dd7d212b5b");
const BLEUUID CLIP_SIZE_UUID    ("cf3744f2-01c3-459b-bfbc-11dd7d212b5b");
const BLEUUID DAMAGE_UUID       ("cf3744f3-01c3-459b-bfbc-11dd7d212b5b");
const BLEUUID FIRE_RATE_UUID    ("cf3744f4-01c3-459b-bfbc-11dd7d212b5b");
const BLEUUID RELOAD_TIME_UUID  ("cf3744f5-01c3-459b-bfbc-11dd7d212b5b");

const BLEUUID GAME_TYPE_UUID    ("082dda51-36cf-4705-ba5d-a8ee5055f0b9");
const BLEUUID GAME_STATUS_UUID  ("082dda52-36cf-4705-ba5d-a8ee5055f0b9");

const BLEUUID UUIDS[] = {
    PLAYER_NUMBER_UUID,
    TEAM_NUMBER_UUID,
    HEALTH_UUID,
    SHIELD_UUID,

    AMMO_UUID,
    CLIP_SIZE_UUID,
    DAMAGE_UUID,
    FIRE_RATE_UUID,
    RELOAD_TIME_UUID,

    GAME_TYPE_UUID,
    GAME_STATUS_UUID
};

/**
 * \class BluetoothHandler
 * \brief Container class for handling Bluetooth interactions
 */
class BluetoothHandler : 
    public BLECharacteristicCallbacks,
    public BLEServerCallbacks
{
public:
    BluetoothHandler(void);
    virtual ~BluetoothHandler();

    /**
     * \brief Initialize the Bluetooth peripheral and setup services
     *          and characteristics.
     * 
     * The services are:
     *  \li Player service
     *  \li Gun service
     *  \li Game service
     * 
     * The characteristics within the Player service are:   
     *  \li Player number
     *  \li Team number
     *  \li Health
     *  \li Shield
     * 
     * The characteristics within the Gun service are:
     *  \li Ammo
     *  \li Clip Size
     *  \li Damage
     *  \li Reload Time
     *  \li Fire Rate
     * 
     * The characteristics within the Game service are:
     *  \li Game Type
     *  \li Game Status
     */
    void init(void);

    /**
     * \brief Set the value of the specified service and characteristic
     * 
     *  \param serv The service index
     *  \param charac The characteristic index
     *  \param value The value to set
     */
    void set(uint16_t serv, uint16_t charac, uint32_t value);

    /* Override functions */
    /**
     * \brief Callback for when a Central device makes a connection.
     */
    void onConnect(BLEServer *pServer);

    /**
     * \brief Callback for when a Central device disconnects.
     */
    void onDisconnect(BLEServer *pServer);

    /**
     * \brief Callback for when a Central device writes a new value.
     */
    void onWrite(BLECharacteristic *pCharacteristic);

private:
    BLEServer *server = NULL;
    BLEService *services[3];
    BLECharacteristic *characteristics[11];
    BLEAdvertising *advertising = NULL;

    bool deviceConnected = false;
};


#endif // _BLUETOOTH_HANDLER_H_