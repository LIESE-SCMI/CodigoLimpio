# Bit-Field, uniones y estructuras: Explicación
## Recordatorio de estructuras

Una estructura es una zona de memoria continua que puede reservar variables de diferentes tipos de datos, como se muestra a continuación:

```c
struct EstructuraBasica
{
  uint8_t A;
  uint16_t B;
  uint32_t C;
  bool D;
  uint64_t E;
  char F[5];
  float G;
  struct Estructura_Interna;

};
```

La estructura nos indica que se deben reservar 11 localidades de memoria continúa con una palabra de 64 bits, como mínimo, para almacenar todas las variables definidas
(por ser un ejemplo, no se sabe la cantidad de localidades de memoria a reservar para la estructura).

## Explicación básica de bit-field

Ahora bien, un bit-field es, literalmente, un campo o un intervalo de bits, como su traducción del inglés lo indica.
Generalmente los nuevos campos de bits son reservados en una misma localidad de memoria, siempre y cuando se respete la longitud de bits definidos con los campos que se van declarando.
Esto quiere decir que para una variable "x", ahora se pueden fraccionar sus bits, a fin de optimizar el uso de memoria. 


Cómo muestra de estó, se tiene el siguiente ejemplo:

```c
/**
* Reutilizando la variable uint8_t A.
*/
struct BitField_A
{
  uint8_t A_UnBit : 1; //Se reserva un bit de los 8 que se declaran para A, en este caso el menos significativo (bit 0).
  uint8_t A_DosBit : 2; //Se reservan dos bits de los 7 que sobran en A, en este caso del bit 1 al 2 (bit 1:2).
  uint8_t A_CincoBit : 5; //Se reservan los bits sobran en A, en este caso del bit 3 al 7 (bit 3:7).

};
```
Con esto en mente, la estructura **BitField_A** simplemente contiene una variable "A" de tipo *uint8_t* (nos interesa resaltar que son ***8 bits*** los reservados en la estructura), es decir, en la nueva estructura reservamos una sola variable de 8 bits, para los cuales definimos una cantidad específica de bits con cada nueva variable, a través de **" : x_NúmerodeBits"**. 

### Puntos importantes a destacar:

- Los " : " sirven para indicar que se hará uso de una *n* cantidad de bits.  
- El uso de bit-field solo se implementa con ***estructuras***, debido a la reserva de memoria continua.
- El bit-Field permite ahorrar recursos, ya que hace más eficiente el uso de memoria.
- Siempre se debe respetar la relación entre el tipo de dato y la cantidad de bits que se ocupan.
- Por lo regular se ocupa con tipos de dato *int*, *uint*, *char*.

### Ejemplo de uso

Vamos a suponer que se quiere optimizar el uso de memoria en la siguiente estructura:

```c
struct Ejemplo_EstructuraBitField
{
  uint8_t A; //Una localidad reservada para ocupar 8 bits
  uint16_t B; //Una localidad reservada para ocupar 16 bits
  uint32_t C; //Una localidad reservada para ocupar 32 bits
  bool D; //Una localidad reservada para ocupar 1 bit

};
```
En total son 4 localidades reservadas; sin embargo, no todos los bits de todas las variables se utilizan, de modo que *B* solo ocupa 5 bits y *C* ocupa 16 bits.
En otras palabras, necesitamos 30 bits en total.
Bajo esta idea, podemos utiliziar bit-field para optimizar el uso de localidades, como se muestra a continuación.

```c
struct Ejemplo_EstructuraBitField
{
  uint32_t A : 8; //Se reserva del bit 0 al 7 (bit 0:7).
  uint32_t B : 5; //Se reserva del bit 8 al 12 (bit 8:12).
  uint32_t C : 16; //Se reserva del bit 13 al 29 (bit 13:29).
  uint32_t D : 1; //Solo se usa el bit 30 (bit 30).

};
```
Con la estructura Ejemplo logramos reducir de 4 a 1 las localidades de memoria utilizadas; además, se redujo el uso de bits que algunas variables no ocupaban.
De este modo, ahora podemos definir en EstructuraBasica:

```c
struct EstructuraBasica
{
  struct Ejemplo_EstructuraBitField Nueva_BitField; //Contiene las variables A - D
  uint64_t E;
  char F[5];
  float G;
  struct Estructura_Interna;

};
```

## Recordatorio de uniones
Una unión permite declarar dos o más tipos de dato bajo una misma localidad de memoria, en donde la variable con el tipo de dato más grande es la que da la pauta de la cantidad de memoria a reservar.

### Ejemplo de uso

Supongamos que queremos utilizar dos variables en una misma localidad: A (tipo uint8_t) y B (tipo uint32_t).
La unión nos permite realizar esto cómo se muestra a continuación:

```c
union Ejemplo
{
    uint32_t B; // 32 bits
    uint8_t A;  // 8 bits (ocupa el byte menos significativo de B)

};
```
Entonces, bajo está idea, podemos utilizarla a través de:

```c
//Código anterior
  union Ejemplo Union1; //Declaramos la unión
  
  Union1.B = 0; // Pone todos los bits a 0: B = 0x00000000
  Union1.A = 0xF1; // Modifica solo el byte menos significativo de B: B = 0x000000F1
  Union1.B = 0xAF8; // Sobrescribe completamente los 32 bits: B = 0x00000AF8
//Código posterior
```

Al final B fue sobreescrito con el último valor.

## Uso de Uniones, estructuras y bit-field

Supongamos que queremos acceder a un registro de 32 bits, a través de estructuras con bit-field y uniones.
Para lograrlo, podemos hacer lo siguiente:

```c
/*
* Definir primero el bit-field del registro:
*/
typedef struct
{
  volatile const uint32_t bits_Lectura : 3; // Bits indicados para solo lectura
  volatile uint32_t bit_Config0 : 1;        // Bits indicados para escritura y lectura
  volatile uint32_t bit_Config1 : 1;         // Bits indicados para escritura y lectura
  volatile uint32_t bit_Config2 : 1;        // Bits indicados para escritura y lectura
  volatile uint32_t bit_Config3 : 1;        // Bits indicados para escritura y lectura
  volatile uint32_t bit_Config4 : 1;        // Bits indicados para escritura y lectura
  volatile uint32_t bits_Led : 2            // Bits de entrada y salida para encender LED
  const uint32_t bits_Reservados : 22;      // Bits indicados para no utilizarse
}Reg_BitField;
```

Con esto en mente ya tenemos una estructura con bit-field para acceso a un registro, en cuyo caso, *bits_Lectura* define los primeros 3 bits como solo de lectura (**volatile** indica cambio de la información de esos 3 bits por hardware, mientras que **const** indica que no se puede modificar la variable de manera manual por el usuario). Por otro lado, los bits de lectura y escritura (en este caso, solo se usa **volatile** para indicar cambios por hardware, por lo cual se debe monitorear esos bits constantemente) son los que van después de *bit_Lectura*. Por último, los bits reservados se definen a través de *bits_Reservados* (en este caso, **const** se encarga de marcar esos bits como constantes, para evitar acceso a ellos por el usuario. Esto marcará un error al compilar).

```c
/*
* Definir la unión para acceder por bit-field o por acceso completo al registro
*/
typedef union
{
  Reg_BitField AccesoBitField;      // Acceso por bit-field al registro
  volatile uint32_t AccesoCompleto; // Registro indicado para escritura y lectura
}Reg_Union;
```
Con el uso de la unión, indicamos dos formas de acceder a la misma localidad de memoria (un registro cualquiera, en este caso): una por acceso a campos de bit y la otra por acceso completo al registro, como usualemente se trabaja. Para definir el acceso al registro, se puede declarar de la siguiente manera:

```c
int main()
{
  static Reg_Union * const GPIO_DATA = (Reg_Union *)(0xA000B104); //Declaración de registro imaginario

  GPIO_DATA->AccesoCompleto = (0xF << 3); //Escritura sobre bits de Configuración por acceso con bitwise

  while (1)
  {
    if ( GPIO_DATA->AccesoBitField.bits_Lectura == 7 ) //podemos leer los 3 bits
    {
      GPIO_DATA->AccesoBitField.bits_Led = 3; //Solo podemos configurar 2 bits
    }else if ( GPIO_DATA->AccesoBitField.bits_Lectura == 5 ) //podemos leer los 3 bits
    {
      GPIO_DATA->AccesoBitField.bits_Led = 2; //Solo podemos configurar 2 bits
    }else if ( GPIO_DATA->AccesoBitField.bits_Lectura == 3 ) //podemos leer los 3 bits
    {
      GPIO_DATA->AccesoBitField.bits_Led = 1; //Solo podemos configurar 2 bits
    }else if ( GPIO_DATA->AccesoBitField.bits_Lectura == 1 ) //podemos leer los 3 bits
    {
      GPIO_DATA->AccesoBitField.bits_Led = 0; //Solo podemos configurar 2 bits
    }
  }
}
```
En el anterior ejemplo, primero declaramos nuestro registro:

```c
  static Reg_Union * const GPIO_DATA = (Reg_Union *)(0xA000B104);
```
el cual es un puntero constante por *const* (no permite uso de arimética de punteros, ya que siempre va a apuntar a una localidad de memoria), de tipo *Reg_Union* lo cual nos permite acceder a la misma localidad de memoria por bit-field y por acceso completo al registro (la unión se encarga de eso, así como la estructura con bit-filed); además, *GPIO_DATA* posee como otra propiedad a *static*, de modo que solamente se puede utilizar dentro de la función donde fue declarado el registro (en este caso dentro de *main*), evitando así que cualquier usuario llame al registro desde otros archivos y forzando al programador a pasar como parámetro ese registro, en lugar de usarlo de manera global.

Gracias a la unión, ahora tenemos dos modos de acceder a nuestro registro: de la manera completa (manera tradicional de hacerlo)

```c
  GPIO_DATA->AccesoCompleto = (0xF << 3); 
```

 y por campos de bits.

 ```c
  if ( GPIO_DATA->AccesoBitField.bits_Lectura == 7 ) 
  {
    GPIO_DATA->AccesoBitField.bits_Led = 3; 
  }else if ( GPIO_DATA->AccesoBitField.bits_Lectura == 5 )
  {
    GPIO_DATA->AccesoBitField.bits_Led = 2;
  }else if ( GPIO_DATA->AccesoBitField.bits_Lectura == 3 )
  {
    GPIO_DATA->AccesoBitField.bits_Led = 1;
  }else if ( GPIO_DATA->AccesoBitField.bits_Lectura == 1 )
  {
    GPIO_DATA->AccesoBitField.bits_Led = 0;
  }
```

### Advertencias
* ***ADVERTENCIA 1:*** El acceso por bit-field puede ser más lento y generar más instrucciones de las necesarias, a diferencia de realizar la escritura mediante operaciones *bitwise* (máscaras y desplazamientos). Esto se debe a que el compilador suele generar una secuencia de lectura, modificación y escritura del dato completo (Read-Modify-Write), aun cuando solo se desea modificar uno o pocos bits.

Recomendación: Para mejorar el desempeño y el control sobre el código generado, se recomienda el uso de operaciones bitwise explícitas, especialmente en registros de hardware o en secciones críticas del sistema.

* ***ADVERTENCIA 2:*** El uso de estructuras con bit-field depende del compilador, de la arquitectura del procesador y de la forma en que el compilador decide organizar los bits dentro de la estructura. El *endianess* define el orden en el que se almacenan los bytes en memoria para datos de más de un byte, pero no garantiza el orden de los bits dentro de un bit-field. Por esta razón, no siempre se puede asumir que la primera variable declarada dentro de una estructura con bit-field comenzará en el bit menos significativo.

Recomendación: Se recomienda evitar el uso de bit-fields para mapear registros de hardware o estructuras que dependan de una disposición exacta de los bits, ya que su comportamiento puede variar entre compiladores y plataformas.

* ***ADVERTENCIA 3:*** Se debe tener en consideración el ***acceso atómico*** a bits. Para ejemplificar este caso, se debe retomar la idea de una linea de código en C: `GPIO_DATA->AccesoBitField.bits_Led = 2`;

A nivel de instrucciones del procesador, esta operación suele implicar:
* (I) acceder a la dirección del registro GPIO_DATA,
* (II) leer la palabra completa de 32 bits,
* (III) modificar los bits aplicando una máscara en el campo correspondiente,
* (IV) escribir la palabra completa modificada en el registro.

El acceso atómico implica que toda esta secuencia de instrucciones no debe ser interrumpida. El problema surge cuando existen interrupciones o se utiliza un Sistema Operativo en Tiempo Real (RTOS), ya que estos pueden interrumpir el flujo normal del programa. Si la ejecución se interrumpe entre los pasos (II) y (IV) y otra ISR o tarea modifica el mismo registro, al retomarse la ejecución se realizará la escritura del registro con información desactualizada, generando un error difícil de detectar y depurar.
