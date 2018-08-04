#ifndef _GAME_MESSAGE_H_
#define _GAME_MESSAGE_H_

#include <Arduino.h>

typedef enum MESSAGE_FROM {
    RECEIVE_SHOT,
    SHOOT,
    SHOOT_RESPONSE,
    RELOAD,
    RELOAD_RESPONSE,
    BLUETOOTH,
    BLUETOOTH_RESPONSE,
    NEW_GAME
} message_from_t;

typedef enum SHOOT_RESPONSE {
    SHOOT_RESPONSE_OK,
    SHOOT_RESPONSE_NO_AMMO,
    SHOOT_RESPONSE_DEAD
} shoot_response_t;

typedef enum RELOAD_RESPONSE {
    RELOAD_RESPONSE_OK,
    RELOAD_RESPONSE_NO_MAG,
    RELOAD_RESPONSE_DEAD
} reload_response_t;

typedef enum BLUETOOTH_SERVICE {
    PLAYER_SERVICE,
    GUN_SERVICE,
    GAME_SERVICE,
    UNKNOWN_SERVICE
} bluetooth_service_t;

typedef enum BLUETOOTH_CHARACTERISTIC {
    PLAYER_NUMBER,
    TEAM_NUMBER,
    HEALTH,
    SHIELD,
    AMMO,
    CLIP_SIZE,
    DAMAGE,
    FIRE_RATE,
    RELOAD_TIME,
    GAME_TYPE,
    GAME_STATUS,
    UNKNOWN_CHARACTERISTIC
} bluetooth_characteristic_t;

typedef enum GAME_MODE_TYPES {
    FREE_FOR_ALL,
    TEAM_DEATHMATCH
} game_type_t;

/**
 * \class GameMessage
 * \brief Base class for specific inter-task messages.
 */
class GameMessage {

public:
    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    virtual message_from_t getType(void) = 0;

    /**
     * \brief Abstract destructor
     */
    virtual ~GameMessage() {}

private:

};

/**
 * \class ReceiveShotMessage
 * \brief Message originating from the Receive Shot Task.
 * 
 * This message contains the information needed by the Game Engine to respond
 * appropriately to an incoming shot packet.
 */
class ReceiveShotMessage : public GameMessage {

public:
    /**
     * \brief Default Constructor.
     */
    ReceiveShotMessage(void) {}
    /**
     * \brief Default Destructor.
     */
    ~ReceiveShotMessage(void) {}

    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    message_from_t getType(void) { return (this_type); }

    uint16_t player_number;         /*!< Shooting player's number */
    uint16_t team_number;           /*!< Shooting player's team number */
    uint16_t damage;                /*!< Received damage value */
    uint32_t timestamp;             /*!< Message timestamp */

private:
    const message_from_t this_type = RECEIVE_SHOT;

};

/**
 * \class ShootMessage
 * \brief Message originating from the Shoot Task.
 * 
 * This message contains the information needed by the Game Engine to 
 * formulate and send a shot packet.
 */
class ShootMessage : public GameMessage {

public:
    /**
     * \brief Default Constructor.
     */
    ShootMessage(void) {}
    /**
     * \brief Default Destructor.
     */
    ~ShootMessage(void) {}

    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    message_from_t getType(void) { return (this_type); }

    uint16_t damage;                /*!< Damage done per shot */
    uint16_t player_number;         /*!< Player's identifying number */
    uint16_t team_number;           /*!< Player's team number */
    uint32_t timestamp;             /*!< Message timestamp */

private:
    const message_from_t this_type = SHOOT;
};

/**
 * \class ShootResponseMessage
 * \brief Message originating from the Game Engine Task to communicate
 *          with the Shoot Task.
 * 
 * This message contains the reponse to the Shoot Message detailing the
 * result of the Shoot Request, specifically whether the shot was taken or
 * an explanation of why the shot was not taken.
 */
class ShootResponseMessage : public GameMessage {

public:
    /**
     * \brief Default Constructor.
     */
    ShootResponseMessage(void) {}
    /**
     * \brief Default Destructor.
     */
    ~ShootResponseMessage(void) {}

    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    message_from_t getType(void) { return (this_type); }

    shoot_response_t shot_valid;    /*!< Response value */
    uint32_t timestamp;             /*!< Message timestamp */

private:
    const message_from_t this_type = SHOOT_RESPONSE;
};

/**
 * \class ReloadMessage
 * \brief Message originating from the Reload Task.
 * 
 * This message contains a request for the Game Engine to allow for an
 * ammunition reload.
 */
class ReloadMessage : public GameMessage {

public:
    /**
     * \brief Default Constructor.
     */
    ReloadMessage(void) {}
    /**
     * \brief Default Destructor.
     */
    ~ReloadMessage(void) {}

    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    message_from_t getType(void) { return (this_type); }

    uint32_t timestamp;         /*!< Message timestamp */

private:
    const message_from_t this_type = RELOAD;
};

/**
 * \class ReloadResponseMessage
 * \brief Message originating from the Game Engine Task to communicate
 *          with the Reload Task.
 * 
 * This message contains the response to the Reload Message granting
 * permission to perform a reload. If permission is not granted, the Game
 * Engine must provide an explanation of why a reload is not allowed.
 */
class ReloadResponseMessage : public GameMessage {

public: 
    /**
     * \brief Default Constructor.
     */
    ReloadResponseMessage(void) {}
    /**
     * \brief Default Destructor.
     */
    ~ReloadResponseMessage(void) {}

    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    message_from_t getType(void) { return (this_type); }

    reload_response_t reload_valid; /*!< Response value */
    uint32_t timestamp;             /*!< Message timestamp */

private:
    const message_from_t this_type = RELOAD_RESPONSE;
};

/**
 * \class BluetoothMessage
 * \brief Message originating from the Bluetooth Handler.
 * 
 * This message contains information about an updated BLE value.
 * It includes the service and characteristic information as well as the
 * modified value.
 */
class BluetoothMessage : public GameMessage {

public:
    /**
     * \brief Default Constructor.
     */
    BluetoothMessage(void) {}
    /**
     * \brief Default Destructor.
     */
    ~BluetoothMessage(void) {}

    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    message_from_t getType(void) { return (this_type); }

    uint16_t characteristic;    /*!< Characteristic that was modified */
    uint16_t service;           /*!< Service that was modified */
    uint32_t value;             /*!< Modified value */
    uint32_t timestamp;         /*!< Message timestamp */

private:
    const message_from_t this_type = BLUETOOTH;
};

/**
 * \class BluetoothResponseMessage
 * \brief Message originating from the Game Engine Task to communicate
 *          with the Bluetooth Task.
 * 
 * This message passes requests from the Game Engine to the Bluetooth Task
 * to update the value of a specified service and characteristic.
 */
class BluetoothResponseMessage : public GameMessage {

public:
    /**
     * \brief Default Constructor.
     */
    BluetoothResponseMessage(void) {}
    /**
     * \brief Default Destructor.
     */
    ~BluetoothResponseMessage(void) {}

    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    message_from_t getType(void) { return (this_type); }

    uint16_t characteristic;    /*!< Characteristic to modify */
    uint16_t service;           /*!< Service to modify */
    uint32_t value;             /*!< Value to modify */
    uint32_t timestamp;         /*!< Message timestamp */

private:
    const message_from_t this_type = BLUETOOTH_RESPONSE;
};

/**
 * \class NewGameMessage
 * \brief Signals to the Game Task that a new game should be started.
 */
class NewGameMessage : public GameMessage {

public:
    /**
     * \brief Default Constructor.
     */
    NewGameMessage(void) {}

    /**
     * \brief Default Destructor.
     */
    ~NewGameMessage(void) {}

    /**
     * \brief Function to determine the true type of the message.
     * 
     * This is important to allow handlers to determine the origin of the
     * message and respond accordingly.
     * 
     *  \return The type of message.
     */
    message_from_t getType(void) { return (this_type); }

    game_type_t game_mode;      /*!< Which game mode to create */
    uint32_t timestamp;         /*!< Message timestamp */

private:
    const message_from_t this_type = NEW_GAME;
};

#endif // _GAME_MESSAGE_H_