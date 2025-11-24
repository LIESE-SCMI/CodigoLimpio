# 🧱 Etapas de diseño y prototipado
En esta sección se presentan las etapas de diseño y de prototipado para implementar en un proyecto.

## 📊 Etapas de diseño
Es evidente que en esta sección se debe hablar de metodologías. En un caso cualquiera, lo ideal es seguir una metodología estilo Ulrich que contempla los siguientes puntos:

* ***Obtención de requisitos:*** cómo su nombre lo indica, aquí se obtienen los requerimientos iniciales del proyecto, contestando preguntas básicas: ¿qué debe hacer? y ¿cómo debe actuar el sistema?

* ***Análisis del sistema:*** aquí se obtienen los módulos básicos del sistema (a partir de los requisitos iniciales obtenidos en el punto anterior), a fin de agrupar todas las funciones y mostrar las interacciones entre cada bloque propuesto. También se hace mención del hardware externo que se va a necesitar.

* ***Diseño de arquitectura:*** a través de diagramas, se define una arquitectura más sólida (desarrollando los módulos obtenidos), de modo que se obtengan las especificaciones técnicas a desarrollar (periféricos asociados, frames, tasas de transmisión, reloj del sistema, así como la selección de integrados, transductores y microcontrolador). Una vez se tenga definida la arquitectura completa, también se selecciona el lenguaje de programación, así como la versión del estándar, las capas de firmware, selección de RTOS y modo de tiempo real, así como las técnicas de código limpio a emplear.

* ***Implementación:*** a partir del punto anterior, se desarrolla todo el firmware hasta obtener un prototipo funcional que pueda validar el diseño de arquitectura. Usualmente se puede hacer uso de funciones HAL ya desarrolladas por el fabricante, para acelerar el proceso de validación.

* ***Refinamiento y pruebas:*** en esta etapa se busca depurar el sistema a base de diferentes pruebas. Esto se logra a partir de un proceso iterativo conformado por diferentes etapas, el cual se muestra a continuación:
   * ***Etapa de implementación limpia (opcional):*** si se considera necesario, se pueden definir las capas de drivers y funciones HAL, a favor de obtener una versión más optimizada, orientada a las necesidades específicas del producto.
   * ***Etapa de validación y respuesta a errores de hardware externo:*** consiste en utilizar hardware y software extra, para resolver posibles errores de comunicación entre periféricos, transductores, o memorias u otros integrados externos al microcontrolador. El objetivo es crear una comunicación robusta que responda a la presencia de errores.
   * ***Etapa de pruebas de larga duración:*** se obtienen y corrigen problemas que se presentan cuando el sistema funciona durante un tiempo prolongado.
   * ***Etapa de inyección de fallas (opcional):*** orientada a sistemas que serán expuestos al medio ambiente de radiación espacial, consiste en emular los efectos de la radiación (a través de técnicas de inyección de fallas) con el fin de reproducir el medio ambiente bajo el cual se encontrará expuesto el sistema. El objetivo es validar las técnicas de tolerancia a fallas y posibles nodos sensibles que deban mejorarse.

* ***Documentación:*** a pesar de que la documentación puede realizarse desde la etapa anterior, aquí se reportan los resultados de todo el proceso de diseño, desde funcionalidad hasta problemas y errores que el producto puede presentar, así como la documentación necesaria para su uso, mantenimiento y futuras mejoras.

Cabe mencionar que las etapas anteriores se pueden mejorar, reducir y/u optimizar, a fin de obtener una implementación adaptada a las necesidades del proyecto.
