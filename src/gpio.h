#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdlib.h>

/**
 * \brief Low level peripheral initialization.
 * 
 * Sets up the User Feedback LEDs and Buzzer.
 */
extern void lowLevelInit(void);

#endif // _GPIO_H_