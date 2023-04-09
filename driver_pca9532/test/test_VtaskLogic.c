/**
 * Date:    03/04/2023
 * Author:  Daniel Marquez
 * e-mail:   damf618@gmail.com
 *    Ceedling:: 0.31.1
 *    Unity:: 2.5.4
 *    CMock:: 2.5.4
 *    CException:: 1.3.3
 * 
 * Driver for PCA9532 with RTOS on mind.
 * 
 * Considerations:
 *   - Internal MUTEX for I2C Comms.
 *   - Object Oriented in "C" Struct instead of "C++"" Classes.
 *   - Reset Implementation and Connection.
 *   - Frequency Range: 0.591 Hz to 152 Hz.
 *   - PWM Range: 0 to 255
 *   - Doxygen Documentation.
 *   - Possible I2C Addresses 192 + [0 to 2*( 1 to 7)]
**/

/**
 **     ===== Casos de Prueba ===== 
            ---¡COMPLETADAS!--- 
 * -- 
 *
 
 **     ===== Casos de Prueba =====
            ---¡PENDIENTES! ---
 * ____________  Function  ____________
 * -- Function "LEDSet_Period".
 * -- Function "LEDSet_Duty".
 * -- Function "LEDSet".
 * -- Function "VLedControlLed".
 * -- Function "ledColour".
 * 
 * ____________ Data Struct ____________
 * -- ledStrip_t: 	Structure that contains all the data needed to control a LED STRIP
 * -- ledColour_t:	Structure that contains all the data needed to configure a LED RGB COLOR
 * 
 * ____________   Macros    ____________
 * -- MACROS LEDpERIODOtASK
 *
 * ____________    Task     ____________
 * -- Task “VLedControlLed”
 * 
 * ____________  Constraints  ____________
 * -- Encender y apagar una tira de led con un color de la lista.
 * -- Hacer blinkear una tira de led con un periodo y por un tiempo determinado.
 * -- Implementar estas funciones, que serán utilizadas por otras tareas a través de las APIs definidas. 
 *      # Status LedTurnOff(ledStrip_t led);
 *      # Status LedTurnOn(ledStrip_t led, ledColour_t color);
 *      # Status LedStayOn(ledStrip_t led, uint16_t timeout, ledColour_t color);
 *      # Status LedBlinkColor(ledStrip_t led,uint16_t period, ledColour_t colorA);
 *      # Status LedBlinkTwoColors(ledStrip_t led,uint16_t period, ledColour_t colorA, ledColour_t colorB);
 *      # Status LedBlinkColorForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA);
 *      # Status LedBlinkTwoColorsForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA,ledColour_t colorB);
 *
**/

#include "unity.h"
#include "led.h"
#include "mock_FreeRTOS.h"
#include "mock_led_mock.h"

#define RETURN_VAL 		0
#define N_DELAYS		3
#define QUEUE_MSG		1
#define NO_QUEUE_MSG	0

#define DELAY_TIME1		{1, 40, 1000}
#define TIMEOUT_INDEX1	2
#define TIMEOUT1		75
#define PERIOD1			180

uint32_t actual_case = 0;

extern LEDcontrol strips[NUMBERoFSTRIPS];
//Custom
extern TickType_t timings[NUMBERoFSTRIPS];
extern LEDqueue commands[NUMBERoFSTRIPS];
extern uint8_t blinks[NUMBERoFSTRIPS];

typedef enum
{
	BLINK_ONE,
	BLINK_TWO,
	ON_TIMEOUT,
	BLINK_ONE_PER,
	BLINK_TWO_PER
}Blink_Func;

//This is run before EACH TEST
void setUp(void)
{
	// Inicializar Casos de Prueba
	LEDInit();
}

//This is run after EACH TEST
void tearDown(void)
{
}

typedef struct Test_Cases_LED_s
{	
	uint8_t 	strip;
	uint16_t 	period;
	uint16_t 	timeout;
	ledMode_t	mode;
	ledColour_t colorA;
	ledColour_t colorB;
	Bool		foreverFlag;
	TickType_t	delay[N_DELAYS];
	Blink_Func	call;
	uint8_t		queue[N_DELAYS];
} Test_Cases_LED_t;
	
static const Test_Cases_LED_t Test_Cases_LED[]= 
{
	//0 LEDON
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= 0,
		.mode			= LED_SET_ON,
		.colorA			= VIOLET,
		.colorB			= BLACK,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= RETURN_VAL,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,QUEUE_MSG},
	},
	//1 LEDON Extra Prmtrs
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= TIMEOUT1,
		.mode			= LED_SET_ON,
		.colorA			= VIOLET,
		.colorB			= BLACK,
		.foreverFlag	= 1,
		.delay			= DELAY_TIME1,
		.call			= RETURN_VAL,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,QUEUE_MSG},
	},
	//2 LEDOFF
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= 0,
		.mode			= LED_SET_OFF,
		.colorA			= BLUE,
		.colorB			= BLACK,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= RETURN_VAL,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,QUEUE_MSG},
	},
	//3 LEDOFF Extra Prmtrs
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= TIMEOUT1,
		.mode			= LED_SET_OFF,
		.colorA			= BLUE,
		.colorB			= BLACK,
		.foreverFlag	= 1,
		.delay			= DELAY_TIME1,
		.call			= RETURN_VAL,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,QUEUE_MSG},
	},
	//4 LED_BLINK 2 Color TIMEOUT Forever
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= TIMEOUT1,
		.mode			= LED_BLINK,
		.colorA			= GREEN,
		.colorB			= INDIGO,
		.foreverFlag	= 1,
		.delay			= DELAY_TIME1,
		.call			= BLINK_TWO_PER,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,NO_QUEUE_MSG},
	},
	//5 LED_BLINK 1 Color Forever
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= 0,
		.mode			= LED_BLINK,
		.colorA			= GREEN,
		.colorB			= BLACK,
		.foreverFlag	= 1,
		.delay			= DELAY_TIME1,
		.call			= BLINK_ONE,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,NO_QUEUE_MSG},
	},
	//6 LED_BLINK 1 Color Once
	{
		.strip 			= 0,
		.period			= 0,
		.timeout		= TIMEOUT1,
		.mode			= LED_BLINK,
		.colorA			= WHITE,
		.colorB			= BLACK,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= ON_TIMEOUT,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,NO_QUEUE_MSG},
	},
	//7 LED_BLINK 2 Color TIMEOUT
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= TIMEOUT1,
		.mode			= LED_BLINK,
		.colorA			= GREEN,
		.colorB			= INDIGO,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= BLINK_TWO_PER,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,NO_QUEUE_MSG},
	},
	//8 LED_BLINK 1 Color Once
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= 0,
		.mode			= LED_BLINK,
		.colorA			= GREEN,
		.colorB			= BLACK,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= BLINK_ONE,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,NO_QUEUE_MSG},
	},
	//9 LED_BLINK 1 Color Once
	{
		.strip 			= 0,
		.period			= 0,
		.timeout		= TIMEOUT1,
		.mode			= LED_BLINK,
		.colorA			= WHITE,
		.colorB			= BLACK,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= ON_TIMEOUT,
		.queue			= {QUEUE_MSG,NO_QUEUE_MSG,NO_QUEUE_MSG},
	},
	//10 LED_BLINK 2 Color TIMEOUT
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= TIMEOUT1,
		.mode			= LED_BLINK,
		.colorA			= GREEN,
		.colorB			= INDIGO,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= BLINK_TWO_PER,
		.queue			= {QUEUE_MSG,QUEUE_MSG,NO_QUEUE_MSG},
	},
	//11 LED_BLINK 1 Color Once
	{
		.strip 			= 0,
		.period			= PERIOD1,
		.timeout		= 0,
		.mode			= LED_BLINK,
		.colorA			= GREEN,
		.colorB			= BLACK,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= BLINK_ONE,
		.queue			= {QUEUE_MSG,QUEUE_MSG,NO_QUEUE_MSG},
	},
	//12 LED_BLINK 1 Color Once
	{
		.strip 			= 0,
		.period			= 0,
		.timeout		= TIMEOUT1,
		.mode			= LED_BLINK,
		.colorA			= WHITE,
		.colorB			= BLACK,
		.foreverFlag	= 0,
		.delay			= DELAY_TIME1,
		.call			= ON_TIMEOUT,
		.queue			= {QUEUE_MSG,QUEUE_MSG,NO_QUEUE_MSG},
	},
};

void Enum_to_Expect(uint8_t mode)
{

	switch(mode)
	{
		case (LED_SET_ON):
			LedTurnOn_IgnoreAndReturn(RETURN_VAL);
		break;

		case (LED_SET_OFF):
			LedTurnOff_IgnoreAndReturn(RETURN_VAL);
		break;

		case (LED_BLINK):

			switch(Test_Cases_LED[actual_case].call)
			{
				case(BLINK_ONE):
					LedBlinkColor_IgnoreAndReturn(RETURN_VAL);
				break;

				case(BLINK_TWO):
					LedBlinkTwoColors_IgnoreAndReturn(RETURN_VAL);
				break;

				case(BLINK_ONE_PER):
					LedBlinkColorForTime_IgnoreAndReturn(RETURN_VAL);
				break;

				case(BLINK_TWO_PER):
					LedBlinkTwoColorsForTime_IgnoreAndReturn(RETURN_VAL);
				break;

				case(ON_TIMEOUT):
					LedStayOn_IgnoreAndReturn(RETURN_VAL);
				break;

				default:
				TEST_FAIL_MESSAGE("Por favor Revisar el Test Case");	
			}
		break;

		default:
			LedTurnOff_IgnoreAndReturn(RETURN_VAL);
	}		
}

void Pre_Blink_Test(Test_Cases_LED_t Test, uint8_t idx, uint8_t cases)
{
	static uint8_t int_state[NUMBERoFSTRIPS] = {0,0};
	TickType_t 	current_time 	= Test.delay[idx];
	TickType_t 	timeout 		= Test.timeout;
	TickType_t 	timing 			= timings[Test_Cases_LED[cases].strip];
	uint8_t 	state			= strips[Test_Cases_LED[cases].strip].state;
	char		txt[60];
	
	if( (current_time - timing) >= timeout)
	{
		if(state)
		{
			//Funciones que requieren de un redisparo
			if( (BLINK_ONE_PER == Test.call) || (BLINK_TWO_PER == Test.call) || (ON_TIMEOUT == Test.call) )
			{
				printf("\r\n --- Turn Off expected --- ");
				LedTurnOff_IgnoreAndReturn(RETURN_VAL);	
			}
		}
	}
}

void Blink_Test(Test_Cases_LED_t Test, uint8_t idx, uint8_t cases)
{
	char		txt[60];
	sprintf(txt,"Caso de Prueba Nro: %d Interno: %d",cases,idx);

	if( TIMEOUT_INDEX1 == idx )
	{
		if(( BLINK_ONE != Test.call) && ( BLINK_TWO != Test.call))
		{
			TEST_ASSERT_EQUAL_MESSAGE(LED_OFF, strips[Test_Cases_LED[cases].strip].state, txt);
		}
		else
		{
			TEST_ASSERT_EQUAL_MESSAGE(LED_ON, strips[Test_Cases_LED[cases].strip].state, txt);	
		}
	}
	else
	{
		TEST_ASSERT_EQUAL_MESSAGE(LED_ON, strips[Test_Cases_LED[cases].strip].state, txt);
	}
}

/**
 * @test Prueba de Validacion de Comandos, Logica y Tiempos.
 * 
 * 
 **/
void test_Logic(void){
	char Text_ID[30];

	printf("\n *** Inicio de Pruebas de vTaskLogic  ***");

	LEDqueue frame;

	// Inicializar Casos de Prueba
	LEDInit();

	for(actual_case = 0; actual_case < sizeof(Test_Cases_LED) / sizeof(struct Test_Cases_LED_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		frame.colorA 		= Test_Cases_LED[actual_case].colorA;
		frame.colorB 		= Test_Cases_LED[actual_case].colorB;
		frame.foreverFlag 	= Test_Cases_LED[actual_case].foreverFlag;
		frame.mode 			= Test_Cases_LED[actual_case].mode;
		frame.period 		= Test_Cases_LED[actual_case].period;
		frame.strip 		= Test_Cases_LED[actual_case].strip;
		frame.timeout 		= Test_Cases_LED[actual_case].timeout;

		for(uint8_t i =0; i<N_DELAYS ;i++)
		{
			Enum_to_Expect(Test_Cases_LED[actual_case].mode);
			Pre_Blink_Test(Test_Cases_LED[actual_case], i, actual_case);
			xTaskGetTickCount_ExpectAndReturn(Test_Cases_LED[actual_case].delay[i]);
			vLedControlLed(frame, Test_Cases_LED[actual_case].queue[i]);
			
			if(LED_BLINK == Test_Cases_LED[actual_case].mode)
			{
				Blink_Test(Test_Cases_LED[actual_case], i, actual_case);
			}
		}
	}

	printf("\n ___ Final de Pruebas de vTaskLogic   ___");
	
}

