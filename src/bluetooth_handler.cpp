#include "bluetooth_handler.h"

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "game_message.h"
#include "tasks/game_engine_task.h"

BluetoothHandler::BluetoothHandler(void) 
{

}

BluetoothHandler::~BluetoothHandler(void) 
{

}

void BluetoothHandler::init(void) 
{
    BLEDevice::init("Biscuits");

    /* Create the BLE server */
    server = BLEDevice::createServer();
    server->setCallbacks(this);

    /* Initialize player service */
    services[PLAYER_SERVICE] = server->createService(PLAYER_SERVICE_UUID);

    for (int i = 0; i < 4; ++i)
    {
        characteristics[i] = services[PLAYER_SERVICE]->createCharacteristic(
            UUIDS[i],
            BLECharacteristic::PROPERTY_READ   |
            BLECharacteristic::PROPERTY_WRITE  |
            BLECharacteristic::PROPERTY_NOTIFY
        );
        characteristics[i]->addDescriptor(new BLE2902());
        characteristics[i]->setCallbacks(this);
    }
    services[PLAYER_SERVICE]->start();

    /* Initialize gun service */
    services[GUN_SERVICE] = server->createService(GUN_SERVICE_UUID);

    for (int i = 4; i < 9; ++i)
    {
        characteristics[i] = services[GUN_SERVICE]->createCharacteristic(
            UUIDS[i],
            BLECharacteristic::PROPERTY_READ   |
            BLECharacteristic::PROPERTY_WRITE  |
            BLECharacteristic::PROPERTY_NOTIFY
        );
        characteristics[i]->addDescriptor(new BLE2902());
        characteristics[i]->setCallbacks(this);
    }
    services[GUN_SERVICE]->start();

    /* Initialize game service */
    services[GAME_SERVICE] = server->createService(GAME_SERVICE_UUID);

    for (int i = 9; i < 11; ++i)
    {
        characteristics[i] = services[GAME_SERVICE]->createCharacteristic(
            UUIDS[i],
            BLECharacteristic::PROPERTY_READ   |
            BLECharacteristic::PROPERTY_WRITE  |
            BLECharacteristic::PROPERTY_NOTIFY
        );
        characteristics[i]->addDescriptor(new BLE2902());
        characteristics[i]->setCallbacks(this);
    }
    services[GAME_SERVICE]->start();

    server->getAdvertising()->start();
}

void BluetoothHandler::set(uint16_t serv, uint16_t charac, uint32_t value)
{
    // BLEService *service = services[serv];
    BLECharacteristic *characteristic = characteristics[charac];

    char new_value[16];

    sprintf(new_value, "%u", value);

    std::__cxx11::string result(new_value);

    characteristic->setValue(result);
    characteristic->notify();
}


/* Override functions */
void BluetoothHandler::onConnect(BLEServer *pServer)
{
    deviceConnected = true;
}

/* Override functions */
void BluetoothHandler::onDisconnect(BLEServer *pServer)
{
    deviceConnected = false;
}

/* Override functions */
void BluetoothHandler::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string value = pCharacteristic->getValue();

    BLEUUID uid = pCharacteristic->getUUID();

    /* Receiver must free this memory */
    BluetoothMessage *msg = new BluetoothMessage();

    if (uid.equals(PLAYER_NUMBER_UUID))
    {
        msg->service = PLAYER_SERVICE;
        msg->characteristic = PLAYER_NUMBER;
    } 
    else if (uid.equals(TEAM_NUMBER_UUID))
    {
        msg->service = PLAYER_SERVICE;
        msg->characteristic = TEAM_NUMBER;
    }
    else if (uid.equals(HEALTH_UUID))
    {
        msg->service = PLAYER_SERVICE;
        msg->characteristic = HEALTH;
    }
    else if (uid.equals(SHIELD_UUID))
    {
        msg->service = PLAYER_SERVICE;
        msg->characteristic = SHIELD;
    }

    else if (uid.equals(AMMO_UUID))
    {
        msg->service = GUN_SERVICE;
        msg->characteristic = AMMO;
    }
    else if (uid.equals(CLIP_SIZE_UUID))
    {
        msg->service = GUN_SERVICE;
        msg->characteristic = CLIP_SIZE;
    }
    else if (uid.equals(DAMAGE_UUID))
    {
        msg->service = GUN_SERVICE;
        msg->characteristic = DAMAGE;
    }
    else if (uid.equals(FIRE_RATE_UUID))
    {
        msg->service = GUN_SERVICE;
        msg->characteristic = FIRE_RATE;
    }
    else if (uid.equals(RELOAD_TIME_UUID))
    {
        msg->service = GUN_SERVICE;
        msg->characteristic = RELOAD_TIME;
    }

    else if (uid.equals(GAME_TYPE_UUID))
    {
        msg->service = GAME_SERVICE;
        msg->characteristic = GAME_TYPE;
        
        /* Need to change game mode */
        delete msg;
        NewGameMessage *new_msg = new NewGameMessage();
        uint32_t temp;
        sscanf(value.c_str(), "%ul", &temp);
        switch(temp) {
        case FREE_FOR_ALL:
            new_msg->game_mode = FREE_FOR_ALL;
            break;
        case TEAM_DEATHMATCH:
            new_msg->game_mode = TEAM_DEATHMATCH;
            break;
        default:
            new_msg->game_mode = FREE_FOR_ALL;
            break;
        }
        xQueueSendFromISR(game_engine_queue, &new_msg, pdFALSE);
        return;

    }
    else if (uid.equals(GAME_STATUS_UUID))
    {
        msg->service = GAME_SERVICE;
        msg->characteristic = GAME_STATUS;
    }
    else
    {
        /* Something funky happened */
        msg->service = UNKNOWN_SERVICE;
        msg->characteristic = UNKNOWN_CHARACTERISTIC;
    }

    /* Incoming values are numbers in string format */
    sscanf(value.c_str(), "%ul", &(msg->value));

    msg->timestamp = xTaskGetTickCount();

    xQueueSendFromISR(game_engine_queue, &msg, pdFALSE);

    if (value.length() > 0)
    {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
            Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
    }
}