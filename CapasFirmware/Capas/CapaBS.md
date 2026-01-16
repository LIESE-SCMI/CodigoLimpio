# Capa de "board support"
En esta sección se muestra el planteamiento de una función para la capa de Board Support.

## 💾 Creación de una función para board support
Retomando la definición, la capa de board support contiene todo el firmware relacionado con la configuración y comunicación entre el uC, los sensores y actuadores. 

Se debe mencionar que esta capa implementa el uso del *diseño por contrato*, visto en la capa HAL. Asimismo, retoma algunas ideas propuestas en esa misma capa.

### 📄 Comentario
El comentario no cambia mucho:

```c
/**
# > Datos generales:

# >> Nombre de la función:

# >> Archivo donde se encuentra:

# >> Creador:

# >> Componente externo:

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

Ahora se agrega un nuevo elemento **Componente externo**, el cual va a indicar el nombre del sensor o actuador utilizado para las funciones descritas. Con esto se tiene un contexto completo de la función, bajo el cual un usuario puede utilizar una función y sabe de las consecuencias que puede tener si coloca información incorrecta o le da un uso incorrecto.

### 🧩 Planteamiento de una función en la capa de board support 
En este apartado se muestran los requerimientos para implementar funciones en esta capa.

### 🧩 Nombre
En esta capa, todas las funciones deben seguir la siguiente síntaxis: *NombreGeneralComponenteExterno_PerifericoPrincipal_Acción*. Por ejemplo:

```c
/**
Comentario
*/
SerialFlash_SPI_Write(param 1)
{

}
```

### 🧩 Parámetros
Del mismo modo, los parámetros se van a utilizar para indicar que sí y que no se puede utilizar en la función.

```c
/**
Comentario
*/
SerialFlash_SPI_Write(ptrFixedData Data)
{
    SPI5_DMA1_SendData(SPI5_Reg, DMA1_Reg, Data);
}
```

⚠️ ***¡OJO!*** En esta capa ya no es necesario pasar como parámetros las estructuras de los periféricos empleados; sin embargo, **sí es indispensable** mencionar cuales se están ocupando y bajo que circunstancias.

### 🧩 Ejemplo de estructura para parámetros de transmisión y/o recepción
Para transmisión y recepción de datos se muestra un ejemplo en el que se implementan estructuras:

```c
typedef char * ptrSerialFlashData;
typedef char SerialFlashArrayData_Byte;

typedef struct
{
    ptrSerialFlashData ptrReceiveData;
    ptrSerialFlashData ptrTransmitData;
    SerialFlashArrayData_Byte ReceiveData[100];
    SerialFlashArrayData_Byte TransmitData[17];
}SerialFlash_Data;

SerialFlash_Data Flash_Data = {ReceiveData, TransmitData, {0}, {0}};

/**
Comentario
*/
SerialFlash_SPI_Write(SerialFlash_Data * Data)
{
    SPI5_DMA1_SendData(SPI5_Reg, DMA1_Reg, &Data->ptrTransmitData);
    Data->ptrTransmitData = &Data->TransmitData[0];
}
```

⚠️ ***¡OJO!*** Esto también puede aplicar para la capa de HAL, sin embargo, dependerá de la arquitectura final que se desee implementar, en específico de las capas seleccionadas.

## 🧩 Resumen
* Retomar el diseño por contrato para funciones.
* Funciones con síntaxis *Nombre_Acción*.
* Comentario descriptivo para cada función, con el agregado *Componente externo*.
* Estructuras como parámetros de transmisión y recepción.
