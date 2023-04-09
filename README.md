# PCA9532 Driver
**No releases yet**
- [ ] Biblioteca HAL PCA9532.
	- [x] Testing. 
	- [ ] Implementacion. 
- [ ] Biblioteca de Aplicacion de LED Driver.
	- [x] Testing.
	- [ ] Implementacion.
- [ ] Analisis de FreeRTOS.
	- [x] Diagrama.
	- [x] Testing de FreeRTOS Task.
	- [ ] Implementacion.      

Desarrollo de un driver para el manejo del IC [PCA9532](https://www.nxp.com/docs/en/data-sheet/PCA9532.pdf). 

Herramientas Necesarias:
- Ceedling: para el Testing.
- Doxygen para la documentacion.


# Estrategia

Se analizan 3 posibilidades para el manejo de los efectos LEDS:

 1. Creacion de Tareas OneShot.
 2. Creacion de Timers de FreeRTOS.
 3. Manejo local en vTaskController.

## Comparaciones

| Opcion | Ventajas | Desventajas | Seleccionada |
|--|--|--|--|
| OneShot | Facil Implementacion | Requiere memoria dinamica y el borrado de tareas  | - |
| Timers | Manejo delegado al kernel | Requiere memoria dinamica y el borrado de timers | - |
| Local | Memoria estatica  | No hace uso de los recursos de RTOS | X |

Basado en que las aplicaciones de MSA son principalmente **sistemas criticos**, se opto por una version mas segura. Se da prioridad a la opcion 3, ya que no requiere de Memoria Dinamica, de esta forma los recursos (memoria) quedan disponibles para tareas de mayor prioridad que el encendido de un LED.  

## Diagrama
Diagrama de arquitectura del driver:

```mermaid
graph LR
A[FreeRTOS Tasks] -- Enqueue Comandos LEDS --> B(VtaskController)

B -- Validacion de Timeouts--> D(Blink Driver)
B -- Ejecucion de Comandos --> C(LED Driver) 
C --> E(HAL PCA9532) 
```
