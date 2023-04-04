/**
 * Date:    03/04/2023
 * Author:  Daniel Marquez
 * e-mail:   damf618@gmail.com
 * 
**/

/*=====[Inclusions of public function dependencies]==========================*/
#include"pca_9532.h"
#include"pca9532_led_driver.h"
#include <string.h>

/*=====[Prototypes (declarations) of private functions]=============*/

Status deff_config(ledColour_t* color, uint8_t rgb_colors[RGB_LEDS] )
{
    return LedColourInit(color, rgb_colors[0], rgb_colors[1], rgb_colors[2]);
}

/*=====[Implementations of private functions]=============*/


/*=====[Implementation of public functions]=======================*/

Status LedStripInit(ledStrip_t* led, pca9532_conf_t * dev, uint16_t pin_conf)
{
    Status rtn = LED_DRIVER_ERROR;

    if((NULL != dev) && (NULL != led))
    {
        led->dev = dev;

        led->pin_addresses = pin_conf;

        pca_9532_enable(led->dev);

        rtn = LED_DRIVER_OK;
    }

    return rtn;
}


Status LedColourInit(ledColour_t* color, uint8_t red, uint8_t green, uint8_t blue )
{
    Status rtn = LED_DRIVER_ERROR;

    if((NULL != color))
    {
        color->rgb_red      = red;
        color->rgb_green    = green; 
        color->rgb_blue     = blue;
        rtn                 = LED_DRIVER_OK;
    }

    return rtn;
}


Status LedColourAuto(ledColour_t* color, color_e deffcolor)
{
    Status rtn = LED_DRIVER_ERROR;

    if((NULL != color))
    {
        if(RED == deffcolor)
        {
            uint8_t colors[] = RGB_RED;
            if( LED_DRIVER_OK == deff_config(color, colors ))
            {
                return LED_DRIVER_OK;
            }
        }
        else if(GREEN == deffcolor)
        {
            uint8_t colors[] = RGB_GREEN;
            if( LED_DRIVER_OK == deff_config(color, colors ))
            {
                return LED_DRIVER_OK;
            }
        }
        else if(BLUE == deffcolor)
        {
            uint8_t colors[] = RGB_BLUE;
            if( LED_DRIVER_OK == deff_config(color, colors ))
            {
                return LED_DRIVER_OK;
            }
        }
        else if(YELLOW == deffcolor)
        {
            uint8_t colors[] = RGB_YELLOW;
            if( LED_DRIVER_OK == deff_config(color, colors ))
            {
                return LED_DRIVER_OK;
            }
        }
        else if(CYAN == deffcolor)
        {
            uint8_t colors[] = RGB_CYAN;
            if( LED_DRIVER_OK == deff_config(color, colors ))
            {
                return LED_DRIVER_OK;
            }
        }
        else if(MAGENTA == deffcolor)
        {
            uint8_t colors[] = RGB_MAGENTA;
            if( LED_DRIVER_OK == deff_config(color, colors ))
            {
                return LED_DRIVER_OK;
            }
        }
        else if(WHITE == deffcolor)
        {
            uint8_t colors[] = RGB_WHITE;
            if( LED_DRIVER_OK == deff_config(color, colors ))
            {
                return LED_DRIVER_OK;
            }
        }
        else if(BLACK == deffcolor)
        {
            uint8_t colors[] = RGB_BLACK;
            if( LED_DRIVER_OK == deff_config(color, colors ))
            {
                return LED_DRIVER_OK;
            }
        }
        
    }

    return rtn;
}


Status LedTurnOff(ledStrip_t led)
{
    uint8_t rtn = LED_DRIVER_ERROR;

    if((LED_DRIVER_OK == pca_9532_pwm_config(led.dev, COLOR_MIN_BRIGHT, BLINK0 )) ||
       (LED_DRIVER_OK == pca_9532_pwm_config(led.dev, COLOR_MIN_BRIGHT, BLINK1 )))
    {
        rtn = LED_DRIVER_OK;
    }
    return rtn;

}


Status LedTurnOn(ledStrip_t led, ledColour_t color)
{
    uint8_t rtn = LED_DRIVER_ERROR;

    if((LED_DRIVER_OK == pca_9532_pwm_config(led.dev, DEFF_COLOR_BRIGHT, BLINK0 )) ||
       (LED_DRIVER_OK == pca_9532_pwm_config(led.dev, DEFF_COLOR_BRIGHT, BLINK1 )))
    {
        rtn = LED_DRIVER_OK;
    }

    if(LED_DRIVER_OK == rtn)
    {
        for(uint8_t i=0; i<MAX_LEDS_STRIP ;i++)
        {
            // If the Led is Connected ...
            if (led.pin_addresses & (1 << i))
            {
                if(PCA_9532_FAIL == pca_9532_led_on(led.dev,i));
                {
                    rtn = LED_DRIVER_ERROR;
                    break;
                }
            }
        }
    }
    return rtn;
}


Status LedStayOn(ledStrip_t led, uint16_t timeout, ledColour_t color);
Status LedBlinkColor(ledStrip_t led,uint16_t period, ledColour_t colorA);
Status LedBlinkTwoColors(ledStrip_t led,uint16_t period, ledColour_t colorA, ledColour_t colorB);
Status LedBlinkColorForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA);
Status LedBlinkTwoColorsForTime(ledStrip_t led,uint16_t period, uint16_t timeout, ledColour_t colorA,ledColour_t colorB);

