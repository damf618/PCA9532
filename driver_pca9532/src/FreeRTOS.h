#ifndef _FREERTOS_H_
#define _FREERTOS_H_

/*==================[inclusions]=============================================*/

#include"stdint.h"
#include"stdbool.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif
/*==================[macros]=================================================*/

#define TRUE 		1
#define FALSE 		0

/*==================[typedef]================================================*/

typedef uint32_t TickType_t;

typedef uint8_t Bool;

/**
 * @ Status type definition
 */
typedef enum {
	ERROR = 0,
	SUCCESS = !ERROR
	} Status;

/*==================[external data declaration]==============================*/


/*==================[external functions declaration]=========================*/

TickType_t xTaskGetTickCount(void);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _FREERTOS_H_ */
