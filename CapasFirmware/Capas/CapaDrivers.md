# 🧱 Capa de drivers
El acceso a los registros es clave en cada proyecto. Para ello, a continuación se presenta la base para describir el acceso a registros.

## 💾 Mapeo de registros

### ℹ️ Definiciones de tipo de dato a utilizar para definir registros
Son las siguientes:

```c
typedef volatile uint32_t   _IO_t;
```
*_IO_t*: entrada/lectura y salida/escritura de información en el registro

```c
typedef volatile uint32_t   _O_t;
```
*_O_t*: salida/escritura de información en el registro

```c
typedef volatile const uint32_t   _I_t;
```
*_I_t*: entrada/lectura de información en el registro

### 💻 Mapeo de registros
El formato a escoger para declarar las estructuras es el siguiente:

```c
typedef struct 
{
    _IO_t AbrevNomReg1;
    _I_t  AbrevNomReg2;
    _O_t  AbrevNomReg3;
}NomPeriferico_Registros_t;
```

### 🏗️ Ejemplo
En un ejemplo real, podría verse como:

```c
typedef volatile uint32_t       _IO_t;
typedef volatile uint32_t       _O_t;
typedef volatile const uint32_t _I_t;

typedef struct
{
    _O_t CCR;   /**< Clock Config register */
    _IO_t CR1;  /**< Config register 1 */
    _IO_t CR2;  /**< Configure register 2 */
    _I_t RR;    /**< Receive data register */
    _O_t  TR;   /**< Transmit data register */
}USART_Register_t;

typedef USART_Register_t *const USART_RegFixedPtr_t;

USART_RegFixedPtr_t USART_Reg = (USART_Register_t *)(0x20010A00);
```

## 🗂️ Acceso a registros

### ℹ️ Definiciones de tipo de dato a utilizar para acceso a registros
Son las siguientes:

```c
typedef const uint32_t   _FixedMask_t;
```
*_FixMask_t*: tipo de dato constante, sin signo, de 32 bits fijos (no es obligatorio usar const, pero asegura que el parámetro no cambia dentro de la función, por escritura del usuario).

```c
typedef const uint32_t   _FixedValue_t;
```
*_FixValue_t*: tipo de dato constante, sin signo, de 32 bits fijos (no es obligatorio usar const, pero asegura que el parámetro no cambia dentro de la función, por escritura del usuario).

### 💻 Funciones para acceder a registros
Para escritura:
```c
void Reg32_Set(_IO_t *Reg, _FixedValue_t value)
{
    *Reg = value;
}
```

```c
void Reg32_SetOR(_IO_t *Reg, _FixedValue_t value)
{
    *Reg |= value;
}
```

```c
void Reg32_SetBit(_IO_t *Reg, _FixedMask_t shift, _FixedValue_t value)
{
    *Reg |= (value << shift);
}
```

```c
void Reg32_Clear(_IO_t *Reg, _FixedValue_t value)
{
    *Reg &= ~value;
}
```

```c
void Reg32_ClearBit(_IO_t *Reg, _FixedMask_t shift, _FixedValue_t value)
{
    *Reg &= ~(value << shift);
}
```

En el caso de lectura:

```c
uint32_t Reg32_Read(_IO_t *Reg)
{
    return *Reg;
}
```

```c
uint32_t Reg32_ReadBit(_IO_t *Reg, _FixedMask_t mask)
{
    return (*Reg & mask);
}
```

Para casos especiales:
```c
void Reg32_SetXOR(_IO_t *Reg, _FixedValue_t value)
{
    *Reg ^= value;
}
```

```c
void Reg32_SetBitXOR(_IO_t *Reg, _FixedMask_t shift, _FixedValue_t value)
{
    *Reg ^= (value << shift);
}
```

Con estas funciones se asegura una buena escritura y lectura de los registros, de modo que un programador ya no tiene que escribir la instrucción "n" veces (lo cual puede provocar un tipo de error); además, el uso de estas funciones evita posibles errores en la aritmética de punteros, a pesar de que la declaración de la estructura para acceder a los registros ya lo hace con el puntero constante (*const*). Sin embargo, este método emplea más instrucciones de ensamblador (alrededor de dos más por escritura en un registro), en comparación con las formas utilizadas por CMSIS (por dar un ejemplo), por lo cual será una decisión a tomar en cuenta a la hora de hacer o no uso de ellas.

### 🏗️ Ejemplo
Retomando el ejemplo del capítulo anterior:

```c
USART_RegFixedPtr_t USART_Reg = (USART_Register_t *)(0x20010A00);

typedef enum
{
    MASK_0  = 0,
    MASK_1  = 1,
    MASK_2  = 2,
    MASK_3  = 3,
    MASK_4  = 4
}Reg32_MaskBits_t;

typedef enum
{
    CLOCK_USART1  = 0,
    CLOCK_USART2  = 1
}USART_Clock_t;

typedef enum
{
    ASYNC_CONFIG  = 0x20AC
}USART_Clock_t;

void USART_Config(USART_RegFixedPtr_t USART_Reg)
{
    Reg32_SetBit(USART_Reg->CCR, MASK_0, CLOCK_USART1);
    Reg32_SetOR(USART_Reg->CR1, ASYNC_CONFIG);
}
```
