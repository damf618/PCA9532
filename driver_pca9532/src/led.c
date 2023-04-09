/*************************************************************************//**

  @file     led.c

  @brief    Hardware Library to use Leds

  @author   Alejandro Bujan

  @version  20200501 v0.5.0   AB
 ******************************************************************************/

/*==================[inclusions]=============================================*/
#include "led.h"
#include "led_mock.h"
//#include "pca9532.h"
//#include <FreeRTOS.h>
//#include <task.h>
//#include <queue.h>
//#include <semphr.h>

//#include "timers.h"
/*==================[macros and definitions]=================================*/

#define LEDqUEUEsIZE 4
#define LEDpERIODOtASK 5
#define LEDqERIODOtASK 1
#define SPEED_PRESCALER_HIGH  2
#define SPEED_PRESCALER_MID 	4
#define SPEED_PRESCALER_LOW 	5
/*==================[internal data declaration]==============================*/

LEDcontrol strips[NUMBERoFSTRIPS];
//Custom
TickType_t timings[NUMBERoFSTRIPS];
LEDqueue commands[NUMBERoFSTRIPS];
uint8_t blinks[NUMBERoFSTRIPS];
//TODO: Init them in 0 value

//QueueHandle_t queueLeds;

/*==================[internal functions declaration]=========================*/

// void ledDuty(uint8_t mode, uint8_t rate);

// /**
//  * @brief Set the schema RGB led strip with the indicated color
//  * @param 	*led	Pointer to ledRGB_t
//  * @param	colour	Color schema
//  * @return 	Nothing
//  */
// void ledColour(ledRGB_t *led, ledColour_t colour, uint8_t brightControl);

/**
 * @brief Set the led of strip with the indicated color
 * @param	ledStrip
 * @param	colour
 * @return	Nothing
 */
Status ledStrip(ledStrip_t ledStrip, ledColour_t colour, uint8_t brightControl);


/**
 * @brief Init all the RGB led strips with the indicated color
 * @param	colour
 * @return	Nothing
 */
void ledInit(ledColour_t colour);

// /**
//  * @brief Set the led of strip with the indicated color
//  * @param	ledStrip
//  * @param	colour
//  * @return	Nothing
//  */
// Status ledStrip(ledStrip_t ledStrip, ledColour_t colour, uint8_t brightControl);

// Status ledInitModeOn (uint8_t strip,LEDqueue frame);

// Status ledInitModeOff (uint8_t strip,LEDqueue frame);

// Status ledInitModeBlink (uint8_t strip,LEDqueue frame);

/*==================[internal data definition]===============================*/

ledRGB_t stripsConfigPins[] = {
			{2,1,0}, /* jack */
			{12,13,14}, /*D17vOTARP6*/
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

// void ledDuty(uint8_t mode, uint8_t rate){

// 	uint8_t reg;

// 	if(mode == LEDpWM0)
// 		reg = 0;
// 	else if (mode == LEDpWM1)
// 		reg = 1;
// 	else
// 		return;

// 	LEDSet_Duty(reg,rate);
// }


// /**
//  * @brief Set the schema RGB led strip with the indicated color
//  */
// void ledColour(ledRGB_t *led, ledColour_t colour, uint8_t brightControl){

// 	/*inicialmente apago todo los colores antes de seleccionar la nueva combinacion */
// 	LEDSet(led->red, LEDoFF);
// 	LEDSet(led->green, LEDoFF);
// 	LEDSet(led->blue, LEDoFF);

// 	switch(colour)
// 	{
// 		case RED:
// 			LEDSet(led->red, brightControl);
// 			break;
// 		case GREEN:
// 			LEDSet(led->green, brightControl);
// 			break;
// 		case BLUE:
// 			LEDSet(led->blue, brightControl);
// 			break;
// 		case YELLOW:
// 			LEDSet(led->red, brightControl);
// 			LEDSet(led->green, brightControl);
// 			break;
// 		case MAGENTA:
// 			LEDSet(led->red, brightControl);
// 			LEDSet(led->blue, brightControl);
// 			break;
// 		case CYAN:
// 			LEDSet(led->green, brightControl);
// 			LEDSet(led->blue, brightControl);
// 			break;
// 		case WHITE:
// 			LEDSet(led->red, brightControl);
// 			LEDSet(led->green, brightControl);
// 			LEDSet(led->blue, brightControl);
// 			break;
// 		case VIOLET:
// 			ledDuty(LEDpWM0,9);
// 			ledDuty(LEDpWM1,8);
// 			LEDSet(led->red, LEDpWM0);
// 			LEDSet(led->green, LEDoFF);
// 			LEDSet(led->blue, LEDpWM1);
// 			break;
// 		case ORANGE:
// 			ledDuty(LEDpWM0,33);
// 			ledDuty(LEDpWM1,2);
// 			LEDSet(led->red, LEDpWM0);
// 			LEDSet(led->green, LEDpWM1);
// 			LEDSet(led->blue, LEDoFF);
// 			break;
// 		case INDIGO:
// 			ledDuty(LEDpWM0,2);
// 			ledDuty(LEDpWM1,7);
// 			LEDSet(led->red, LEDpWM0);
// 			LEDSet(led->green, LEDoFF);
// 			LEDSet(led->blue, LEDpWM1);
// 			break;
// 		case BLACK:
// 			LEDSet(led->red, LEDoFF);
// 			LEDSet(led->green, LEDoFF);
// 			LEDSet(led->blue, LEDoFF);
// 			break;

// 		default:
// 			break;
// 	}
// }


// /**
//  * @brief Set the led of strip with the indicated color
//  */
// Status ledStrip(ledStrip_t ledStrip, ledColour_t colour, uint8_t brightControl){


// 	if(ledStrip >= NUMBERoFSTRIPS || colour >= NUMBERoFCOLORS)
// 		return ERROR;

// 	ledColour(strips[ledStrip].pins_color,colour,brightControl);

// 	return SUCCESS;
// }


// Status LedGetState(uint8_t strip,ledState_t *state){

// 	/* realizo chequeos de seguridad */
// 	if(strip >= NUMBERoFSTRIPS) return ERROR;

// 	*state = strips[strip].state;

// 	return SUCCESS;
// }

// Status LedGetMode(uint8_t strip,ledMode_t *mode){

// 	/* realizo chequeos de seguridad */
// 	if(strip >= NUMBERoFSTRIPS) return ERROR;

// 	*mode = strips[strip].mode;

// 	return SUCCESS;
// }

// Status LedGetCurrentColor(uint8_t strip,ledColour_t *color){

// 	/* realizo chequeos de seguridad */
// 	if(strip >= NUMBERoFSTRIPS) return ERROR;

// 	*color = strips[strip].color[0];

// 	return SUCCESS;
// }


/**
 * @brief Init all the RGB led strips with the indicated color
 */
void ledInit(ledColour_t colour){

	uint8_t i;

	for(i=0 ; i < NUMBERoFSTRIPS; i++){

		strips[i].color[0] = BLACK;
		strips[i].color[1] = BLACK;
		strips[i].current_period = 0;
		strips[i].period = 0;
		strips[i].mode = LED_SET_OFF;
		strips[i].state = LED_OFF;
		strips[i].timeout = 0;
		strips[i].pins_color = &stripsConfigPins[i];
		strips[i].foreverFlag = TRUE;
	}


	/* Condicion Inicial de colores */

	//ledStrip(LED_AUDIO,colour,LEDoN);
	//ledStrip(LED_PCB,colour,LEDoN);

}



// Status ledInitModeOn (uint8_t strip,LEDqueue frame){

// 	LEDcontrol *pled;

// 	if(strip >= NUMBERoFSTRIPS) return ERROR;
// 	/* apunto a la estructura del strip */
// 	pled = &strips[strip];

// 	pled->mode = frame.mode;
// 	pled->color[0] = frame.colorA;
// 	pled->timeout = frame.timeout;
// 	pled->period = frame.period;
// 	pled->foreverFlag = frame.foreverFlag;
// 	/* Enciendo el Led */
// 	pled->brightControl = LEDoN;
// 	ledStrip(strip,pled->color[0],pled->brightControl);
// 	pled->state = LED_ON;

// 	return TRUE;
// }

// Status ledInitModeOff (uint8_t strip,LEDqueue frame){

// 	LEDcontrol *pled;

// 	if(strip >= NUMBERoFSTRIPS) return ERROR;
// 	/* apunto a la estructura del strip */
// 	pled = &strips[strip];

// 	/* apago el Led */
// 	pled->brightControl = LEDoFF;
// 	ledStrip(strip,BLACK,pled->brightControl);
// 	pled->state = LED_OFF;
// 	/* reseteo modo */
// 	pled->mode = LED_SET_OFF;

// 	return TRUE;

// }

// Status ledInitModeBlink (uint8_t strip,LEDqueue frame){

// 	LEDcontrol *pled;

// 	if(strip >= NUMBERoFSTRIPS) return ERROR;
// 	/* apunto a la estructura del strip */
// 	pled = &strips[strip];

// 	pled->mode = frame.mode;
// 	pled->color[0] = frame.colorA;
// 	pled->color[1] = frame.colorB;
// 	pled->timeout = frame.timeout;
// 	pled->period = frame.period;
// 	pled->foreverFlag = frame.foreverFlag;
// 	/*incializo periodo */
// 	pled->current_color = 0;
// 	pled->current_period = 0;
// 	/* Enciendo el Led */
// 	pled->brightControl = LEDoN;
// 	ledStrip(strip,pled->current_color,pled->brightControl);
// 	pled->state = LED_ON;

// 	return TRUE;
// }

void ledblinkProcess(LEDqueue frame)
{
	// LED Stay On
	if((BLACK == frame.colorB) && (NO_INPUT != frame.timeout) && (NO_INPUT == frame.period) && (BLACK != frame.colorA))
	{
		LedStayOn(frame.strip, frame.timeout, frame.colorA);
	}
	// LED Blink Color 
	else if((BLACK == frame.colorB) && (NO_INPUT != frame.period) && (NO_INPUT == frame.timeout) && (BLACK != frame.colorA))
	{
		LedBlinkColor(frame.strip, frame.period,  frame.colorA);
	}
	// LED Blink Two Colors
	else if((BLACK != frame.colorB) && (NO_INPUT != frame.period) && (NO_INPUT == frame.timeout) && (BLACK != frame.colorA))
	{
		LedBlinkTwoColors(frame.strip, frame.period, frame.colorA, frame.colorB);
	}
	// LED Blink Color For Time
	else if((BLACK == frame.colorB) && (NO_INPUT != frame.period) && (NO_INPUT != frame.timeout) && (BLACK != frame.colorA))
	{
		LedBlinkColorForTime(frame.strip, frame.period, frame.timeout, frame.colorA);
	}
	// LED Blink Two Colors For Time
	else if((BLACK != frame.colorB) && (NO_INPUT != frame.period) && (NO_INPUT != frame.timeout) && (BLACK != frame.colorA))
	{
		LedBlinkTwoColorsForTime(frame.strip, frame.period, frame.timeout, frame.colorA, frame.colorB);
	}
	// Comando no identificado.
	else
	{
		// TODO: Error Handling
	}
}


void ledqProcess(LEDqueue frame, TickType_t value)
{
	//Identificar el comando recibido 
	switch(frame.mode)
	{
		// Comando LED Set On
		case(LED_SET_ON):
			if( frame.strip < NUMBERoFSTRIPS)
			{
				if(LED_OFF == strips[frame.strip].state)
				{
					blinks[frame.strip] = LED_OFF;
					LedTurnOn(frame.strip, frame.colorA);
					//TODO: Implement
					printf("\r\nEncendido 368 LED Strip # %d",frame.strip);
					strips[frame.strip].state = LED_ON;
				}
			}
		break;

		// Comando LED Set Off
		case(LED_SET_OFF):
			if( frame.strip < NUMBERoFSTRIPS)
			{
				if(LED_ON == strips[frame.strip].state)
				{
					blinks[frame.strip] = LED_OFF;
					LedTurnOff(frame.strip);
					//TODO: Implement
					printf("\r\nApagado 380 LED Strip # %d",frame.strip);
					strips[frame.strip].state = LED_OFF;
				}
			}
		break;

		// Comando LED Blink
		case(LED_BLINK):

			// Guardamos datos para el recall de las funciones			
			blinks[frame.strip] = LED_ON;
			commands[frame.strip] = frame;
			timings[frame.strip] = value;

			//TODO:Implement
			strips[frame.strip].state = LED_ON;
			printf("\r\n LED BLINK ON: ", strips[frame.strip].state);
			
			// Procesamiento del commando
			ledblinkProcess(frame);

		break;

		default:
			// En caso de Error mantener apagado.
			printf("\r\n Command Error");
			blinks[frame.strip] = LED_OFF;
			LedTurnOff(frame.strip);
	}
}


void timeValidation(TickType_t value)
{
	ledState_t led_state;

	// Evaluamos si tenemos algun Blink pendiente
	for(uint8_t i =0; i<NUMBERoFSTRIPS ;i++)
	{
		// Si tenemos pendiente un redisparo pasamos a evaluarlo
		if( LED_ON == blinks[i] )
		{
			printf("\r\nTiming LED: %d", value - timings[i]);
			printf("\r\nTimeout LED: %d", commands[i].timeout);
			// Evaluar el cumplimiento del timeout especificado
			if( ((value - timings[i]) >= commands[i].timeout) && (0 != commands[i].timeout ))
			{
				timings[i] = value;
				led_state = strips[commands[i].strip].state;
				
				// Si el estado es encendido
				if( LED_ON == led_state )
				{
					// TurnOff
					LedTurnOff(commands[i].strip);
					//TODO: Implement
					printf("\r\nApagando 432 LED Strip # %d",commands[i].strip);
					strips[commands[i].strip].state = LED_OFF; 
					
					// Si no se requiere el redisparo lo desmarcamos
					if( !commands[i].foreverFlag)
					{
						blinks[i] = LED_OFF;
					}
				}
				else if((commands[i].foreverFlag))
				{
					// TurnOn
					ledblinkProcess(commands[i]);
					//TODO: Implement
					printf("\r\nEncendido 446 LED Strip # %d",commands[i].strip);
					strips[commands[i].strip].state = LED_ON;
				}
				else
				{
					// Apaguemos por precaucion
					LedTurnOff(commands[i].strip);
					//TODO: Implement
					printf("\r\nApagando 454 LED Strip # %d",commands[i].strip);
					strips[commands[i].strip].state = LED_OFF; 
					blinks[i] = LED_OFF;
				}
			}
		}
	}
}

void LEDInit(void)
{
	ledInit(WHITE);
}

void vLedControlLed(LEDqueue frame, uint8_t enqueque)
{
	static uint8_t init = 0;
	//LEDqueue frame;
	LEDcontrol *pled;
	uint8_t i,idxStart,idxEnd;
	//ledInit(WHITE);
	/*Creo la Queue para gestion de leds */
	//queueLeds = xQueueCreate(LEDqUEUEsIZE,sizeof(LEDqueue));

	//while (1)
	//{
		// TODO: Implementar el control de cada efecto

		// Funcion limitada a 1 llamado por ciclo
		TickType_t timing = xTaskGetTickCount();

		// Si se recibio un comando, se procesa el comando y se toma una accion
		//if( pdPASS ==  xQueueReceive(queueLeds, &frame, LEDqERIODOtASK/portTICK_PERIOD_MS ))
		if( enqueque )
		{
			// Procesar comando proveninete de la Cola
			ledqProcess(frame, timing);
		}
		// Evaluamos si tenemos algun blink pendiente
		timeValidation(timing);

		//vTaskDelay(LEDpERIODOtASK/portTICK_PERIOD_MS);
	//}
}

/*==================[Public functions definition]==========================*/

// Status LedTurnOff(ledStrip_t led){
	
// }


// Status LedTurnOn(ledStrip_t led, ledColour_t color){

// }


// Status LedStayOn(ledStrip_t led, uint16_t timeout, ledColour_t color){

// }


// Status LedBlinkColor(ledStrip_t led,uint16_t period,  ledColour_t colorA){

// }


// Status LedBlinkTwoColors(ledStrip_t led,uint16_t period, ledColour_t colorA, ledColour_t colorB){

// }


// Status LedBlinkColorForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA){

// }


// Status LedBlinkTwoColorsForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA,ledColour_t colorB){

// }


// Status LedGetColor(ledStrip_t led,ledColour_t *color){
// 	return LedGetCurrentColor(led,color);
// }
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
