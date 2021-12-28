1. Introducción 


El termino Industria 4.0 (I40) hace referencia a la cuarta evolución tecnológica por la que la industria esta pasando actualmente. Esta pretende, a traves de tecnologías como sistemas ciberfísicos (CPS - cyber physical systems)y el Internet de las cosas, crear una producción inteligente, descentralizada y autónoma con sistemas trabajando en tiempo real, con la finalidad de brindar al cliente un servicio lo mas personalizado posible. Los Sistemas Embebidos proporcionan la base para la creación de los CPS, los cuales a su vez se relacionan para crear un Internet de las Cosas, quien se combina con el Internet de los Servicios para hacer posible la I40. 46

En este contexto, la recopilación y evaluación integral de datos de diferentes fuentes (equipos y sistemas de producción, así como sistemas de gestión) se convertirán en estándar para soportar la toma de decisiones en tiempo real 53. Ademas, con el aumento de la conectividad y el uso de protocolos de comunicaciones estándar que vienen con la I40, la necesidad de proteger los sistemas industriales críticos y las líneas de fabricación de las amenazas, aumenta dramáticamente 53. Por lo tanto, definir los estándares de comunicación mas eficientes, se vuelve algo fundamental al momento de querer construir un sistema CPS, ya que el núcleo computacional de un CPS es un sistema de tiempo-real estricto, es decir, que es absolutamente necesesario que produzca respuestas correctas dentro de un intervalo de tiempo definido, si el tiempo de respuesta excede ese límite, se produce un funcionamiento erróneo en el sistema. 47

La red de comunicaciones del CPS debe también debe ofrecer prestaciones de tiempo-real estricto. Tecnologías de red tales como Ethernet, WiFi, Bluetooth o ZigBee, utilizan protocolos con la política del mejor esfuerzo, ya que si bien ofrecen un muy buen rendimiento, no hay garantía del tiempo de entrega de los mensajes. Por otra parte, recientemente se han propuesto protocolos con prestaciones de tiempo-real estricto, tales como Real-Time Ethernet, WirelessHART o ISA 100, que en teoría son mas adecuados para ser utilizados en los CPS y en aplicaciones de la I40. Sin embargo, no existen estudios comparativos de su desempeño ni se ha experimentado que tan superiores son con respecto a los protocolos de redes de propósito general. Por otra parte, es importante conocer qué soporte existe para su uso en SCP, su facilidad de instalación, el soporte de bibliotecas para el desarrollo de aplicaciones, entre otros parámetros. 48

El desarrollo de este trabajo busca identificar cual es el protocolo de redes de computadoras más conveniente para el desarrollo de aplicaciones en el contexto de la I40, así como presentar resultado de un estudio comparativo de los protocolos de red más importantes de la actualidad.

Objetivos generales.

Evaluar los protocolos de redes de computadoras más importantes, para poder determinar cual el más conveniente para ser utilizado en el contexto de alguna aplicación específica de la Industria 4.0.

Objetivos específicos. 

- Definir cuales son las características con las que debe cumplir un protocolo para que pueda ser utilizado en aplicaciones I04. 

- Definir experimentos para determinar si los protocolos cumplen con las características definidas. 

- Determinar y estudiar los protocolos a evaluar con los experimentos predefinidos.

- Evaluar los protocolos elegidos para determinar sus caracteristicas y comprobar su funcionamiento dentro de a I40.



2. Trabajos relacionados

1. Integración de ZigBee/6loWPAN en una red de sensores inalabrica. 
Valorar la viabilidad y los beneficios que proporciona adaptar a un hardware, las tecnologías IEEE 802.15.4, SWAN, 6LoWPAN y ZigBee, para crear redes de sensores inalambricas es el objetivo de este proyecto. 49

Se realiza una descripción general de las redes de sensores inalámbricas, mostrando las caracteristicas mas importantes y algunos ejemplos de aplicación. 49


2. A Comparison of WirelessHART and ZigBee for Industrial Applications. 

En este documento se presenta WirelessHART, un estándar de red inalámbrica industrial, y  se compara con ZigBee en áreas que son interesantes para aplicaciones industriales. La comparación muestra que WirelessHART aborda muchas de las debilidades por las que se ha criticado a ZigBee y, por lo tanto, tiene potencial de éxito en aplicaciones industriales.50

3. A Survey of ZigBee Wireless Sensor Network Technology: Topology, Applications and Challenges.

Este documento ofrece un panorama general de ZigBee como una tecnología basada en una red de sensores inalámbricos y proporciona a los lectores una descripción general de la tecnología de red ZigBee. 51

Durante la descripción de la tecnología, las características de WiFi, Bluetooth y ZigBee se resumen y comparan en una tabla. En ella se muestra que, comparado con WiFi y Bluetooth, la pila de ZigBee tiene un peso más ligero, tiene un mayor rendimiento de transmisión, además, si bien se informa que los dispositivos WiFi tienen entre 8 y 12 horas de duración de la batería y los dispositivos Bluetooth con una duración de la batería de unos pocos días, muchos dispositivos ZigBee pueden presumir de una duración de la batería de hasta 5 años. El enorme ahorro de energía resultó de un rango de transmisión relativamente corto, bajas tasas de transferencia de datos y la pila de protocolos simple de ZigBee. 51


4. BLUETOOTH AND WI-FI WIRELESS PROTOCOLS: A SURVEY AND A COMPARISON.
En este articulo se hace una descripción general de los estándares de comunicación inalámbrica Bluetooth y WiFi,
compara sus principales características y comportamientos en términos de varias métricas, incluida la capacidad, la red
topología, seguridad, calidad de soporte de servicio y consumo de energía. 52



3. Protocolos de redes de computadoras para Internet de las Cosas


1.   WiFi
Wi-Fi o WLAN (redes de área local inalámbricas) es una red inalámbrica basada en una serie de especificaciones del Instituto de Ingenieros Eléctricos y Electrónicos (IEEE) denominada 802.11. Wi Fi utiliza radiofrecuencia sin licencia, principalmente en la banda de 2,4 GHz. Permite que una persona con una computadora o dispositivo movil con capacidad inalámbrica se conecte a Internet a través de un punto de acceso inalámbrico. 
La región geográfica cubierta por uno o varios puntos de acceso se denomina zona activa. El Wi-Fi estaba destinado a ser utilizado para dispositivos móviles y redes de área local, pero ahora se utiliza a menudo para el acceso a Internet en exteriores. 1

1.1  Historia: 
En el año de 1999, varias empresas(3Com, Airones, Intersil, Lucent Technologies, Nokia y Symbol Technologies) se unieron para conseguir formar una asociación sin fines de lucro a fin de mejorar la experiencia de usuario sin importar la marca de los dispositivos, utilizando una nueva tecnología de redes inalámbricas, por el año 2000 se le definió el término “Wi-Fi” para su trabajo técnico y anunció su nombre oficial como Wireless Ethernet Compatibility Alliance o WECA, pero a dia de hoy es llamada Wi-Fi Alliance. 2
A día de hoy la Wi-Fi Alliance está conformada por cientos de empresas en muchos países y poseen una visión común: “Conectar a todos y a todo” , por lo que se dedican a impulsar nuevas tecnologías y aplicaciones para el uso del Wi-Fi. 3


1.2  Organismo que lo desarrolla o regula: 
Wi-Fi Alliance, la cual es una organización que cumple con los estándares 802.11 establecidos por la IEEE. 4

1.3  Principales características:
El estándar 802.11n de la IEEE es casi sinónimo de Wi-Fi , mejora respecto a sus versiones anteriores, en la actualidad, la Wi-Fi Alliance decidió hacer lo mismo que bluetooth esté estándar se llama 802.15.1 oficialmente pero es mejor conocido como bluetooth, entonces sus nuevas versiones son bluetooth 5.0, ahora la Wi-Fi Alliance utilizara Wi-Fi 6 para 802.11ax, Wi-Fi 5 para 802.11ac y Wi-Fi 4 para el 802.11n. 5
“La norma 802.11 sigue el mismo modelo o arquitectura que toda la familia 802, es decir: capa física y capa enlace”, si se mira a mas detalle la subcapa inferior, PMD (Physical Media Dependent), que corresponde al conjunto de especificaciones de cada uno de los sistemas de transmisión a nivel físico, el estándar define cuatro: Infrarrojos, FHSS, DSSS y OFDM.  La subcapa superior, PLCP (Physical Layer Convergence Procedure), se encarga de adaptar las diversas especificaciones de la subcapa PMD a la subcapa MAC, inmediatamente superior. 6
La velocidad máxima teórica del estándar 802.11ax es de 2.4 Gbps manteniendo una frecuencia entre 2.4 y 5 GHz, alcance <100 metros. Estantandar en IoT: Wi-Fi Halow es una designación para productos que incorporan la tecnología IEEE 802.11.ah. Mejora el Wi-Fi al trabajar en un espectro por debajo de 1GHz con más alcance y conectividad de bajo consumo energético ideal para dispositivos IoT. 7


IMAGEN Modelo OSI y la Familia IEEE 802.11 

IMAGEN Resumen del protocolo Wi-Fi IEEE 802.11

1.4  Topologías, tipos de nodos, etc.
Toda red inalámbrica compleja está compuesta por la combinación de una o más tipos de conexiones:
Punto a punto
Punto a multipunto
Multipunto a multipunto

Las redes inalámbricas WiFi contempla tres topologías o configuraciones distintas:

Modo infraestructura o BSS: “Contrario al modo ad hoc donde no hay un elemento central, en el modo de infraestructura hay un elemento de “coordinación”: un punto de acceso o estación base.” 8
Modo ad hoc o IBSS: “También conocido como punto a punto, es un método para que los clientes inalámbricos puedan establecer una comunicación directa entre sí.” 8
Modo ESS: “Se trata de un conjunto de BSS conectados mediante un sistema de distribución. Los puntos de acceso se comunican entre sí para permitir que las estaciones puedan pasar de un BSS a otro sin perder la comunicación, servicio denominado roaming.” 9

1.5  Bibliotecas para el desarrollo de aplicaciones
Biblioteca Wifi para arduino: Para que una placa arduino se conecte a internet tiene que hacer uso de un elemento llamado "Arduino WiFi Shield" y para manipularlo se hace uso de la WiFi Library("#include <WiFi.h>"). 10

WifiManager: es una clase que provee la principal API para manejar todos los aspectos relacionados la conectividad WiFi con java. 11 

Python-wifi: Es un módulo de python que proporciona acceso de lectura y escritura a las capacidades de una tarjeta de red inalámbrica usando las extensiones inalámbricas de linux, para implementarlo utilice “pip instalar python-wifi”. 12

WifiUtils: es una biblioteca que proporciona un conjunto de métodos convenientes para administrar el estado de WiFi, el escaneo de WiFi y la conexión de WiFi a puntos de acceso. 13





2.   Bluetooth
“Bluetooth es una especificación industrial para redes inalámbricas de área personal que posibilita la transmisión de voz y datos entre diferentes dispositivos mediante un enlace por radiofrecuencia.”  14

2.1  Historia: 
Bluetooth fue nombrado en homenaje al rey danés Harald Blatand (Harold Bluetooth en inglés) conocido por ser un gran comunicador y por haber unificado los pueblos de Dinamarca, Noruega y Suiza. Bluetooth fue creado con la intención de comunicar dos tecnologías diferentes, los ordenadores y los teléfonos móviles. A comienzos del año 2000 el Bluetooth SIG lanzó el logo como la fusión de dos letras del alfabeto rúnico,hagall y berkana que representan la H y la B, las iniciales del rey, lo que da como resultado su característico logo. 14
En el año de 1994 Ericsson tenía la necesidad de una conexión entre dispositivos, con una interfaz vía radio y de bajos recursos, se llegó a un sistema basado en la comunicación por radio de corto alcance, llamado Mclink, en el año 1998 nació el Bluetooth Special Interest Group(SIG), promovido por las empresas Ericsson, Nokia, Toshiba, IBM e Intel, al agrupar las empresas líderes de las comunicaciones se llegó al éxito del Bluetooth. 14

2.2  Organismo que lo desarrolla o regula:
Bluetooth Special Interest Group (SIG) es quien regula Bluetooth, en la actualidad SIG está conformado por más de 36000 empresas que buscan unificar, armonizar e impulsar la innovación en el vasto rango de dispositivos conectados que nos rodean. 15

2.3  Principales características:
La siguiente figura muestra un diagrama de la pila de protocolos de Bluetooth. La pila o stack se compone de protocolos específicos de Bluetooth como tecnología inalámbrica, así como del protocolo de búsqueda de servicios SDP, u otros protocolos adoptados como el protocolo de intercambio de objetos OBEX. 16

IMAGEN

- El protocolo de gestión del enlace LMP se responsabiliza del establecimiento y configuración del enlace entre los dispositivos
- La HCI proporciona una interfaz con el módulo radio, el controlador de la banda base y el gestor de enlace.
- El protocolo de control del enlace lógico y adaptación L2CAP abstrae a las capas superiores de los detalles de los protocolos de capas inferiores.
- SDP proporciona a las aplicaciones un medio para realizar búsquedas de servicios y de sus características.
- RFCOMM ofrece una emulación de un puerto serie sobre L2CAP proporcionando el mecanismo de transporte a servicios de capas más altas, permitiendo realizar múltiples conexiones con un dispositivo al mismo tiempo.
- La especificación del protocolo de control telefónico, TCS binary, define la señalización de control de llamadas para el establecimiento de llamadas de voz y datos entre dispositivos Bluetooth. 16

IMAGEN	Comparativa del modelo osi y los protocolos de Bluetooth.


2.4  Topologías, tipos de nodos, etc.
La topología de las redes Bluetooth puede ser punto-a-punto o punto-a multipunto. Los dispositivos, se comunican en redes denominadas piconets. Estas redes tienen posibilidad de crecer hasta tener 8 conexiones punto a punto. Además, se puede extender la red mediante la formación de scatternets, el cual es la red producida cuando dos dispositivos pertenecientes a dos piconets diferentes, se conectan.  En una piconet, un dispositivo debe actuar como master, enviando la información del reloj (para sincronizarse) y la información de los saltos de frecuencia. El resto de los dispositivos actúan como slaves (esclavos). 16

 2.5  Bibliotecas para el desarrollo de aplicaciones
Bluez:  Se trata de una biblioteca que se utiliza para emitir comandos y escuchar eventos de los controladores bluetooth para linux. 17
Node-bluetooth: Es una biblioteca que permite la comunicación de puerto serial bluetooth para Node.js, funciona en Linux y se necesita tener libbluetooth-dev (En Ubuntu/Debian: "$ sudo apt-get install libbluetooth-dev") para instalar node-bluetooth use la siguiente instrucción: "$ npm install node-bluetooth --save". 18
BluetoothManager: Es una biblioteca que facilita el uso de bluetooth en las aplicaciones de windows 10. 19
Android.bluetooth: La plataforma de android incluye compatibilidad con la pila de red Bluetooth, la cual permite que un dispositivo intercambie datos de manera inalámbrica con otros dispositivos Bluetooth. 20



3.   ZigBee
Zigbee es un estándar de comunicaciones inalámbricas diseñado por la Zigbee Alliance. Es un conjunto estandarizado de soluciones que pueden ser implementadas por cualquier fabricante, está basado en el estándar IEEE 802.15.4 de redes inalámbricas de área personal (Wireless Personal Area Network, WPAN) y tiene como objetivo las aplicaciones que requieren comunicaciones seguras con baja tasa de envío de datos y maximización de la vida útil de sus baterías. 21

3.1  Historia:
La idea de las redes de familia Zigbee fue concebida en el año 1998 donde quedó claro que Wi-Fi y Bluetooth no cubrirán todos los contextos, bajo la necesidad de redes descentralizadas. En el 2003 se aprobó el estándar de la IEEE 802.15.4 , por lo cual Zigbee Alliance alcanzó sus objetivos, y actualmente colabora para crear y desarrollar estándares abiertos para los dispositivos que utilizamos dia a dia. También aporta soluciones abiertas de internet de las cosas, principalmente en la domótica. 22
3.2  Organismo que lo desarrolla o regula:
Zigbee fue desarrollado por Zigbee Alliance y actualmente es quien lo regula, fue establecida en el 2002. 23


3.3  Principales características: 
Zigbee es un sistema ideal para redes domóticas, está diseñado para reemplazar la proliferación de sensores individuales, cubre las necesidades del mercado a un bajo costo, bajo consumo y de manera muy segura. 

(La imagen de arriba muestra la Pila de protocolos IEEE 802.5.4)

Está basado en el estándar IEEE 802.15.4 de redes inalámbricas, define el nivel físico y el control de acceso al medio de redes inalámbricas de área personal con tasas bajas de transmisión de datos. 
Opera en la banda libre de ISM 2.4 GHz para conexiones inalámbricas con un alojamiento de 16 bits a 64 bits de dirección extendida y 128-bit AES de cifrado por lo cual provee conexiones seguras entre dispositivos, además, su alcance es de 10 m a 75m. 24

3.4  Topologías, tipos de nodos, etc.
Zigbee permite tres topologías de red:
Topología de estrella: El coordinador se sitúa en el centro.
Topología en árbol: El coordinador será la raíz del árbol.
Topología de malla: Al menos uno de los nodos tendrá más de dos conexiones.
Hasta 65.000 nodos en una red (Las limitaciones físicas no permiten tantos nodos, por ej. ancho de banda o memoria.) 25

(IMAGEN)

	La topología más interesante es la de malla, la cual permite que si, en un momento dado, un nodo del camino falla y se cae, pueda seguir la comunicación entre todos los demás nodos debido a que se rehacen todos los caminos. 25

3.5  Bibliotecas para el desarrollo de aplicaciones

Una biblioteca de .Net Multiplataforma para la comunicación entre dispositivos Zigbee es: ZigBeeNet,  debido a que Zigbee es solo una especificación, necesita una pila de un fabricante que la implemente. ZigBeeNet se desarrolla con una separación estricta de Zigbee Cluster Library (ZCL) y las distintas pilas de fabricantes. Por eso, es posible utilizar hardware diferente para su solución Zigbee. 26

Para aplicaciones con C#: XBee C# Library es una API desarrollada en C # que le permite interactuar con los módulos de radiofrecuencia (RF), ofrecida por Digi International, empresa que ofrece soluciones para IoT. Posee dos módulos, XBeeLibrary.Core, contiene el código común para cualquier plataforma y XBeeLibrary.Xamarin, la cual contiene las API´s necesarias para desarrollar aplicaciones móviles multiplataforma en Xamarin para comunicarse con dispositivos Xbee sobre Bluetooth de baja energía. 27

XBee Java Library: APi desarrollada en java que permite interactuar con los módulos de radiofrecuencia XBee de Digi International, también existe una biblioteca llamada XBee Android ofrecida igualmente por Digi International basada en XBee Java library, la cual hace énfasis en el desarrollo de aplicaciones móviles para android, de igual manera ofrece soporte para diferentes interfaces de comunicación con dispositivos XBee por Bluetooh, usb o por el puerto serial. 28

XBee Python: Esta es una biblioteca para python, la cual utiliza el módulo PySerial para comunicarse con los módulos de radio, también utiliza el módulo SRP para una autenticación entre dispositivos a través de Bluetooth de baja energía, ambos módulos se descargan de manera autónoma al instalar la biblioteca mediante: “pip install digi-xbee”. 29

4.   LoRa
LoRa es una tecnología inalámbrica (al igual que WiFi, Bluetooth o Zigbee) que emplea un tipo de modulación en radiofrecuencia patentado por Semtech, utiliza la tecnología de modulación se denomina Chirp Spread Spectrum (o CSS) y se emplea en comunicaciones militares y espaciales desde hace décadas. 30

4.1  Historia:
LoRa (significa Long Range) es una tecnica de modulacion de red de área amplia de baja potencia, se basa en técnicas de modulación de espectro y fue desarrollada por Semtech, proveedor de semiconductores analogicos, de señal mixta y algoritmos avanzados para consumidores de informática empresarial. Hoy en dia, LoRa es administrado por LoRa Alliance quien es una organización creada en el 2015 para admitir el protocolo LoRaWAN y garantizar la interoperabilidad de los productos que utilizaban esta tecnología, actualmente posee más de 500 miembros, entre ellos IBM, Cisco y Semtech. 30

4.2  Organismo que lo desarrolla o regula:
En la actualidad LoRa está administrada por la LoRa Alliance, quien certifica a todo fabricante de hardware que desee trabajar con esta tecnología. LoRa Alliance es una organización sin fines de lucro que se a convertido en una de las más grandes alianzas del sector tecnológico, comprometida a permitir el despliegue a gran escala con un bajo nivel energético en redes de área amplia (LPWAN) IO a través del desarrollo y la promoción de LoRaWAN. 31

4.3  Principales características:
LoRaWAN es un protocolo de red que usa la tecnología LoRa, para redes de baja potencia y área amplia, LPWAN (Low Power Wide Area Network) empleado para comunicar y administrar dispositivos LoRa. El protocolo LoRaWAN se compone de gateways y nodos:
gateways (antenas): son los encargados de recibir y enviar información a los nodos.
nodos (dispositivos): son los dispositivos finales que envían y reciben información hacia el gateway. 32

Características
LoRa posee una alta tolerancia a las interferencias, alta sensibilidad para recibir datos (-168dB), es basada modulación “chirp” y su bajo consumo energético rinde hasta 10 años con una sola batería.
Alcance de 10 a 20 km
Baja transferencia de datos (hasta 255 bytes)
Conexión punto a punto
Frecuencias de trabajo: 868 Mhz en Europa, 915 Mhz en América, y 433 Mhz en Asia. 
LoRa es una tecnología ideal para conexiones a grandes distancias y para redes de IoT en las que se necesitan sensores que no dispongan de corriente eléctrica de red, teniendo grandes aplicaciones: 
Para Smart Cities (ciudades inteligentes)
Lugares con poca cobertura (cómo explotaciones agrícolas o ganaderas en el campo)
Para construir redes privadas de sensores y/o actuadores. 32
 
4.4  Topologías, tipos de nodos, etc.
Permite la interconexión entre objetos inteligentes sin la necesidad de instalaciones locales complejas, y además otorga amplia libertad de uso al usuario final, al desarrollador y a las empresas que quieran instalar su propia red para Internet de las Cosas (IoT). 32
 
4.5  Bibliotecas para el desarrollo de aplicaciones
Arduino-LoRa: Una biblioteca Arduino para enviar y recibir datos usando radios LoRa. Admite placas / shields basados ​​en Semtech SX1276 /77/78/79. 33

pyLoRa: es una adaptación y una versión mejorada del trabajo mayeranalytic original (mayeranalytics / pySX127x) pyLoRa se puede usar para comunicarse con Arduino a través de la biblioteca RADIOHEAD, la cual es una biblioteca orientada a objetos para enviar y recibir mensajes empaquetados a través de una variedad de radios de datos comunes y otros transportes en una variedad de microprocesadores integrados. 34

ESP32_LoRaWAN: Proporciona una implementación de LoRaWAN Clase A y Clase C bastante completa, este es un puerto de la biblioteca LoRaWAN del nodo LoRaMac de Semtech para el ESP32 y solo se puede utilizar para productos Heltec ESP32 LoRa (Se requiere licencia para su uso). 35

5.   6LoWPAN
Un LoWPAN es una red de comunicación simple de bajo costo que permite conectividad inalámbrica en aplicaciones con energía limitada y relajada. Un LoWPAN generalmente incluye dispositivos que trabajan juntos para conectar el entorno físico con el mundo real, por ejemplo, sensores inalámbricos.
6LoWPAN es un estándar que posibilita el uso de IPv6 sobre redes basadas en el estándar IEEE 802.15.4. 36

5.1  Historia:
El grupo de trabajo IETF 6LoWPAN se inició oficialmente en 2005, aunque la historia de la propiedad intelectual integrada se remonta más atrás. Las primeras especificaciones de 6LoWPAN se publicaron en 2007, primero con un RFC informativo [RFC4919] que especifica los requisitos y objetivos subyacentes de la estandarización inicial, y luego con un RFC de seguimiento estándar [RFC4944] que especifica el formato y la funcionalidad de 6LoWPAN. 37
En 2008 se formó un nuevo grupo de trabajo IETF, este especifica los requisitos y las soluciones de enrutamiento para redes inalámbricas, poco fiables y de bajo consumo.
En 2008, ISA comenzó a estandarizar un sistema de automatización industrial inalámbrico llamado SP100.11a (también conocido como ISA100), que se basa en 6LoWPAN. 
Aún por el 2008 la  IP500 Alliance, comenzó a desarrollar una recomendación para 6LowPAN sobre IEEE 802.15.4
En 2009 Zigbee Alliance anunció que comenzará a integrar estándares IETF como 6LowPAN en sus especificaciones futuras. 37

5.2  Organismo que lo desarrolla o regula: 
El grupo de trabajo IETF 6LoWPAN. El IETF( Internet Engineering Task Force) es una gran comunidad internacional abierta de diseñadores, operadores, comercializadores e investigadores de redes que se ocupan de la evolución de Internet y su buen funcionamiento. 38

5.3  Principales características: 
6LowPAN ha sido diseñada para enviar paquetes IPv6 sobre redes IEEE 802.15.4, para luego implementar protocolos superiores como TCP, UDP, HTTP, COAP, MQTT y websockets. Como se puede apreciar en la imagen de abajo, 6LowPAN se encuentra entre las capas de enlace y red, pudiendo llegar incluso a la capa de transporte compitiendo con otros protocolos que a su vez también incorporan la especificación 6LowPAN. 39


5.4  Topologías, tipos de nodos, etc.
6LowPAN es una red de topología en malla robusta, escalable y auto-regenerativa, también puede funcionar en topología de tipo estrella, y en general las mismas que el estándar 802.15.4. 

 
6.   Wireless HART
WirelessHART es un estándar (62591) global aprobado por la IEC que especifica una tecnología de malla interoperable auto-organizada en el cual los dispositivos de la red inalámbrica mitigan obstáculos en el ambiente del proceso, posee excelentes habilidades para el control y monitoreo, con una gran confiabilidad. 40

6.1  Historia:
El estándar comenzó en el 2004 y fue desarrollado por 37 empresas de la fundación de comunicaciones HART (HCF), entre las empresas pertenecientes se encuentran: Emerson, Pepperl y Siemens. WirelessHART fue aprobada por la junta directiva de la fundación en el año 2007, posteriormente en el 2009 WirelessHART fue aprobado por la Comisión Electrotécnica Internacional (IEC). 41

6.2  Organismo que lo desarrolla o regula: 
FieldComm Group es propietario de las especificaciones HART y proporciona servicios de registro de productos, capacitación y desarrollo de especificaciones asociados con la tecnología. 42
6.3  Principales características: 
WirelessHART es un protocolo basado en la especificación IEEE 802.15.4 en las capas bajas del modelo OSI, al igual que Zigbee o ISA 100, cubriendo la especificación de las capas superiores. La característica principal de este protocolo es su bajo consumo, lo que le permite ser usado en dispositivos alimentados por baterías sin agotarse en un corto periodo de tiempo. 43

(IMAGEN)En la siguiente imagen se puede observar la relación entre WirelessHART y el modelo OSI. 43


6.4  Topologías, tipos de nodos, etc.
WirelessHART utiliza una red mallada plana en la que todas las estaciones de radio (dispositivos de campo) forman una red. Todas las estaciones actúan simultáneamente como fuente de señal y repetidor. 44
El transmisor original envía un mensaje a su vecino más próximo, que a su vez transmite el mensaje hasta que llega a la estación base y al receptor. Además, se configuran rutas alternativas en la fase de inicialización. Si el mensaje no se puede transmitir en una ruta concreta por causa de un obstáculo o un receptor defectuoso, el mensaje se transmite automáticamente a una ruta alternativa. Además de ampliar el rango de la red, la red mallada plana proporciona rutas de comunicación redundantes que aumentan la fiabilidad de la transmisión. 44

(IMAGEN) Topología de red mallada WirelessHART 44

6.5  Bibliotecas para el desarrollo de aplicaciones
SmartMesh SDK: Smart Mesh SDK es un paquete de Python que simplifica la integración de una red SmartMesh IP o SmartMesh WirelessHART en su aplicación. Implementa la interfaz de programación de aplicaciones (API) del dispositivo al que está conectado. Se incluye un conjunto de aplicaciones de muestra en SmartMesh SDK, lo que permite al programador comprender rápidamente la API y usarla como parte de un sistema más grande. 45



 Referencias

1. 
https://www.researchgate.net/profile/Saliyah-Kahar/publication/232613949_A_Review_of_Wireless_Technology_Usage_for_Mobile_Robot_Controller/links/0fcfd50825b4446099000000/A-Review-of-Wireless-Technology-Usage-for-Mobile-Robot-Controller.pdf
A Review of Wireless Technology Usage for Mobile Robot Controller 

2. Cueto García, C. (2019, 21 junio). 20 años de conexiones inalámbricas. IDG Communications S.A.U. Recuperado 12 de abril de 2021, de https://www.computerworld.es/tecnologia/20-anos-de-conexiones-inalambricas

3. Wi-Fi Alliance. (s. f.). Who We Are | Wi-Fi Alliance. Recuperado 12 de abril de 2021, de https://www.wi-fi.org/who-we-are
Corporación Intel. (2021, 26 marzo). Resumen del protocolo Wi-Fi IEEE 802.11 [Grafico]. Recuperado de https://www.intel.com/content/www/us/en/support/articles/000005725/wireless/legacy-intel-wireless-products.html#legacy

4. García, R. (2021, 1 marzo). Qué es el WiFi y cómo funciona para conectar todo a Internet. ADSLZone. Reuperado de https://www.adslzone.net/reportajes/tecnologia/que-es-wifi-como-funciona/

5. García, A. (2020, 25 mayo). El WiFi cambia de nombre: 802.11ac ahora es Wi-Fi 5 y 802.11ax es Wi-Fi 6. Recuperado 12 de abril de 2021, de https://www.adslzone.net/2018/10/03/wifi-6-nuevos-nombres/

6. Anguís Horno, J. J. (2008). Capítulo 2 - Familia IEEE 802.11. En Redes de Área Local Inalámbricas: Diseño de la WLAN de Wheelers Lane Technology College (1.a ed., Vol. 1, pp. 26–76). Sevilla, España. Recuperado 12 de abril de 2021, de http://bibing.us.es/proyectos/abreproy/11579   

7. Lorenzo, J. A. (2020, 20 agosto). Conoce todo sobre Wi-Fi HaLow, el estándar Wi-Fi para IoT. Recuperado 12 de abril de 2021, de https://www.redeszone.net/tutoriales/redes-wifi/wi-fi-halow-estandar-que-es-iot/

8. Buettrich, S., & Escudero, A. (2017, octubre). Topología e Infraestructura - estructura Básica de Redes Inalámbricas. Universidad Autónoma de Chiapas. Recuperado de, https://unac.edu.pe/images/inventario/documentos/manuales/topologia-e-infraestructura_guia_v02.pdf


9. Recalde, A., & Rodríguez, M. (s. f.). Redes Inalámbricas. Universidad Pública de Navarra. Recuperado 12 de abril de 2021, de https://www.tlm.unavarra.es/~daniel/docencia/rba/rba06_07/trabajos/resumenes/gr01-RedesInalambricas.pdf

10. Arduino - WiFi. (2019, 24 diciembre). Recuperado 18 de abril de 2021, de https://www.arduino.cc/en/Reference/WiFi

11. Copyright (C) 2008 The Android Open Source Project, s. f. https://android.googlesource.com/platform/frameworks/base/+/79a3981/wifi/java/android/net/wifi/WifiManager.java

12. Robinson, S. (2015, 11 noviembre). Python-wifi. Recuperado 18 de abril de 2021, de https://pypi.org/project/python-wifi/

13. Psaridis, A. (2021, 1 febrero). WifiUtils. Recuperado 18 de abril de 2021, de https://github.com/ThanosFisherman/WifiUtils

14. EcuRed. (s. f.-a). Bluetooth - EcuRed. Recuperado 12 de abril de 2021, de https://www.ecured.cu/Bluetooth

15. Bluetooth SIG. (s. f.). Learn about the history of and people behind the bluetooth SIG. Recuperado 12 de abril de 2021, de https://www.bluetooth.com/about-us/

16. E-Reading. (s. f.). Capitulo 2: Bluetooth. Recuperado 12 de abril de 2021, de http://bibing.us.es/proyectos/abreproy/11972/fichero/Cap%C3%ADtulo+2+-+Bluetooth.pdf

17. Bluez - Rust. (s. f.). Recuperado 18 de abril de 2021, de https://docs.rs/bluez/0.3.1/bluez/ 

18. Libraries.io. (2019, 26 agosto). Node-bluetooth on NPM. Recuperado 18 de abril de 2021, de https://libraries.io/npm/node-bluetooth

19. RobMiles.com. (2021, 9 abril). Bluetooth Manager. Recuperado 18 de abril de 2021, de https://www.robmiles.com/bluetooth-manager

20. Android Developers. (s. f.). Bluetooth overview. Recuperado 18 de abril de 2021, de https://developer.android.com/guide/topics/connectivity/bluetooth

21. Tecnología Zigbee. (s. f.). Recuperado 14 de abril de 2021, de https://sites.google.com/site/ingsoftwares5a/home

22. EcuRed. (s. f.-m). ZigBee - EcuRed. Recuperado 14 de abril de 2021, de https://www.ecured.cu/ZigBee#Cronolog.C3.ADa

23. Zigbee Alliance. (2021, 26 marzo). Who We Are. Recuperado 14 de abril de 2021, de https://zigbeealliance.org/about/

24. Callejo, D. (s. f.). Modelo de cobertura para redes inalámbricas de interiores. Recuperado 14 de abril de 2021, de http://bibing.us.es/proyectos/abreproy/11761/direccion/Volumen1%252F 

25. TOPOLOGÍAS - Fundamentos de Ing. Software. (s. f.). Recuperado 14 de abril de 2021, de https://sites.google.com/site/ingsoftwares5a/topologias

26. DiGi International. (2019, 27 junio). Mr-Markus/ZigbeeNet. Recuperado 18 de abril de 2021, de https://github.com/Mr-Markus/ZigbeeNet

27. DiGi International. (2020, 18 agosto). Digidotcom/xbee-csharp. Recuperado 18 de abril de 2021, de https://github.com/digidotcom/xbee-csharp

28. DiGi International. (2019, julio 10). Digidotcom/xbee-java. Recuperado 18 de abril de 2021, de https://github.com/digidotcom/xbee-java

29. DiGi International. (s. f.). Get started with XBee Python library — Digi XBee Python library 1.4.0 documentation. Recuperado 18 de abril de 2021, de https://xbplib.readthedocs.io/en/latest/getting_started_with_xbee_python_library.html

30. AlfaIOT. (s. f.). LoRa - AlfaIOT. Recuperado 13 de abril de 2021, de https://alfaiot.com/tecnologias-iot/lora/

31. LoRa Alliance. (2021, 7 abril). Homepage. Recuperado 13 de abril de 2021, de https://lora-alliance.org/about-lora-alliance/

32. Catsensors. (s. f.). Tecnología LoRA y LoRAWAN - Catsensors. Recuperado 13 de abril de 2021, de https://www.catsensors.com/es/lorawan/tecnologia-lora-y-lorawan

33. Mistry, S. (s. f.). LoRa - Arduino. Recuperado 18 de abril de 2021, de https://www.arduino.cc/reference/en/libraries/lora/

34. Rpsreal. (2019, 23 abril). pyLoRa. Recuperado 18 de abril de 2021, de https://pypi.org/project/pyLoRa/

35. HelTecAutomation. (s. f.). ESP32_LoRaWAN. Recuperado 18 de abril de 2021, de https://github.com/HelTecAutomation/ESP32_LoRaWAN

36. IEEE. (s. f.-a). IEEE 802.15.4a-2007 - IEEE Standard for Information technology-- Local and metropolitan area networks-- Specific requirements-- Part 15.4: Wireless Medium Access Control (MAC) and Physical Layer (PHY) Specifications for Low-Rate Wireless Personal Area Networks (WPANs): Amendment 1: Add Alternate PHYs. Recuperado 14 de abril de 2021, de https://standards.ieee.org/standard/802_15_4a-2007.html

37. EETimes. (2011, 30 mayo). EETimes - 6LoWPAN: The wireless embedded Internet - Part 2: 6LoWPAN history, market perspective & applications. Recuperado 14 de abril de 2021, de https://www.eetimes.com/6lowpan-the-wireless-embedded-internet-part-2-6lowpan-history-market-perspective-applications/

38. IETF. Internet Engineering Task Force. (2018, 21 diciembre). About the IETF. Recuperado 14 de abril de 2021, de https://www.internetsociety.org/es/about-the-ietf/

39. Crespo, E. (2018, 16 noviembre). 6LoWPAN. Recuperado 14 de abril de 2021, de https://aprendiendoarduino.wordpress.com/tag/6lowpan/

40. Instituto Politécnico Nacional, Pérez Martínez, L. (s. f.). Consideraciones de diseño e implementación de un sistema de monitoreo inalámbrico en campo con tecnología WirelesHART. Recuperado 13 de abril de 2021, de https://tesis.ipn.mx/jspui/bitstream/123456789/21026/1/Sistema%20de%20Monitoreo%20WirelessHART%20Rev0.pdf

41. Wikipedia contributors. (2021, 31 enero). WirelessHART. Recuperado 15 de abril de 2021, de https://en.wikipedia.org/wiki/WirelessHART

42. FieldComm Group. (2018, 8 octubre). HART. Recuperado 15 de abril de 2021, de https://www.fieldcommgroup.org/technologies/hart

43. INCIBE. (2019, 27 febrero). Aplicando seguridad en WirelessHART. Recuperado 15 de abril de 2021, de https://www.incibe-cert.es/blog/aplicando-seguridad-wirelesshart

44. Pepperl. (2015, 15 enero). WirelessHART - Tecnología inalámbrica | Soluciones de interface HART. Recuperado 15 de abril de 2021, de https://www.pepperl-fuchs.com/mexico/es/10028.htm

45. Watteyne, T. (2014, 24 octubre). SmartMesh SDK - confluence. Recuperado 18 de abril de 2021, de https://dustcloud.atlassian.net/wiki/spaces/SMSDK/overview

46. William MacDougall (2014, Julio). *Industrie 4.0, Smart manufacturing for the future.* [Online]. https://www.gtai.com/smart-systems 

47. Aldea Rivas, M. (2002, Noviembre). Planificación de Tareas en Sistemas Operativos de Tiempo Real Estricto para Aplicaciones Empotradas https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.102.9880&rep=rep1&type=pdf

48. Sha, L., Abdelzaher, T., årzén, Anton Cervin, Theodore Baker, Alan Burns, Giorgio Buttazzo, Marco Caccamo, John Lehoczky \& Aloysius K. Mok (2004) http://retis.sssup.it/~giorgio/paps/2004/rtsj04-edf.pdf

49. Integración de ZigBee/6loWPAN en una red de sensores inalabrica https://repositorio.unican.es/xmlui/bitstream/handle/10902/1464/349717.pdf?sequence=1&isAllowed=y

50. A Comparison of WirelessHART and ZigBee for Industrial Applications. http://abblibrary.s3.amazonaws.com/public/8043193f45e3cc6a85257bc6006366c8/A%20Comparison%20of%20WirelessHART%20and%20ZigBee%20for%20Industrial%20Applications.pdf

51. A Survey of ZigBee Wireless Sensor Network Technology: Topology, Applications and Challenges. http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.741.6729&rep=rep1&type=pdf

52. BLUETOOTH AND WI-FI WIRELESS PROTOCOLS: A SURVEY AND A COMPARISON. https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.1.1709&rep=rep1&type=pdf

53. Michael Rüßmann, Markus Lorenz, Philipp Gerbert, Manuela Waldner, Jan Justus, Pascal Engel, and Michael Harnisch (2015, Abril 09.). *Industry 4.0: The Future of Productivity and Growth in Manufacturing Industries.* [Online] https://www.bcg.com/publications

54. Network Jitter – What is it and How to Monitor it with Software/Tools https://www.pcwdld.com/network-jitter





Castellano, Á. R. (2011). Bluetooth. Introducción a su Funcionamiento. Recuperado 12 de abril de 2021, de http://www.sistemamid.com/panel/uploads/biblioteca/1/619/640/641/3792.pdf
E-Reading, & Gutiérrez, R. (s. f.). Redes Bluetooth. Recuperado 12 de abril de 2021, de http://bibing.us.es/proyectos/abreproy/40048/direccion/VOLUMEN+1.+MEMORIA%252F
P. García, C. (2017). Zigbee, Comunicación para Dispositivos. Recuperado 14 de abril de 2021, de https://sg.com.mx/content/view/310
Kyun Ng, C. (2010, 1 mayo). Path recovery mechanism in 6LoWPAN routing. Recuperado 15 de abril de 2021, de https://ieeexplore.ieee.org/document/5556760
ISA100 WIRELESS Compliance Institute. (s. f.). The Technology Behind the ISA100.11a Standard – An Exploration. Recuperado 16 de abril de 2021, de http://isa100wci.org/Documents/PDF/The-Technology-Behind-ISA100-11a-v-3_pptx.aspx
International Society of Automation. (s. f.). About ISA - The Home for Automation. Recuperado 16 de abril de 2021, de https://www.isa.org/about-isa
Chen, P. (2014, 6 marzo). ISA100 Is Ready for the Internet of Things. Recuperado 17 de abril de 2021, de https://www.pumpsandsystems.com/isa100-ready-internet-things
Zigbee Alliance. (2016, 14 febrero). ZigBee Cluster Library Specification. Recuperado 18 de abril de 2021, de https://zigbeealliance.org/wp-content/uploads/2019/12/07-5123-06-zigbee-cluster-library-specification.pdf
Copyright (C) 2008 The Android Open Source Project. (s. f.). WifiManager. Recuperado 18 de abril de 2021, de https://android.googlesource.com/platform/frameworks/base/+/79a3981/wifi/java/android/net/wifi/WifiManager.java
Cisco. (2020, 13 julio). ¿Qué es la tecnología wifi? Recuperado 12 de abril de 2021, de https://www.cisco.com/c/es_mx/products/wireless/what-is-wifi.html