# 🧱 Etapas de diseño y prototipado
En esta sección se presentan las etapas de diseño y de prototipado para implementar en un proyecto.

## 📊 Etapas de diseño
Es evidente que en esta sección se debe hablar de metodologías. En un caso cualquiera, lo ideal es seguir una metodología estilo Ulrich que contempla los siguientes puntos:

* ***Obtención de requisitos:*** se deben obtener necesidades funcionales y no funcionales del sistema; asimismo, se contestan preguntas como por ejemplo: ¿Qué debe hacer el sistema?, ¿Qué restricciones tiene?, ¿Qué interfaces (sensores, actuadores) utilizará?

* ***Análisis del sistema:*** con base en los requisitos, se obtienen módulos y subsistemas. En esta etapa se busca modelar el comportamiento del sistema, identificar componentes de hardware y softwar y evaluar costos y tecnologías disponibles.

* ***Diseño de arquitectura:*** se define la estructura del sistema, en donde se selecciona el dispositivo electrónico (microcontrolador o FPGA), se obtienen las tareas para el equipo de *hardware* y de *software*; y se diseñan las interfaces entre módulos.

* ***Diseño detallado:*** aquí se aborda a profundidad la implementación de cada componente; es decir:

    * **Diseño de hardware:** esquemáticos, tarjetas PCB, etc.
    * **Diseño de software:** diagramas de flujo, máquinas de estado, arquitectura de firmware, selección de protocolos de comunicación, temporizadores, interrupciones a utilizar, etc.

* ***Implementación:*** aquí se aborda todo el desarrollo de hardware y software, de modo que se implementa la programación y desarrollo de prototipados físicos que contengan sensores y actuadores.

* ***Verificación y validación:*** con base en los requisitos, en esta etapa se implementan: pruebas unitarias, de integración y funcionales; se verifica el consumo energético, la respuesta en tiempo real (si la requiere), la robustez, entre otras; así como la validación por parte del cliente.

* ***Optimización:*** como su nombre lo indica, en esta etapa se mejora el rendimiento del sistema, a través de la reducción del consumo de energía, reducción del tiempo de respuesta, comprensión del código y optimización de espacio.

Cabe mencionar que las etapas anteriores se pueden mejorar, reducir y/u optimizar, a fin de obtener una implementación adaptada a las necesidades del proyecto.

## 📊 Etapas de prototipado
La etapa de *implementación*, de *verificación y validación* y de *optimización* vistas anteriormente, se pueden desarrollar aún más en las siguientes etapas, en el caso de software embebido.

* ***Etapa de prototipado rápido:*** primera implementación, que se centra en implementar el sistema bajo el uso de software comercial, con componentes y herramientas básicos; a fin de obtener una primera implementación funcional y viable que permita continuar su desarrollo. En esta sección no se busca optimización, verificación, correción de errores ni rendimiento, sino su validación como posible desarrollo. De este modo, se pueden utilizar funciones HAL y drivers de terceros, así como algún RTOS de próposito general de software libre (si el proyecto lo requiere). 

* ***Etapa de implementación limpia:*** busca la creación y/o limpieza de librerías propias, manteniendo la funcionalidad de la etapa anterior.

* ***Etapa de validación y respuesta a errores de hardware I:*** en esta etapa se busca resolver posibles errores en los periféricos y el hardware que se esta utilizando en el sistema embebido, a través de banderas de interrupción y otras técnicas de validación.

* ***Etapa de validación y respuesta a errores de hardware II:*** aquí se utilizan ténicas para validar y mejorar la transmisión y recepción de información entre los periféricos y el hardware. 

* ***Etapa de pruebas de larga duración:*** a través de pruebas de larga duración en un entorno controlado, aquí se ponen a prueba y se depuran los bugs y errores que el software pueda llegar a producir, como parte de la lógica implementada.

* ***Etapa de inyección de fallas I (si es necesaria):*** se implementa un método de inyección de fallas, para validar las técnicas de mitigación de fallas que se propusieron para el sistema. Es una prueba rápida que busca sacar a la luz la mayor cantidad de errores, a través de la inyección de fallas continúa.

* ***Etapa de inyección de fallas II (si es necesaria):*** se implementa una segunda etapa de pruebas de larga duración, inyectando fallas al sistema bajo un entorno lo más parecido a las condiciones reales bajo las que va a operar el sistema.

Cabe mencionar que este proceso puede realizarse de manera iterativa, incluir o no todos los puntos mencionados, así como agregar más etapas. Dependerá de las necesidades del proyecto.
