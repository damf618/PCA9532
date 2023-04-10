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

#include "unity.h"
#include "pca_9532.h"
#include "pca9532_led_driver.h"

#define CORRECT_ADDRESS		0xC2

#define PIN_CONFIG			0xF0F0

#define ENA_PIN				8
#define INDEX_0				0
#define INDEX_1				1
#define PWM_VAL				180
#define FREQ_VAL			180

#define RED_COLOR			255
#define GREEN_COLOR			0
#define BLUE_COLOR			152

pca9532_conf_t 	dev;
ledStrip_t		led;
ledColour_t 	color;
uint32_t actual_case = 0;


//This is run before EACH TEST
void setUp(void)
{
	TEST_ASSERT_EQUAL(PCA_9532_OK,LEDSet(&dev,CORRECT_ADDRESS,ENA_PIN));
}

//This is run after EACH TEST
void tearDown(void)
{
}

typedef struct Test_Cases_Init_s
{	
	pca9532_conf_t*	pca;
	ledStrip_t*		led;
	uint16_t		pin_conf;
	uint8_t 		expect_init;
} Test_Cases_Init_t;

static const Test_Cases_Init_t Test_Cases_Init[]= 
{
	{
		.pca		= &dev,
		.led		= &led,
		.pin_conf	= PIN_CONFIG,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pca		= &dev,
		.led		= NULL,
		.pin_conf	= PIN_CONFIG - 0b0111000011000000 ,
		.expect_init= LED_DRIVER_ERROR,
	},
	{
		.pca		= NULL,
		.led		= &led,
		.pin_conf	= PIN_CONFIG,
		.expect_init= LED_DRIVER_ERROR,
	},
};

typedef struct Test_Cases_Color_s
{	
	ledColour_t*	color;
	uint8_t			red;
	uint8_t			green;
	uint8_t			blue;
	uint8_t 		expect_init;
} Test_Cases_Color_t;

typedef struct Test_Cases_Color_Auto_s
{	
	ledColour_t*	color;
	color_e			deffcolor;
	uint8_t			red;
	uint8_t			green;
	uint8_t			blue;
	uint8_t 		expect_init;
} Test_Cases_Color_Auto_t;

typedef struct Test_Cases_LedOn_s
{	
	uint16_t		pin_conf;
	uint16_t		state;
	uint8_t 		expect_init;
} Test_Cases_LedOn_t;

typedef struct Test_Cases_LedOff_s
{	
	uint16_t		pin_conf;
	uint16_t		state;
	uint8_t 		expect_init;
} Test_Cases_LedOff_t;


static const Test_Cases_Color_Auto_t Test_Cases_Color_Auto[]= 
{
	{
		.color		= &color,
		.deffcolor	= YELLOW,
		.red		= DEFF_COLOR_BRIGHT,
		.green		= DEFF_COLOR_BRIGHT,
		.blue		= 0,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.color		= &color,
		.deffcolor	= RED,
		.red		= DEFF_COLOR_BRIGHT,
		.green		= 0,
		.blue		= 0,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.color		= &color,
		.deffcolor	= WHITE,
		.red		= DEFF_COLOR_BRIGHT,
		.green		= DEFF_COLOR_BRIGHT,
		.blue		= DEFF_COLOR_BRIGHT,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.color		= NULL,
		.deffcolor	= YELLOW,
		.red		= DEFF_COLOR_BRIGHT,
		.green		= DEFF_COLOR_BRIGHT,
		.blue		= 0,
		.expect_init= LED_DRIVER_ERROR,
	},
	{
		.color		= &color,
		.deffcolor	= 29,
		.red		= DEFF_COLOR_BRIGHT,
		.green		= 0,
		.blue		= 0,
		.expect_init= LED_DRIVER_ERROR,
	},
};


static const Test_Cases_Color_t Test_Cases_Color[]= 
{
	{
		.color		= &color,
		.red		= RED_COLOR,
		.green		= GREEN_COLOR,
		.blue		= BLUE_COLOR,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.color		= NULL,
		.red		= RED_COLOR,
		.green		= GREEN_COLOR,
		.blue		= BLUE_COLOR,
		.expect_init= LED_DRIVER_ERROR,
	},
	{
		.color		= &color,
		.red		= GREEN_COLOR,
		.green		= BLUE_COLOR,
		.blue		= RED_COLOR,
		.expect_init= LED_DRIVER_OK,
	},
};


static const Test_Cases_LedOn_t Test_Cases_LedOn[]= 
{
	{
		.pin_conf	= PIN_CONFIG,
		.state		= PIN_CONFIG,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG + 0x0003,
		.state		= PIN_CONFIG + 0x0003,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG + 0x0F0F,
		.state		= PIN_CONFIG + 0x0F0F,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG + 0x00FF,
		.state		= PIN_CONFIG + 0x00FF,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG - 0xF000,
		.state		= PIN_CONFIG - 0xF000,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG - 0x00F0,
		.state		= PIN_CONFIG - 0x00F0,
		.expect_init= LED_DRIVER_OK,
	},
};


static const Test_Cases_LedOff_t Test_Cases_LedOff[]= 
{
	{
		.pin_conf	= PIN_CONFIG,
		.state		= 0,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG + 0x0003,
		.state		= 0,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG + 0x0F0F,
		.state		= 0,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG + 0x00FF,
		.state		= 0,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG - 0xF000,
		.state		= 0,
		.expect_init= LED_DRIVER_OK,
	},
	{
		.pin_conf	= PIN_CONFIG - 0x00F0,
		.state		= 0,
		.expect_init= LED_DRIVER_OK,
	},
};


/**
 * @test Prueba de Validacion de Inicializacion.
 * 
 * 
 **/
void test_Init(void){
	char Text_ID[30];

	printf("\n *** Inicio de Pruebas de VtaskLogic  ***");

	for(actual_case = 0; actual_case < sizeof(Test_Cases_Init) / sizeof(struct Test_Cases_Init_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		// Validacion Inicializacion de Strip
		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Init[actual_case].expect_init,
				LedStripInit(Test_Cases_Init[actual_case].led,
							 Test_Cases_Init[actual_case].pca,
							 Test_Cases_Init[actual_case].pin_conf),Text_ID);
		
		if((Test_Cases_Init[actual_case].expect_init) == LED_DRIVER_OK)
		{
			// Validacion de configuracion de estructura de Strip
			TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Init[actual_case].pin_conf, 
								  led.pin_addresses,Text_ID);
		}
	}

	printf("\n ___ Final de Pruebas de VtaskLogic   ___");
	
}


/**
 * @test Prueba de Validacion de Color Configuration.
 * 
 * 
 **/
void test_Color(void){
	char Text_ID[30];

	printf("\n *** Inicio de Pruebas de Color Config  ***");

	for(actual_case = 0; actual_case < sizeof(Test_Cases_Color_Auto) / sizeof(struct Test_Cases_Color_Auto_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		// Validacion de Inicializacion de Colores Automaticos
		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Color_Auto[actual_case].expect_init,
				LedColourAuto(Test_Cases_Color_Auto[actual_case].color,
							 Test_Cases_Color_Auto[actual_case].deffcolor),Text_ID);
		
		// Validacion de EStrucutra de Colores Automaticos
		if((Test_Cases_Color_Auto[actual_case].expect_init) == LED_DRIVER_OK)
		{
			TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Color_Auto[actual_case].red, 
								  	  color.rgb_red,Text_ID);
			TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Color_Auto[actual_case].green, 
								  	  color.rgb_green,Text_ID);
			TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Color_Auto[actual_case].blue, 
								  	  color.rgb_blue,Text_ID);
		}
	}

	for(actual_case = 0; actual_case < sizeof(Test_Cases_Color) / sizeof(struct Test_Cases_Color_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		// Validacion de Inicializacion de Colores Custom
		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Color[actual_case].expect_init,
				LedColourInit(Test_Cases_Color[actual_case].color,
							 Test_Cases_Color[actual_case].red,
							 Test_Cases_Color[actual_case].green,
							 Test_Cases_Color[actual_case].blue),Text_ID);
		
		// Validacion de EStrucutra de Colores Custom
		if((Test_Cases_Color[actual_case].expect_init) == LED_DRIVER_OK)
		{
			TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Color[actual_case].red, 
								  	  color.rgb_red,Text_ID);
			TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Color[actual_case].green, 
								  	  color.rgb_green,Text_ID);
			TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Color[actual_case].blue, 
								  	  color.rgb_blue,Text_ID);
		}
	}

	printf("\n ___ Final de Pruebas de Color Configuration   ___");
	
}


/**
 * @test Prueba de Validacion de encendido.
 * 
 * 
 **/
void test_Led_On(void)
{
	char Text_ID[30];
	
	printf("\n *** Inicio de Pruebas de Encendido de LED Strip  ***");

	for(actual_case = 0; actual_case < sizeof(Test_Cases_LedOn) / sizeof(struct Test_Cases_LedOn_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		// Inicializacion de IC PCA9532
		TEST_ASSERT_EQUAL(PCA_9532_OK,LEDSet(&dev,CORRECT_ADDRESS,ENA_PIN));

		// Inicializacion de LED Strip
		TEST_ASSERT_EQUAL_MESSAGE(LED_DRIVER_OK,
				LedStripInit(&led, &dev, Test_Cases_LedOn[actual_case].pin_conf),Text_ID);
		
		// Inicializacion de LED Color Automatico
		TEST_ASSERT_EQUAL_MESSAGE(LED_DRIVER_OK,
				LedColourAuto(&color, MAGENTA), Text_ID);

		// Comando de Encendido de LED Color Automatico
		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_LedOn[actual_case].expect_init,
				LedTurnOn(led, color),Text_ID);
		
		if((Test_Cases_LedOn[actual_case].expect_init) == LED_DRIVER_OK)
		{
			// Validacion de estructura de estado en ON.
			TEST_ASSERT_EQUAL_HEX16_MESSAGE(Test_Cases_LedOn[actual_case].state, 
								  	  get_LedState(&led),Text_ID);
		}
	}

	printf("\n ___ Final de Pruebas de Encendido de LED Strip ___");
}


/**
 * @test Prueba de Validacion de apagado.
 * 
 * 
 **/
void test_Led_Off(void)
{
	char Text_ID[30];
	
	printf("\n *** Inicio de Pruebas de Apagado de LED Strip ***");

	for(actual_case = 0; actual_case < sizeof(Test_Cases_LedOff) / sizeof(struct Test_Cases_LedOff_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		// Inicializacion de IC PCA9532
		TEST_ASSERT_EQUAL(PCA_9532_OK,LEDSet(&dev,CORRECT_ADDRESS,ENA_PIN));

		// Inicializacion de LED Strip
		TEST_ASSERT_EQUAL_MESSAGE(LED_DRIVER_OK,
				LedStripInit(&led, &dev, Test_Cases_LedOff[actual_case].pin_conf),Text_ID);
		
		// Inicializacion de LED Color Automatico
		TEST_ASSERT_EQUAL_MESSAGE(LED_DRIVER_OK,
				LedColourAuto(&color, MAGENTA), Text_ID);
		
		// Comando de Encendido de LED Color Automatico
		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_LedOff[actual_case].expect_init,
				LedTurnOn(led, color),Text_ID);

		// Comando de Apagado de LED Color Automatico
		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_LedOff[actual_case].expect_init,
				LedTurnOff(led),Text_ID);

		
		if((Test_Cases_LedOff[actual_case].expect_init) == LED_DRIVER_OK)
		{
			// Validacion de estructura de estado en OFF
			TEST_ASSERT_EQUAL_HEX16_MESSAGE(Test_Cases_LedOff[actual_case].state, 
								  	  get_LedState(&led),Text_ID);
		}
	}

	printf("\n ___ Final de Pruebas de Apagado de LED Strip ___");
}


