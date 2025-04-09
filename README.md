# LiquidCrystal-AVR-ATmega328P

La biblioteca `LiquidCrystal` proporciona una implementación para controlar pantallas LCD basadas en el controlador HD44780 con microcontroladores AVR como el ATmega328P. Esta biblioteca está escrita en C y contiene las siguientes partes principales:

- **LiquidCrystal.c**: Implementación principal de las funciones para interactuar con el LCD.
- **LiquidCrystal.h**: Declaraciones de funciones, macros y configuraciones necesarias para usar la biblioteca.
- **main.c**: Ejemplo de uso que muestra cómo inicializar y usar la biblioteca para controlar una pantalla LCD.

## Características
- Soporte para pantallas LCD de 16x2, 20x4 y otras basadas en HD44780.
- Configuración en modo de 4 bits.
- Funciones para escribir texto, mover el cursor y limpiar la pantalla.

## Requisitos
- Un microcontrolador AVR, como el ATmega328P.
- Pantalla LCD compatible con HD44780.
- Conexión adecuada entre el microcontrolador y el LCD (4 bits o 8 bits).

## Instalación
1. Clona este repositorio:
   ```bash
   git clone https://github.com/luis25xo/LiquidCrystal-AVR-ATmega328P.git
   cd LiquidCrystal-AVR-ATmega328P
   ```

2. Compila el proyecto usando `avr-gcc` o cualquier herramienta de compilación AVR que prefieras:
   ```bash
   make
   ```

3. Sube el archivo binario al microcontrolador usando `avrdude`:
   ```bash
   make flash
   ```

## Uso
### Archivos principales
1. **LiquidCrystal.h**: Incluye las definiciones de pines y las funciones para interactuar con el LCD.
2. **LiquidCrystal.c**: Implementa las funciones del LCD, como inicialización, escritura y control.
3. **main.c**: Ejemplo de programa principal que utiliza la biblioteca `LiquidCrystal`.

### Ejemplo de prueba en `main.c`
El archivo `main.c` contiene un ejemplo básico para probar la biblioteca. A continuación se muestra una breve descripción de lo que hace:

1. Configura los pines del microcontrolador para comunicarse con el LCD.
2. Inicializa la biblioteca `LiquidCrystal`.
3. Escribe un mensaje en la pantalla LCD.
4. Realiza operaciones como mover el cursor y limpiar la pantalla.

```c
#include "LiquidCrystal.h"

int main(void) {
    // Inicialización de la pantalla LCD
    LiquidCrystal lcd;
    LiquidCrystal_init(&lcd);

    // Escribir un mensaje en la pantalla
    LiquidCrystal_print(&lcd, "Hola, mundo!");

    // Mover el cursor
    LiquidCrystal_setCursor(&lcd, 0, 1);
    LiquidCrystal_print(&lcd, "AVR Rocks!");

    // Bucle infinito
    while (1) {
        // Puedes agregar más lógica aquí
    }

    return 0;
}
```

### Conexión de los pines
Asegúrate de conectar los pines del LCD a los pines correctos del microcontrolador. La configuración predeterminada está definida en `LiquidCrystal.h`. Si necesitas cambiarla, ajusta las macros en el archivo de encabezado.

## Contribuciones
Si deseas mejorar esta biblioteca, por favor envía un pull request o abre un issue en este repositorio. ¡Son bienvenidas las contribuciones de la comunidad!

## Licencia
Este proyecto está licenciado bajo la licencia MIT. Consulta el archivo `LICENSE` para más detalles.
