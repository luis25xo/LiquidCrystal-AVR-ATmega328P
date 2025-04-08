/*
 * File:   main.c
 * Author: Luis25XO
 *
 * Created on April 5, 2025, 1:07 PM
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LiquidCrystal.h"

/*
 * Asignación de pines para el LCD:
 * - El primer parámetro (2) corresponde al pin RS del LCD.
 * - El segundo parámetro (3) corresponde al pin Enable del LCD.
 * - Los siguientes parámetros (4, 5, 6, 7) corresponden a los pines D4, D5, D6 y D7 del LCD.
 * 
 * Reglas para asignar los pines:
 * 1. Los números de pines deben corresponder a los pines físicos del microcontrolador ATmega328P.
 *    - Pines 0-7: PORTD (PD0-PD7)
 *    - Pines 8-13: PORTB (PB0-PB5)
 *    - Pines 14-19: PORTC (PC0-PC5)(A0-A5 en Arduino)
 * 2. Asegúrate de que los pines seleccionados no estén siendo utilizados por otros periféricos (UART, SPI, I2C, etc.).
 * 3. Los pines deben configurarse como salidas en el código (esto se realiza automáticamente en la inicialización del LCD).
 * 4. Verifica que las conexiones físicas del hardware coincidan con los pines configurados en el software.
 */

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
    }
}
