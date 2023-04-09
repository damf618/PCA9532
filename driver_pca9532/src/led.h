/*************************************************************************//**

  @file     led.h

  @brief    Hardware Library to use EVENTs

  @author   Pablo Llull (PL)

  @version  20191122 v0.0.1   PL Initial release.
 ******************************************************************************/


#ifndef _LED_H_
#define _LED_H_

/*==================[inclusions]=============================================*/

#include"FreeRTOS.h"
#include"stdint.h"
#include"stdbool.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif
/*==================[macros]=================================================*/

#define NO_INPUT 	0
#define TRUE 		1
#define FALSE 		0

/* modos de funcionamiento por led */
typedef enum pca9532Modes_t{
	LEDoFF,
	LEDoN,
	LEDpWM0,
	LEDpWM1
}pca9532Modes_t;

/*==================[typedef]================================================*/

// Seven different color schema using RGB led

typedef enum ledMode_t{
	LED_SET_ON,
	LED_SET_OFF,
	LED_BLINK
}ledMode_t;

typedef enum ledState_t{
	LED_OFF,
	LED_ON
}ledState_t;

typedef enum ledColour_t{
	BLACK = 0,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	MAGENTA,
	CYAN,
	WHITE,
	VIOLET,
	ORANGE,
	INDIGO,
	NUMBERoFCOLORS
}ledColour_t;

//  Strips models
typedef enum ledStrip_t{
	LED_AUDIO,
	LED_PCB,
	NUMBERoFSTRIPS
}ledStrip_t;

//  Strips models
typedef enum ledConfigPin{
	CFG_AUDIO,
	CFG_PCB,
	NUMBERoFcONFIG
}pins_color_t;

typedef struct ledRGB_t{
	uint8_t red;
	uint8_t	green;
	uint8_t blue;
}ledRGB_t;

typedef struct LEDcontrol
{
	uint16_t period;
	uint16_t timeout;
	uint16_t current_period;
	uint8_t current_color;
	ledMode_t	mode;
	ledState_t state;
	ledColour_t color[2];
	ledRGB_t *pins_color;
	uint8_t brightControl;
	Bool		foreverFlag;
}LEDcontrol;

typedef struct LEDqueue
{
	uint8_t strip;
	uint16_t period;
	uint16_t timeout;
	ledMode_t	mode;
	ledColour_t colorA;
	ledColour_t colorB;
	Bool		foreverFlag;
}LEDqueue;




/*==================[external data declaration]==============================*/


/*==================[external functions declaration]=========================*/

// Status LedTurnOff(ledStrip_t led);

// Status LedTurnOn(ledStrip_t led, ledColour_t color);

// Status LedStayOn(ledStrip_t led, uint16_t timeout, ledColour_t color);


// Status LedBlinkColor(ledStrip_t led,uint16_t period,  ledColour_t colorA);


// Status LedBlinkTwoColors(ledStrip_t led,uint16_t period, ledColour_t colorA, ledColour_t colorB);


// Status LedBlinkColorForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA);


// Status LedBlinkTwoColorsForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA,ledColour_t colorB);


Status LedGetColor(ledStrip_t led,ledColour_t *color);

Status LedGetMode(uint8_t strip,ledMode_t *mode);

Status LedGetState(uint8_t strip,ledState_t *state);

//void vLedControlLed(void * p);
void vLedControlLed(LEDqueue frame, uint8_t enqueque);

void LEDInit(void);
/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _LED_H_ */
