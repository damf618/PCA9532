/**
 * Date:    03/04/2023
 * Author:  Daniel Marquez
 * e-mail:   damf618@gmail.com
 * 
**/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef PCA_9532_LED_H_
#define PCA_9532_LED_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include"stdint.h"
#include"stdbool.h"
#include"pca_9532.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/
#define LEDpERIODOtASK      1
#define MAX_LEDS_STRIP      16
#define COLOR_MAX_BRIGHT    PCA_9532_MAX_PWM
#define COLOR_MIN_BRIGHT    0
#define DEFF_COLOR_BRIGHT   125
#define LED_DRIVER_ERROR    PCA_9532_FAIL
#define LED_DRIVER_OK       PCA_9532_OK
#define RGB_LEDS            3
#define RGB_RED             {DEFF_COLOR_BRIGHT,0,0}
#define RGB_GREEN           {0,DEFF_COLOR_BRIGHT,0}
#define RGB_BLUE            {0,0,DEFF_COLOR_BRIGHT}
#define RGB_YELLOW          {DEFF_COLOR_BRIGHT,DEFF_COLOR_BRIGHT,0}
#define RGB_MAGENTA         {DEFF_COLOR_BRIGHT,0,DEFF_COLOR_BRIGHT}
#define RGB_CYAN            {0,DEFF_COLOR_BRIGHT,DEFF_COLOR_BRIGHT}
#define RGB_WHITE           {DEFF_COLOR_BRIGHT,DEFF_COLOR_BRIGHT,DEFF_COLOR_BRIGHT}
#define RGB_BLACK           {0,0,0}
#define BLINK0              0
#define BLINK1              1
#define PCA_9532_OFF        0
#define PCA_9532_ON         1

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/**
 * @brief Library enum for precalibrated Colors
 */
typedef enum{ RED , GREEN, BLUE, YELLOW, MAGENTA, CYAN ,WHITE, BLACK} color_e;


/**
 * @brief Variable type to validate the execution of the commands
 */
typedef uint8_t Status;

/**
 * @brief Structure to define a LED Colour
 * 
 */
typedef struct
{
    uint8_t rgb_red;
    uint8_t rgb_green;
    uint8_t rgb_blue;
}ledColour_t;


/**
 * @brief Structure to control a LED Strip
 * 
 */
typedef struct
{
    pca9532_conf_t* dev;
    uint8_t         n_leds;
    uint16_t        pin_addresses;
    uint8_t         freq;
    ledColour_t     color;
}ledStrip_t;


/*=====[Prototypes (declarations) of public functions]=======================*/
/*     --  CUSTOM  --    */
Status LedStripInit(ledStrip_t* led, pca9532_conf_t * dev, uint16_t pin_conf);
Status LedColourInit(ledColour_t* color, uint8_t red, uint8_t green, uint8_t blue );
Status LedColourAuto(ledColour_t* color, color_e deffcolor);


/*     --  REQUIRED  --    */
Status LedTurnOff(ledStrip_t led);
Status LedTurnOn(ledStrip_t led, ledColour_t color);
Status LedStayOn(ledStrip_t led, uint16_t timeout, ledColour_t color);
Status LedBlinkColor(ledStrip_t led,uint16_t period, ledColour_t colorA);
Status LedBlinkTwoColors(ledStrip_t led,uint16_t period, ledColour_t colorA, ledColour_t colorB);
Status LedBlinkColorForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA);
Status LedBlinkTwoColorsForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA,ledColour_t colorB);


/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* PCA_9532_LED_H_ */