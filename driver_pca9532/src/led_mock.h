#ifndef _LED_MOCK_H_
#define _LED_MOCK_H_

/*==================[inclusions]=============================================*/

#include"FreeRTOS.h"
#include"led.h"


/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/


/*==================[external data declaration]==============================*/

Status LedTurnOff(ledStrip_t led);

Status LedTurnOn(ledStrip_t led, ledColour_t color);

Status LedStayOn(ledStrip_t led, uint16_t timeout, ledColour_t color);


Status LedBlinkColor(ledStrip_t led,uint16_t period,  ledColour_t colorA);


Status LedBlinkTwoColors(ledStrip_t led,uint16_t period, ledColour_t colorA, ledColour_t colorB);


Status LedBlinkColorForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA);


Status LedBlinkTwoColorsForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA,ledColour_t colorB);

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _LED_MOCK_H_ */
