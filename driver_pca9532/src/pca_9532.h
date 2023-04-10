/**
 * Date:    03/04/2023
 * Author:  Daniel Marquez
 * e-mail:   damf618@gmail.com
 * 
**/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef PCA_9532_H_
#define PCA_9532_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include"stdint.h"
#include"stdbool.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/
#define PCA_9532_I2C_BASE   0xC0
#define PCA_9532_I2C_MAX    0xD4
#define PCA_9532_FREQ_RATE  2
#define PCA_9532_PWM_RATE   2
#define PCA_9532_STATE_REG  4
#define PCA_9532_MAX_PWM    255         //Based on 6.3.4 y 6.3.6 PWMX (pg.7) BLINKX = (PWMX/256)
#define PCA_9532_MIN_PWM    0
#define PCA_9532_MAX_FREQ   255         //Based on 6.3.3 y 6.3.5 PCSX (pg.7) BLINKX = (PCSX + 1)/ 152
#define PCA_9532_MIN_FREQ   0
#define PCA_9532_MIN_INDEX  0
#define PCA_9532_CHANNELS   16

#define PCA_9532_FAIL       0
#define PCA_9532_OK         1
#define PCA_9532_RESET      0
#define PCA_9532_ON         1

#define PCA_9532_ENABLE     0
#define PCA_9532_DISABLE    1

#define PCA_NO_INPUT        0

#ifndef NULL
#define NULL                0
#endif


/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/**
 * @brief Structure to configure a PCA9532 driver
 * 
 */
typedef struct
{
    uint8_t     address;
    uint8_t     ena_pin;
    uint8_t     status;
    uint16_t    state;
    uint8_t     pwm[PCA_9532_PWM_RATE];
    uint8_t     freq[PCA_9532_FREQ_RATE];
}pca9532_conf_t;


/*=====[Prototypes (declarations) of public functions]=======================*/
/**
 * @brief Function to initialize the entire structure.
 * 
 * @warning It must be called before any other Function.
 * @param dev structure to be configured.
 * @param add i2c Address of the device.
 * @param ena_pin pin of the MCU used to RESET the PCA9532.
 * @return uint8_t returns PCA_9532_OK if correct.
 * 
 */
uint8_t LEDSet(pca9532_conf_t* dev, uint8_t add, uint8_t ena_pin);

/**
 * @brief Seteo de DutyCycle para la strip indicada 
 * 
 * @param dev 
 * @param pwm 
 * @return uint8_t 
 */
uint8_t LEDSet_Duty(pca9532_conf_t* dev, uint8_t pwm, uint8_t index );

/**
 * @brief  Seteo de Periodo para la strip indicada 
 * 
 * @param dev 
 * @param freq 
 * @param index 
 * @return uint8_t 
 */
uint8_t LEDSet_Period(pca9532_conf_t* dev, uint8_t freq, uint8_t index );

/**
 * @brief Habilitar IC PCA9532 mediante el pin RESET
 * 
 * @param dev 
 * @return uint8_t 
 */
uint8_t LEDSet_enable(pca9532_conf_t* dev);

/**
 * @brief Deshabilitar IC PCA9532 mediante el pin RESET
 * 
 * @param dev 
 * @return uint8_t 
 */
uint8_t LEDSet_disable(pca9532_conf_t* dev);

/**
 * @brief Encendido de led especificado del IC PCA9532
 * 
 * @param dev 
 * @param led 
 * @return uint8_t 
 */
uint8_t LEDSet_led_on(pca9532_conf_t* dev, uint8_t led);

/**
 * @brief Apagado de led especificado del IC PCA9532
 * 
 * @param dev 
 * @param led 
 * @return uint8_t 
 */
uint8_t LEDSet_led_off(pca9532_conf_t* dev, uint8_t led);

/**
 * @brief Obtener el estado en el que se encuentra cada Output del IC
 * 
 * @param dev 
 * @return uint16_t 
 */
uint16_t get_LEDSet_state(pca9532_conf_t* dev);




/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* PCA_9532_H_ */