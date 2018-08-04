#ifndef _LCD_H_
#define _LCD_H_

/**
 * \class LCD
 * \brief Class for interfacing with an LCD.
 */
class LCD {
public:
    LCD();
    virtual ~LCD();

    /**
     * \brief Initialize the LCD.
     */
    void init(void);

    /**
     * \brief Runs a debugging test.
     */
    void runTest(void); /* Simply for verification purposes */

private:

};

#endif // _LCD_H_