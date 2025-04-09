# LiquidCrystal-AVR-ATmega328P

La biblioteca `LiquidCrystal` proporciona una implementación para controlar pantallas LCD basadas en el controlador HD44780 con microcontroladores AVR como el ATmega328P. Esta biblioteca está escrita en C y contiene las siguientes partes principales:

- **LiquidCrystal.c**: Implementación principal de las funciones para interactuar con el LCD.
- **LiquidCrystal.h**: Declaraciones de funciones, macros y configuraciones necesarias para usar la biblioteca.
- **main.c**: Ejemplo de uso que muestra cómo inicializar y usar la biblioteca para controlar una pantalla LCD.

## Características
- Soporte para pantallas LCD de 16x2 basadas en HD44780.
- Configuración en modo de 4 bits.
- Funciones para escribir texto, mover el cursor y limpiar la pantalla.

## Requisitos
- Microcontrolador AVR ATmega328P.
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
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LiquidCrystal.h"

int main(void) {
    LiquidCrystal_t lcd;
    LiquidCrystal(&lcd, 2, 3, 4, 5, 6, 7); // Configuración de pines para el LCD
    lcd.begin(&lcd, 16, 2);
    lcd.print(&lcd, "Hello World!");
    lcd.setCursor(&lcd, 0, 1);
    lcd.print(&lcd, "Hola Mundo!");
    
    // Esperar 5 segundos
    _delay_ms(5000);
    
    // Limpiar la pantalla y mostrar nuevos mensajes
    lcd.clear(&lcd);
    lcd.home(&lcd);
    lcd.print(&lcd, "AVR");
    lcd.setCursor(&lcd, 0, 1);
    lcd.print(&lcd, "ATmega328P");
    
    while (1) {
        // Bucle infinito
    }
    
    return 0;
}
```

## Conexión de los pines
Asegúrate de conectar los pines del LCD a los pines correctos del microcontrolador. La configuración predeterminada está definida en `LiquidCrystal.h`. El microcontrolador tiene tres puertos principales:
- PORTD (Pines 0-7): Corresponden a los pines digitales 0-7. En nuestro ejemplo, usamos PD2 a PD7.
- PORTB (Pines 8-13): Corresponden a los pines digitales PB0–PB5
- PORTC (Pines 14-19): Corresponden a los pines analógicos (PC0–PC5)
### Consideraciones Importantes:
- Los pines 0 y 1 (PD0 y PD1) generalmente se evitan porque son para comunicación UART (TX/RX)
- Asegúrate de que los pines seleccionados no estén siendo utilizados por otros periféricos (UART, SPI, I2C, etc.).
- Los pines deben configurarse como salidas en el código (esto se realiza automáticamente en la inicialización del LCD).
- Verifica que las conexiones físicas del hardware coincidan con los pines configurados en el software.

## Contribuciones
Si deseas mejorar esta biblioteca, por favor envía un pull request o abre un issue en este repositorio. ¡Son bienvenidas las contribuciones de la comunidad!
