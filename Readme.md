# ESP32 Lasertag #
This is my vision of what Lasertag can and should be. By adding Bluetooth to the tagger, a companion phone app can provide access to a central server and augment the tagger's information, such as supplying location data. This can allow more game modes than would be possible with just a tagger setup. 

## Simple Features ##
Two game modes:
- Free-For-All: Every player is for themselves and can receive damage from all other players.
- Team Deathmatch: Each player is placed on a team. Received shots from players on the same team results in half damage. This game mode requires a Bluetooth connection to a phone. A companion app will be available <LINK HERE> for download.

Common elements to both game modes:
* Each shot does 10 base damage
* Players start with 100 health and 20 shield
  - Shield may eventually regenerate, but this functionality has not been added
* Clip size is 10
* Firing rate is 2 shots per second
* Reload time is 0.75 seconds


## TODO ##
Add more game modes and work on companion app.

## Dependencies ##
This project is built using PlatformIO. There are a few libraries that this project depends on that will need to be installed:
 * ESP8266-SSD1306 by Fabrice Weinberg

## Known Issues ##
Starting the LCD Task without an I2C Slave device connected causes issues with all the tasks, causing the microcontroller to hang. The current workaround consists of a `#define` value in config.h that should only be uncommented when an LCD is connected.