/**
 * Date:    03/04/2023
 * Author:  Daniel Marquez
 * e-mail:   damf618@gmail.com
 * 
**/
/*=====[Inclusions of public function dependencies]==========================*/
#include "pca_9532.h"

/*=====[Implementation of public functions]=======================*/

// Init Function
uint8_t LEDSet(pca9532_conf_t* dev, uint8_t add, uint8_t ena)
{
    uint8_t rtn = PCA_9532_FAIL;

    //NULL Validation
    if(NULL != dev )
    {

        //minimum ADDRESS Validation
        if( (PCA_9532_I2C_BASE <= add) && (PCA_9532_I2C_MAX >= add))
        {
            dev->address = add;
            rtn = PCA_9532_OK;
        }

        //TODO: Include i2C validation of the address.

        dev->ena_pin = ena;
        LEDSet_disable(dev);
        dev->state = 0;
    }

    return rtn;
}


// PWM Configuration Function
uint8_t LEDSet_Duty(pca9532_conf_t* dev, uint8_t pwm, uint8_t index )
{
    uint8_t rtn = PCA_9532_FAIL;
    
    //NULL Validation
    if(NULL != dev)
    {
        //Index Validation
        if((PCA_9532_MIN_INDEX <= index) && (PCA_9532_FREQ_RATE > index))
        {
            //PWM Max Validation
            if(( PCA_9532_MIN_PWM <= pwm) && (PCA_9532_MAX_PWM >= pwm))
            {
                dev->pwm[index] = pwm;
                rtn = PCA_9532_OK;
            }

            //TODO: I2C PWM Command
        }
    }

    return rtn;
}


// FREQ Configuration Function
uint8_t LEDSet_Period(pca9532_conf_t* dev, uint8_t freq, uint8_t index )
{
    uint8_t rtn = PCA_9532_FAIL;
    
    //NULL Validation
    if(NULL != dev)
    {
        //Index Validation
        if((PCA_9532_MIN_INDEX <= index) && (PCA_9532_FREQ_RATE > index))
        {
            //FREQ Max Validation
            if(( PCA_9532_MIN_FREQ <= freq) && (PCA_9532_MAX_FREQ >= freq))
            {
                dev->freq[index] = freq;
                rtn = PCA_9532_OK;

                //TODO: I2C FREQ Command
            }
        }
    }

    return rtn;
}


// IC Enable
uint8_t LEDSet_enable(pca9532_conf_t* dev)
{
    uint8_t rtn = PCA_9532_FAIL;
    
    //NULL Validation
    if(NULL != dev)
    {
        //TODO: GPIO Implementation OFF
        dev->status = PCA_9532_RESET;
    }
}


// IC Disable
uint8_t LEDSet_disable(pca9532_conf_t* dev)
{
    uint8_t rtn = PCA_9532_FAIL;
    
    //NULL Validation
    if(NULL != dev)
    {
        //TODO: GPIO Implementation ON
        dev->status = PCA_9532_ON;
    }
}


// PCA9532 LED ON
uint8_t LEDSet_led_on(pca9532_conf_t* dev, uint8_t led)
{
    uint8_t rtn = PCA_9532_FAIL;

    if(NULL != dev)
    {
        if((0 <= led) && (PCA_9532_CHANNELS > led))
        {
            //TODO: I2C Command and validation
            rtn = PCA_9532_OK;

            //Actualizo el estado del GPIO
            dev->state |= (1 << led);
        }
    }

    return rtn;
}


// PCA9532 LED OFF
uint8_t LEDSet_led_off(pca9532_conf_t* dev, uint8_t led)
{
    uint8_t rtn = PCA_9532_FAIL;

    if(NULL != dev)
    {
        if((0 <= led) && (PCA_9532_CHANNELS > led))
        {
            //TODO: I2C Command and validation
            rtn = PCA_9532_OK;

            //Actualizo el estado del GPIO
            dev->state &= ~(1 << led);
        }
    }

    return rtn;
}


// PCA9532 STATE
uint16_t get_LEDSet_state(pca9532_conf_t* dev)
{
    if(NULL != dev)
    {
        return dev->state;
    }
}