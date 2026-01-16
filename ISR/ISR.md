# ℹ️ Rutina de servicio de interrupción
En esta sección se habla acerca de la rutina de servicio de interrupción (ISR).

## 🧩 ISR cómo caso especial
Una ISR permite atender eventos especiales que ocurren en un periférico, los cuales detienen al procesador para ejecutar ciertas instrucciones con base en el evento que se ha generado. De este modo, una bandera de interrupción es la encargada de indicar que evento se ha generado. Una vez se ha entrado a la ISR, esta bandera debe ser limpiada para evitar que entremos a la ISR de forma indefinida (lo cual provocaría un error en la lógica del código).

De este modo, la ISR debe cumplir con ciertas condiciones (algunas de ellas ya mencionadas en otros capítulos):

* Limpiar la bandera de interrupción.
* Tener la menor cantidad posible de instrucciones.

Cuando se utiliza *bare-metal*, los efectos de tener una ISR con muchas instrucciones no se notan tanto; no obstante, cuando se hace uso de un RTOS o de varías ISR que deben atenderse, tener muchas instrucciones hace inviable el uso de estas últimas. 

Partiendo de esta idea, lo recomendable siempre es utilizar conceptos como **tareas diferidas**, **colas** u otra herramienta de software que permita acortar lo más posible el código de la ISR para que una tarea (en el caso de un RTOS) o el código del main se encarguen de realizar las acciones correspondientes ante la generación de la interrupción.

De este modo, retomando las condiciones:

* Limpiar la bandera de interrupción.
* Tener la menor cantidad posible de instrucciones.
* Hacer uso de tareas diferidas, colas u cualquier otra herramienta que permita indicarle al proceso principal o tarea que ya se debe ejecutar una cierta acción.

### 🧩 Ejemplo simple
Cómo ejemplo podemos tomar el siguiente código:

```c
static inline void GPIO_ClearFlagHardware();
static inline void GPIO_GiveFlagISR();
static void GPIO_TakeFlagISR();
static void GPIO_FlipLED();
static void GPIO_ClearFlagISR();

void ISR_GPIOA()
{
    GPIO_ClearFlagHardware(); //Limpia la bandera de interrupción, generada por hardware
    GPIO_GiveFlagISR(); //Función que indica al main que se puede ejecutar una instrucción relacionada a la bandera de GPIOA, en este caso es otra bandera.
}

int main()
{
    GPIO_Config();
    GPIO_ConfigInterrupt();

     while(1)
     {
        if( !GPIO_TakeFlagISR() ) //Validar si la bandera de monitoreo ha cambiado por la ISR
        {
            GPIO_FlipLED();     //parpadear LED
            GPIO_ClearFlagISR();    //Limpiar la bandera de monitoreo para una nueva interrupción
        }
     }
}
```

En el ejemplo se observa una ISR de GPIO que administra una interrupción por hardware; sin embargo, las acciones que se deben realizar a partir de dicha interrupción, se generan en el programa principal. Así conseguimos una ISR que ejecuta pocas instrucciones y que permite permite al programa principal tomar el control de nuevo en el menor tiempo posible. Asimismo, mientras se puede efectuar el parpadeo del LED, se puede generar otra interrupción y esta será atendida casi de inmediato, a pesar de que sea de menor prioridad.

### 🧩 Últimas consideraciones
Además de las consideraciones hechas anteriormente, es importante mencionar que la ISR es un caso especial de función en la que se pueden utilizar todas las capas de hardware. Sin embargo, se recomienda hacer uso de instrucciones sencillas; es decir, limpiar la bandera puede hacerse mediante el acceso al registro directamente, por dar un ejemplo, mientras que el uso de banderas de monitoreo puede hacerse empleando la capa de middleware. De este modo se podría reducir aún más la cantidad de instrucciones que el procesador tiene que efectuar en la ISR.
