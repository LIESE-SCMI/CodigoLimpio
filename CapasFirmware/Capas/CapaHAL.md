# 🧱 Capa HAL
Es más fácil utilizar funciones abstractas que escribir acceso a registros.

## 💾 Diseño por contrato
Para este caso específico, crear una o varías HAL va a depender de las necesidades generales del proyecto. A continuación se muestra un ejemplo de cómo plantear una función bajo la metodología *diseño por contrato*, la cual se explica a continuación.

🌐 [Diseño por contrato](https://www.revista.unam.mx/vol.4/num5/art11/art11.htm)

### 🧩 Objetivo
El **diseño por contrato** se centra en evitar la validación de los parámetros que recibe una función durante la ejecución del programa, debido a que consume tiempo valioso que la CPU puede ocupar para aquellas instrucciones que si son prioritarias. De este modo, el diseño por contrato hará uso de documentación o comentarios, para exponer al usuario todo el contexto relevante de la función, como sus parámetros de entrada, el valor de retorno, las pre-condiciones que se deben cumplir para que la función realice un correcto trabajo y las post-condiciones que el usuario debe esperar (después de la ejecución de la función), además de la inclusión de ejemplos de uso e invariantes (condiciones que se deben cumplir dentro de la función con el fin de esperar una correcta operación).

### ⚠️ Puntos importantes
Retomando la información anteriormente mencionada, el método se va a ajustar de la siguiente manera:

* **Tipos de rutina.** Define que tipo de función va a utilizar un usuario (el mismo debe entenderlo o saber acerca de la metodología a usar, sino no representa nada para él).
    * **Comandos.** Cambian y/o modifican el estado interno de un objeto(en nuestro caso, algún periférico, componente electrónico, etc). 
    * **Consultas.** Devuelven o retornan un valor (en nuestro caso, consultar el valor interno de un registro).
* **Pre-condiciones.** Condiciones que se deben cumplir **ANTES** de llamar a la función (Ejemplo: función de configuración de un GPIO, solo si ya se utilizó la función que da reloj al mismo; archivos que se deben agregar para utilizar la función).
* **Post-condiciones.** Condiciones que se van a cumplir **DÉSPUES** de ejecutar la función (definir archivos que se pueden usar ahora, obtener una configuración específica para mi periférico, con ciertas carácteristicas; tipo de retorno de información...).
* **Invariantes.** Condiciones internas de la función, para que esta pueda operar correctamente (Si dentro de los parámetros, uno es de tipo entero y sin signo, se debe especificar que ese parámetro no acepta datos menores a cero).

## 💾 Creación de una función HAL
Retomando la definición, una función *HAL* abstrae en una o varias funciones el acceso y escritura a registros, a fin de facilitar la configuración y uso del periférico/puerto a trabajar. Bajo esta idea y empleando el *diseño por contrato*, a continuación se dará un ejemplo de creación de función HAL para configurar un periférico cualquiera.

### 📄 Comentario
Un solo comentario es suficiente. Debe ser concetro, muy específico y capas de transmitir toda la información necesaria para que el usuario pueda emplearla.

Con esto en mente. el primer paso es definir todas aquellas condiones (el contexto en sí), bajo las cuales se va a describir la función. Cómo es importante dejarlo de manera explícita, a fin de evitar errores por parte del usuario que utilice dichas funciones, se debe hacer uso de los comentarios con el siguiente formato.

```c
/**
# > Aserciones de la función:

# >> Tipo de rutina:

# >> Pre-condiciones:

# >> Post-condiciones:

# >> Invariantes:
**/
```
Por otra parte, el usuario debe tener una explicación general de la función, así como algún ejemplo de uso. De este modo, el comentario anterior debe contener otros puntos extras:

```c
/**
# > Datos generales:

# >> Nombre de la función:

# >> Archivo donde se encuentra:

# >> Creador:

# >> Decripción:

# >> Versión:

# >> Estado (funcional o no actualmente en la versión):

# > Aserciones de la función:

# >> Tipo de rutina:

# >> Pre-condiciones:

# >> Post-condiciones:

# >> Invariantes:

# > Parámetros que recibe:

# > Parámetros que retorna:

# > Funciones contiguas:

# > Ejemplo de uso:
**/
```

Con esto se tiene un contexto completo de la función, bajo el cual un usuario puede utilizar una función y sabe de las consecuencias que puede tener si coloca información incorrecta o le da un uso incorrecto.

Por último, es importante resaltar que este comentario aplica para todas las capas de firmware y software a utilizar en sistemas embebidos.

### 🧩 Planteamiento de una función HAL
En este caso, depende de la aplicación que se le quiere dar.

* ¿Quieres qué la función configure varios perifericos o solo uno?
* ¿Quieres abstraer todo el acceso a los registros o quieres que pueda acceder a ellos?
* ¿Con una sola función se configuran todos los modos de operación o se hace una función para cada modo?

En este caso, al ser una misión de alto desempeño, se hará lo siguiente:

* Funciones que configuren un solo periférico bajo las condiciones requeridas para la misión.
* Pocos parámetros.
* Acceso atómico a registros.

#### 🧩 Nombre
Para la capa de HAL, todas las funciones deben seguir la siguiente síntaxis: *NombrePeriférico_Acción*. Por ejemplo:

```c
/**
Comentario
*/
USART_Config(param 1, param 2)
{

}
```

Si se ocupan dos periféricos o más se puede utilizar la siguiente síntaxis: *NombrePeriféricoPrincipal_NombrePeriféricoExtra_Acción*. De este modo, se presenta a continuación el siguiente ejemplo:

```c
/**
Comentario
*/
USART_DMA_Transmit(param 1, param 2, param 3)
{

}
```

### 🧩 Parámetros
Los parámetros se van a utilizar para indicar que sí y que no se puede utilizar en la función. Parece obvio, pero no lo es. Por ejemplo en la siguiente función:

```c
/**
Comentario
*/
USART_Config(USART_Reg *USART1_Reg, USART_RegConfig USART1_Config)
{
    // Dentro de esta función solo se pueden utilizar las dos estructuras que se han pasado como parámetros

    Reg32_Set(&USART1_Reg->CCR, USART1_Config.Clock); //Esta bien
    Reg32_SetOR(U&SART1_Reg->CR1, USART1_Config.BaudRate); //Esta bien

    //Estó esta mal. A pesar de que una estructura global se puede llamar sin pasarse como parámetro, estó puede confundir a alguien poco experto e incluso corromper el código cuando se habla de un RTOS o una ISR que comparten ese registro o variable.
    Reg32_Set(&GPIO_Reg->IR, 0x01); 

}
```

Por ello, nosotros solo podremos pasar a lo mucho cuatro parámetros:

* La estructura del mapeo de registros del periférico a configurar.
* La estructura de configuración.
* La estructura del GPIO para configurar pines con función alterna.
* Algún parámetro cómo un puntero a un arreglo de tipo char (en periféricos de comunicación serial).

⚠️ ***¡OJO!*** Ya no es necesario escribir comentarios línea por línea, el *comentario* se encarga de eso.

### 🧩 Estructura de configuración y parámetros de configuración
La estructura de configuración se encarga de contener toda la información para configurar un periférico y/o GPIO. para optimizar el uso de memoria, se emplean estructuras con bit-field. A continuación Se muestra un ejemplo para describir el comportamiento del periférico USART:

```c
typedef struct
{
    uint32_t Clock : 1; /**< Reloj para USART1. Enum: USART_Enum_Clock */
    uint32_t BaudRate : 31; /**< Bauds para USART1. Enum: USART_Enum_BaudRate */
}USART_RegConfig;
```

Está es la definición de la estructura (es el esqueleto), sin embargo, para inicializarse se vería algo cómo:

```c
USART_RegConfig USART1_Config 
{
    USART1_Clk,
    USART1_9600bps
};
```

En donde cada parámetro de configuración se puede emplear una enumeración. Dichos parámetros son los **ÚNICOS** que se van a escribir en las estructuras de configuración. Por ejemplo:

```c
typedef enum
{
    USART1_Clk = 0x08;
}USART_Enum_Clock;

typedef enum
{
    USART1_9600bps = 9600;
}USART_Enum_BaudRate;
```

⚠️ ***¡OJO!*** Las enumeraciones ayudan a eliminar los números mágicos(valores explícitos en hexa, bin, octal o decimal).

### 🏗️ Ejemplo completo
A continuación se muestra un ejemplo, reutilizando todo lo visto en esta capa.

```c
/*********************************************************************
                    Sección de configuración
*********************************************************************/
typedef enum
{
    USART1_Clk = 0x0800;
}USART_Enum_Clock;

typedef enum
{
    USART1_9600bps = 9600;
}USART_Enum_BaudRate;

typedef enum
{
    USART1_AsynConfig = 0xAF;
}USART_Enum_Config;

typedef struct
{
    uint32_t Clock : 1; /**< Reloj para USART1. Enum: USART_Enum_Clock */
    uint32_t Config : 8; /**< Configuración para USART1. Enum: USART_Enum_Config*/
    uint32_t BaudRate : 23; /**< Bauds para USART1. Enum: USART_Enum_BaudRate */

}USART_RegConfig;

USART_RegConfig USART1_Config = { USART1_Clk, USART1_AsynConfig, USART1_9600bps};

/*********************************************************************
                            Funciones
*********************************************************************/

/*********************************************************************
* # > Datos generales
* 
* # >> Nombre de la función: USART_Config
* 
* # >> Archivo donde se encuentra: USART.h y USART.c
* 
* # >> Creador: Ares Isai J.G.
*
* # >> Decripción: Esta función se encarga de configurar el periférico 
* USART1, a 9600 bps, con un bit de stop, 9 bits de dato, un bit de 
* paridad par, transmisión y recepción de datos con interrupciones.  
*
* # >> Versión: 1.0.0
*
* # >> Estado: Funcional
* 
* # > Aserciones de la función
*
* # >> Tipo de rutina: Comando
*
* # >> Pre-condiciones:
* PRE-CONDITION: Configurar el reloj del periférico con USART1_Clock()
* 
* PRE-CONDITION: Inicializar la estructura de configuración USART1_Config 
* con los parámetros que puede aceptar
*
* # >> Post-condiciones:
* POST-CONDITION: USART1 se configura a 9600 bps, con un bit de stop, 9 
* bits de dato, un bit de paridad par, transmisión y recepción de datos 
* con interrupciones.
* 
* POST-CONDITION: Ya se puede utilizar su ISR.
*
* # >> Invariantes:
* 1. La estructura de configuración debe tener las variables definidas de 
* las enumeraciones definidas en el archivo: USART.h. 
* 
* 2. 
* 
* # > Parámetros que recibe (en órden)
* @param[in]: USART_Reg *
* @param[in]: USART_RegConfig
*
* # > Parámetros que retorna
* @return NULL 
*
* # > Funciones contiguas
* @see USART1_Clock
* @see USART1_ISR
* @see USART1_DMA
* @see USART1_ReceiveData
* @see USART1_TransmitData
*
* # > Ejemplo de uso:
* @code
* USART1_Clock();
* USART_Config(USART1_Reg, USART1_Config);
* USART1_TransmitData(ptrData);
* @endcode
*********************************************************************/
USART_Config(USART_Reg *USART1_Reg, USART_RegConfig USART1_Config)
{
    Reg32_SetOR(&USART1_Reg->CCR, USART1_Config.Clock);
    Reg32_SetBitOR(&USART1_Reg->CR1, 16, USART1_Config.BaudRate);
    Reg32_SetOR(&USART1_Reg->CR1, USART1_Config.AsyncConfig);
}
```

## 🧩 Resumen
* Diseño por contrato para funciones.
* Funciones con síntaxis *Nombre_Acción*.
* Comentario descriptivo para cada función.
* Estructuras como parámetros para funciones.
* Estructura para configurar con uso de bit-field.
* Enumeraciones para eliminar números mágicos.
