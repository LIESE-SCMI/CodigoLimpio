# 🧱 Capas de software embebido: Middleware y API
En esta capa se presentan algunas de las técnicas que se pueden implementar en los proyectos.

## 🧩 Consideraciones y recomendaciones para Middleware
Cómo esta capa usualmente es proporcionada por terceros, aquí pueden variar las implementaciones. De este modo, no se pueden dar una serie de seguimientos, es decir, dependerá más del programador y de las necesidades que se tienen. No obstante se pueden dar algunos ejemplos sobre una de las tantas formas en las que puede implementarse un RTOS, en específico, el Free-RTOS.

* ***Primera recomendación:*** utilizar tareas diferidas en rutinas de servicio de interrupción (ISR), debido a que una ISR con muchas instrucciones puede ocasionar errores, en específico cuando el RTOS realiza el cambio de contexto a otra tarea.
* ***Segunda recomendación:*** en recepción de datos por ISR, utilizar colas para pasar esos datos a otras tareas y liberar la ISR.
* ***Tercera recomendación:*** realizar una buena gestión de la memoria, debido a que un mal gestión puede ocasionar desbordamientos y errores.

## 🧩 Consideraciones y recomendaciones para funciones API
En el caso de una función API, depende más de las necesidades de cada aplicación y del programdor, por lo cual no se puede definir un estándar a seguir del todo. Sin embargo si se pueden definir ciertas condiciones generales:

* En esta capa solo se deberían observar funciones de las capas inferiores:middleware, board support y algunas HAL, si llega a ser el caso; la capa de drivers *NO* debe verse.

* El comentario general que se ha manejado hasta ahora debe mantenerse, debido a que esas funciones van a seguir el *diseño por contrato*, ya visto anteriormente a profundidad.

* La sintaxis para el nombre de las funciones puede mantener el patrón propuesto anteriormente en las capas anteriores: *NombreMódulo_Acción*; debido a que en este nivel de abstracción ya se tienen módulos definidos a través de las etapas de diseño.