/*************************************************************************//**

  @file     led.c

  @brief    Hardware Library to use Leds

  @author   Alejandro Bujan

  @version  20200501 v0.5.0   AB
 ******************************************************************************/

// Modificaciones para realizar el testing de la parte desarrollada


/*==================[inclusions]=============================================*/
#include "led.h"
#include "led_mock.h"

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

/*==================[internal functions declaration]=========================*/

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


/*==================[internal data definition]===============================*/

ledRGB_t stripsConfigPins[] = {
			{2,1,0}, /* jack */
			{12,13,14}, /*D17vOTARP6*/
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/


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
		// Limpieza de blinks
		blinks[i] = 0;
	}


	/* Condicion Inicial de colores */

	//ledStrip(LED_AUDIO,colour,LEDoN);
	//ledStrip(LED_PCB,colour,LEDoN);

}


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


/** @} doxygen end group definition */
/*==================[end of file]============================================*/
