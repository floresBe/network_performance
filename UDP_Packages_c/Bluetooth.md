Bluetooth Stack APIs de Silicon Labs

La Bluetooth Stack de Silicon Labs es una pila de protocolos avanzada compatible con Bluetooth 5 que implementa el estándar de baja energía de Bluetooth. Admite múltiples conexiones, funciones simultáneas de central, periférico, emisor y observador. La pila Bluetooth de Silicon Labs está diseñada para módulos y SoC inalámbricos Gecko de Silicon Labs.

La pila Bluetooth de Silicon Labs proporciona varias API para que el desarrollador acceda a la funcionalidad Bluetooth. Se admiten dos modos:

Modo autónomo, donde tanto la Bluetooth Stack como la aplicación se ejecutan en un módulo o SoC inalámbrico de Geckos. La aplicación se puede desarrollar con lenguaje de programación C.

Modo de coprocesador de red (NCP), donde la Bluetooth Stack se ejecuta en un Wireless Gecko y la aplicación se ejecuta en una MCU de host separada. Para este caso de uso, la Bluetooth Stack se puede configurar en modo NCP donde la API se expone a través de una interfaz en serie como UART.

https://docs.silabs.com/bluetooth/latest/






BlueZ
https://people.csail.mit.edu/albert/bluez-intro/c404.html


32feet.NET - Personal Area Networking for .NET
https://archive.codeplex.com/?p=32feet

Warble - mbientlab
https://github.com/mbientlab/Warble/blob/master/README.md
Warble, besides being the chirping/singing of birds, is a wrapper around various Bluetooth LE libraries, providing a common C API for Linux and Windows 10.

Developers can use this library to connect to Bluetooth LE devices, read and write GATT characteristic values, and enable chatacteristic notifications.

On Linux, Warble wraps around the libblepp library, which is included as a submodule. You will need to have BlueZ, Boost headers, and GNU Make installed along with a C++ compiler that support C++14.


Qt Bluetooth C++ Classes