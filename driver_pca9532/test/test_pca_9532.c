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
 *   - .
**/

#include "unity.h"
#include "pca_9532.h"
#include "pca9532_led_driver.h"

#define CORRECT_ADDRESS		0xC2
#define INCORRECT_ADDRESS   0xD6

#define ENA_PIN				8
#define INDEX_0				0
#define INDEX_1				1
#define ERROR_INDEX_0		-1
#define ERROR_INDEX_1		2
#define CORRECT_PWM1		0
#define CORRECT_PWM2		180
#define CORRECT_PWM3		255

#define CORRECT_FREQ1		0
#define CORRECT_FREQ2		180
#define CORRECT_FREQ3		255
#define INCORRECT_FREQ		330

pca9532_conf_t dev;
pca9532_conf_t* dev1 = NULL;
uint32_t actual_case = 0;

//This is run before EACH TEST
void setUp(void)
{
	TEST_ASSERT_EQUAL(PCA_9532_OK,pca_9532_init(&dev,CORRECT_ADDRESS,ENA_PIN));
}

//This is run after EACH TEST
void tearDown(void)
{
}

typedef struct Test_Cases_Dir_s
{	
	pca9532_conf_t*	pca;
	uint8_t 		addr;
	uint8_t 		ena;
	uint8_t 		expect_init;
} Test_Cases_Dir_t;

typedef struct Test_Cases_PWM_s
{	
	pca9532_conf_t*	pca;
	uint8_t 		index0;
	uint8_t 		index1;
	uint8_t 		pwm0;
	uint8_t 		pwm1;
	uint8_t 		expect_config0;
	uint8_t 		expect_config1;
} Test_Cases_PWM_t;

typedef struct Test_Cases_Freq_s
{	
	pca9532_conf_t*	pca;
	uint8_t 		index0;
	uint8_t 		index1;
	uint8_t 		freq0;
	uint8_t 		freq1;
	uint8_t 		expect_config0;
	uint8_t 		expect_config1;
} Test_Cases_Freq_t;

typedef struct Test_Cases_LedOff_s
{	
	pca9532_conf_t*	pca;
	uint8_t 		led;
	uint16_t 		state;
	uint8_t 		expect_config;
} Test_Cases_LedOff_t;

typedef struct Test_Cases_LedOn_s
{	
	pca9532_conf_t*	pca;
	uint8_t 		led;
	uint16_t 		state;
	uint8_t 		expect_config;
} Test_Cases_LedOn_t;

static const Test_Cases_PWM_t Test_Cases_PWM[]= 
{
	{
	.pca				= &dev,
	.index0				= INDEX_0,
	.index1				= INDEX_1,
	.pwm0				= CORRECT_PWM1,
	.pwm1				= CORRECT_PWM3,
	.expect_config0		= PCA_9532_OK,
	.expect_config1		= PCA_9532_OK,
	},
	{
	.pca				= NULL,
	.index0				= INDEX_0,
	.index1				= INDEX_1,
	.pwm0				= CORRECT_PWM1,
	.pwm1				= CORRECT_PWM3,
	.expect_config0		= PCA_9532_FAIL,
	.expect_config1		= PCA_9532_FAIL,
	},
	{
	.pca				= &dev,
	.index0				= ERROR_INDEX_0,
	.index1				= INDEX_1,
	.pwm0				= CORRECT_PWM1,
	.pwm1				= CORRECT_PWM2,
	.expect_config0		= PCA_9532_FAIL,
	.expect_config1		= PCA_9532_OK,
	},
	{
	.pca				= &dev,
	.index0				= INDEX_0,
	.index1				= ERROR_INDEX_1,
	.pwm0				= CORRECT_PWM2,
	.pwm1				= CORRECT_PWM3,
	.expect_config0		= PCA_9532_OK,
	.expect_config1		= PCA_9532_FAIL,
	},
};

static const Test_Cases_LedOff_t Test_Cases_LedOff[]= 
{
	{
	.pca				= &dev,
	.led				= PCA_9532_CHANNELS - 5,
	.state				= 0b0000100000000000,
	.expect_config		= PCA_9532_OK,
	},
	{
	.pca				= &dev,
	.led				= PCA_9532_CHANNELS-11,
	.state				= 0b0000000000100000,
	.expect_config		= PCA_9532_OK,
	},
	{
	.pca				= &dev,
	.led				= PCA_9532_CHANNELS - PCA_9532_CHANNELS,
	.state				= 0b0000000000000001,
	.expect_config		= PCA_9532_OK,
	},
	{
	.pca				= &dev,
	.led				= PCA_9532_CHANNELS +1,
	.state				= 0b0000000000000000,
	.expect_config		= PCA_9532_FAIL,
	},
	{
	.pca				= &dev,
	.led				= -1,
	.state				= 0b0001000000000000,
	.expect_config		= PCA_9532_FAIL,
	},
	{
	.pca				= NULL,
	.led				= PCA_9532_CHANNELS-1,
	.state				= 0b0000000011000000,
	.expect_config		= PCA_9532_FAIL,
	},
};

static const Test_Cases_LedOn_t Test_Cases_LedOn[]= 
{
	{
	.pca				= &dev,
	.led				= PCA_9532_CHANNELS - 5,
	.state				= 0b0000100000000000,
	.expect_config		= PCA_9532_OK,
	},
	{
	.pca				= &dev,
	.led				= PCA_9532_CHANNELS-11,
	.state				= 0b0000000000100000,
	.expect_config		= PCA_9532_OK,
	},
	{
	.pca				= &dev,
	.led				= PCA_9532_CHANNELS - PCA_9532_CHANNELS,
	.state				= 0b0000000000000001,
	.expect_config		= PCA_9532_OK,
	},
	{
	.pca				= &dev,
	.led				= PCA_9532_CHANNELS +1,
	.state				= 0b0000000000000000,
	.expect_config		= PCA_9532_FAIL,
	},
	{
	.pca				= &dev,
	.led				= -1,
	.state				= 0b0000000000000000,
	.expect_config		= PCA_9532_FAIL,
	},
	{
	.pca				= NULL,
	.led				= PCA_9532_CHANNELS - 2,
	.state				= 0b0000000000000100,
	.expect_config		= PCA_9532_FAIL,
	},
};

static const Test_Cases_Freq_t Test_Cases_Freq[]= 
{
	{
	.pca				= &dev,
	.index0				= INDEX_0,
	.index1				= INDEX_1,
	.freq0				= CORRECT_FREQ1,
	.freq1				= CORRECT_FREQ3,
	.expect_config0		= PCA_9532_OK,
	.expect_config1		= PCA_9532_OK,
	},
	{
	.pca				= NULL,
	.index0				= INDEX_0,
	.index1				= INDEX_1,
	.freq0				= CORRECT_FREQ1,
	.freq1				= CORRECT_FREQ3,
	.expect_config0		= PCA_9532_FAIL,
	.expect_config1		= PCA_9532_FAIL,
	},
	{
	.pca				= &dev,
	.index0				= ERROR_INDEX_0,
	.index1				= INDEX_1,
	.freq0				= CORRECT_FREQ1,
	.freq1				= CORRECT_FREQ2,
	.expect_config0		= PCA_9532_FAIL,
	.expect_config1		= PCA_9532_OK,
	},
	{
	.pca				= &dev,
	.index0				= INDEX_0,
	.index1				= ERROR_INDEX_1,
	.freq0				= CORRECT_FREQ2,
	.freq1				= CORRECT_PWM3,
	.expect_config0		= PCA_9532_OK,
	.expect_config1		= PCA_9532_FAIL,
	},
};

static const Test_Cases_Dir_t Test_Cases_Dir[]= 
{
	{
		.pca		= &dev,
		.addr		= INCORRECT_ADDRESS,
		.ena		= ENA_PIN,
		.expect_init= PCA_9532_FAIL,
	},
	{
		.pca		= &dev,
		.addr		= CORRECT_ADDRESS,
		.ena		= ENA_PIN,
		.expect_init= PCA_9532_OK,
	},
	{
		.pca		= NULL,
		.addr		= CORRECT_ADDRESS,
		.ena		= ENA_PIN,
		.expect_init= PCA_9532_FAIL,
	},
};


/**
 * @test Prueba de Validacion de Direcciones.
 * 
 * 
 **/
void test_Direcciones(void){
	char Text_ID[30];

	printf("\n *** Inicio de Pruebas de Error: Direcciones  ***");
	
	for(actual_case = 0; actual_case < sizeof(Test_Cases_Dir) / sizeof(struct Test_Cases_Dir_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Dir[actual_case].expect_init,
			pca_9532_init(Test_Cases_Dir[actual_case].pca, Test_Cases_Dir[actual_case].addr ,
						  Test_Cases_Dir[actual_case].ena),Text_ID);
		
		if((Test_Cases_Dir[actual_case].expect_init) == PCA_9532_OK)
		{
			TEST_ASSERT_EQUAL(Test_Cases_Dir[actual_case].addr, dev.address);
			TEST_ASSERT_EQUAL(Test_Cases_Dir[actual_case].ena, dev.ena_pin);
		}
	}


	printf("\n ___ Final de Pruebas de Error: Direcciones   ___");
	
}


/**
 * @test Prueba de Configuracion de PWM.
 * 
 * 
 **/
void test_PWM(void){
	
	char Text_ID[30];
	
	printf("\n *** Inicio de Pruebas de Configuracion PWM  ***");

	for(actual_case = 0; actual_case < sizeof(Test_Cases_PWM) / sizeof(struct Test_Cases_PWM_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_PWM[actual_case].expect_config0,
			pca_9532_pwm_config(  Test_Cases_PWM[actual_case].pca, Test_Cases_PWM[actual_case].pwm0 ,
						  		  Test_Cases_PWM[actual_case].index0),Text_ID);

		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_PWM[actual_case].expect_config1,
			pca_9532_pwm_config(  Test_Cases_PWM[actual_case].pca, Test_Cases_PWM[actual_case].pwm1 ,
						  		  Test_Cases_PWM[actual_case].index1),Text_ID);
		
		if((Test_Cases_PWM[actual_case].expect_config0) == PCA_9532_OK)
		{
			TEST_ASSERT_EQUAL(Test_Cases_PWM[actual_case].pwm0, dev.pwm[INDEX_0]);
		}

		if((Test_Cases_PWM[actual_case].expect_config1) == PCA_9532_OK)
		{
			TEST_ASSERT_EQUAL(Test_Cases_PWM[actual_case].pwm1, dev.pwm[INDEX_1]);
		}
	}
	
	printf("\n ___ Final de Pruebas de Configuracion PWM   ___");
	
}


/**
 * @test Prueba de Configuracion de PWM.
 * 
 * 
 **/
void test_Freq(void){
	
	char Text_ID[30];
	
	printf("\n *** Inicio de Pruebas de Configuracion FREQ  ***");

	for(actual_case = 0; actual_case < sizeof(Test_Cases_Freq) / sizeof(struct Test_Cases_Freq_s); actual_case++)
	{
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Freq[actual_case].expect_config0,
			pca_9532_freq_config( Test_Cases_Freq[actual_case].pca, Test_Cases_Freq[actual_case].freq0 ,
						  		  Test_Cases_Freq[actual_case].index0),Text_ID);

		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_Freq[actual_case].expect_config1,
			pca_9532_freq_config( Test_Cases_Freq[actual_case].pca, Test_Cases_Freq[actual_case].freq1 ,
						  		  Test_Cases_Freq[actual_case].index1),Text_ID);
		
		if((Test_Cases_Freq[actual_case].expect_config0) == PCA_9532_OK)
		{
			TEST_ASSERT_EQUAL(Test_Cases_Freq[actual_case].freq0, dev.freq[INDEX_0]);
		}

		if((Test_Cases_Freq[actual_case].expect_config1) == PCA_9532_OK)
		{
			TEST_ASSERT_EQUAL(Test_Cases_Freq[actual_case].freq1, dev.freq[INDEX_1]);
		}
	}
	
	printf("\n ___ Final de Pruebas de Configuracion FREQ   ___");
}



/**
 * @test Prueba de Encendido de LED.
 * 
 * 
 **/
void test_LedOn(void){
	
	char Text_ID[30];

	printf("\n *** Inicio de Pruebas de Encendido de LED  ***");

	for(actual_case = 0; actual_case < sizeof(Test_Cases_LedOn) / sizeof(struct Test_Cases_LedOn_s); actual_case++)
	{
		//TODO: Turn OFF to validate the Test.
		dev.state = 0x0;
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_LedOn[actual_case].expect_config,
			pca_9532_led_on( Test_Cases_LedOn[actual_case].pca, Test_Cases_LedOn[actual_case].led),
						  	  Text_ID);

		
		if((Test_Cases_LedOn[actual_case].expect_config) == PCA_9532_OK)
		{
			TEST_ASSERT_EQUAL_HEX16_MESSAGE(Test_Cases_LedOn[actual_case].state,
											get_pca_9532_state(Test_Cases_LedOn[actual_case].pca),Text_ID);
		}
	}
	
	printf("\n ___ Final de Pruebas de Encendido de LED   ___");
}



/**
 * @test Prueba de Apagado de LED.
 * 
 * 
 **/
void test_LedOff(void){
	
	char Text_ID[30];
	
	printf("\n *** Inicio de Pruebas de Apagado de LED  ***");

	for(actual_case = 0; actual_case < sizeof(Test_Cases_LedOff) / sizeof(struct Test_Cases_LedOff_s); actual_case++)
	{
		//TODO: Turn ON to validate the Test.
		dev.state = 0xFFFF;
		printf("\n *** Ejecutando Caso de Prueba Nro: %d ***",actual_case);
		sprintf(Text_ID,"Caso de Prueba Nro: %d",actual_case);

		TEST_ASSERT_EQUAL_MESSAGE(Test_Cases_LedOff[actual_case].expect_config,
			pca_9532_led_off( Test_Cases_LedOff[actual_case].pca, Test_Cases_LedOff[actual_case].led),
						  	  Text_ID);

		
		if((Test_Cases_LedOff[actual_case].expect_config) == PCA_9532_OK)
		{
			TEST_ASSERT_EQUAL_HEX16_MESSAGE(Test_Cases_LedOff[actual_case].state,
											~(get_pca_9532_state(Test_Cases_LedOff[actual_case].pca)),Text_ID);
		}
	}
	
	printf("\n ___ Final de Pruebas de Apagado de LED  ___");
}
