#include "LiquidCrystal.h"

/*
 * Asignación de pines para el LCD en main.c:
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


// Comandos del LCD
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// Flags para display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags para display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// Flags para display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// Flags para function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// Funciones auxiliares privadas
static void _digitalWrite(volatile uint8_t *port, uint8_t pin, uint8_t value) {
    if (value) {
        *port |= (1 << pin);
    } else {
        *port &= ~(1 << pin);
    }
}

static void _pulseEnable(struct LiquidCrystal_t *lcd) {
    _digitalWrite(lcd->enable_port, lcd->enable_pin, 0);
    _delay_us(1);
    _digitalWrite(lcd->enable_port, lcd->enable_pin, 1);
    _delay_us(1);
    _digitalWrite(lcd->enable_port, lcd->enable_pin, 0);
    _delay_us(100);
}

static void _write4bits(struct LiquidCrystal_t *lcd, uint8_t value) {
    _digitalWrite(lcd->d4_port, lcd->d4_pin, (value >> 0) & 0x01);
    _digitalWrite(lcd->d5_port, lcd->d5_pin, (value >> 1) & 0x01);
    _digitalWrite(lcd->d6_port, lcd->d6_pin, (value >> 2) & 0x01);
    _digitalWrite(lcd->d7_port, lcd->d7_pin, (value >> 3) & 0x01);
    _pulseEnable(lcd);
}

// Función para verificar si el LCD está ocupado
static uint8_t _isBusy(struct LiquidCrystal_t *lcd) {
    uint8_t result;
    // Configurar los pines de datos como entrada
    *(lcd->d4_port - 1) &= ~(1 << lcd->d4_pin);
    *(lcd->d5_port - 1) &= ~(1 << lcd->d5_pin);
    *(lcd->d6_port - 1) &= ~(1 << lcd->d6_pin);
    *(lcd->d7_port - 1) &= ~(1 << lcd->d7_pin);
    
    _digitalWrite(lcd->rs_port, lcd->rs_pin, 0);
    _digitalWrite(lcd->enable_port, lcd->enable_pin, 1);
    _delay_us(1);
    
    result = ((*lcd->d7_port >> lcd->d7_pin) & 1) << 7;
    result |= ((*lcd->d6_port >> lcd->d6_pin) & 1) << 6;
    result |= ((*lcd->d5_port >> lcd->d5_pin) & 1) << 5;
    result |= ((*lcd->d4_port >> lcd->d4_pin) & 1) << 4;
    
    _digitalWrite(lcd->enable_port, lcd->enable_pin, 0);
    _delay_us(1);
    _digitalWrite(lcd->enable_port, lcd->enable_pin, 1);
    _delay_us(1);
    
    result |= ((*lcd->d7_port >> lcd->d7_pin) & 1) << 3;
    result |= ((*lcd->d6_port >> lcd->d6_pin) & 1) << 2;
    result |= ((*lcd->d5_port >> lcd->d5_pin) & 1) << 1;
    result |= ((*lcd->d4_port >> lcd->d4_pin) & 1) << 0;
    
    _digitalWrite(lcd->enable_port, lcd->enable_pin, 0);
    
    // Restaurar los pines de datos como salida
    *(lcd->d4_port - 1) |= (1 << lcd->d4_pin);
    *(lcd->d5_port - 1) |= (1 << lcd->d5_pin);
    *(lcd->d6_port - 1) |= (1 << lcd->d6_pin);
    *(lcd->d7_port - 1) |= (1 << lcd->d7_pin);
    
    return result & 0x80; // Retorna el bit de busy
}

// Modificar la función _send para esperar hasta que el LCD esté listo
static void _send(struct LiquidCrystal_t *lcd, uint8_t value, uint8_t mode) {
    _digitalWrite(lcd->rs_port, lcd->rs_pin, mode);
    _write4bits(lcd, value >> 4);
    _write4bits(lcd, value);
}

// Función simple de espera para el LCD
static void _waitForLCD(struct LiquidCrystal_t *lcd) {
    // Pequeña espera para asegurar que el LCD procese el comando
    for (volatile uint16_t i = 0; i < 1000; i++) {
        __asm__("nop");
    }
}

// Implementaciones de las funciones miembro
static void _begin(struct LiquidCrystal_t *lcd, uint8_t cols, uint8_t rows) {
    lcd->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    
    if (rows > 1) {
        lcd->_displayfunction |= LCD_2LINE;
    }
    
    lcd->_numlines = rows;
    lcd->_currline = 0;
    
    // Inicialización del LCD
    _delay_ms(50);
    _digitalWrite(lcd->rs_port, lcd->rs_pin, 0);
    _digitalWrite(lcd->enable_port, lcd->enable_pin, 0);
    
    // Secuencia de inicialización
    _write4bits(lcd, 0x03);
    _delay_ms(5);
    _write4bits(lcd, 0x03);
    _delay_ms(5);
    _write4bits(lcd, 0x03);
    _delay_ms(1);
    _write4bits(lcd, 0x02);
    
    // Configuración del display
    _send(lcd, LCD_FUNCTIONSET | lcd->_displayfunction, 0);
    _delay_ms(5);  // Espera adicional después de la configuración
    
    lcd->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd->display(lcd);
    _delay_ms(5);  // Espera adicional después de encender el display
    
    lcd->clear(lcd);
    _delay_ms(5);  // Espera adicional después de limpiar
    
    lcd->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    _send(lcd, LCD_ENTRYMODESET | lcd->_displaymode, 0);
    _delay_ms(5);  // Espera adicional después de configurar el modo
}

static void _clear(struct LiquidCrystal_t *lcd) {
    _send(lcd, LCD_CLEARDISPLAY, 0);
    _waitForLCD(lcd);
}

static void _home(struct LiquidCrystal_t *lcd) {
    _send(lcd, LCD_RETURNHOME, 0);
    _waitForLCD(lcd);
}

static void _setCursor(struct LiquidCrystal_t *lcd, uint8_t col, uint8_t row) {
    const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row >= lcd->_numlines) {
        row = lcd->_numlines - 1;
    }
    _send(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]), 0);
}

static void _print(struct LiquidCrystal_t *lcd, const char *str) {
    while (*str) {
        lcd->printChar(lcd, *str++);
    }
}

static void _printChar(struct LiquidCrystal_t *lcd, char c) {
    _send(lcd, c, 1);
}

static void _cursor(struct LiquidCrystal_t *lcd) {
    lcd->_displaycontrol |= LCD_CURSORON;
    _send(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol, 0);
}

static void _noCursor(struct LiquidCrystal_t *lcd) {
    lcd->_displaycontrol &= ~LCD_CURSORON;
    _send(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol, 0);
}

static void _blink(struct LiquidCrystal_t *lcd) {
    lcd->_displaycontrol |= LCD_BLINKON;
    _send(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol, 0);
}

static void _noBlink(struct LiquidCrystal_t *lcd) {
    lcd->_displaycontrol &= ~LCD_BLINKON;
    _send(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol, 0);
}

static void _display(struct LiquidCrystal_t *lcd) {
    lcd->_displaycontrol |= LCD_DISPLAYON;
    _send(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol, 0);
}

static void _noDisplay(struct LiquidCrystal_t *lcd) {
    lcd->_displaycontrol &= ~LCD_DISPLAYON;
    _send(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol, 0);
}

static void _scrollDisplayLeft(struct LiquidCrystal_t *lcd) {
    _send(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, 0);
}

static void _scrollDisplayRight(struct LiquidCrystal_t *lcd) {
    _send(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, 0);
}

static void _autoscroll(struct LiquidCrystal_t *lcd) {
    lcd->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
    _send(lcd, LCD_ENTRYMODESET | lcd->_displaymode, 0);
}

static void _noAutoscroll(struct LiquidCrystal_t *lcd) {
    lcd->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    _send(lcd, LCD_ENTRYMODESET | lcd->_displaymode, 0);
}

static void _createChar(struct LiquidCrystal_t *lcd, uint8_t location, const uint8_t charmap[]) {
    location &= 0x7;
    _send(lcd, LCD_SETCGRAMADDR | (location << 3), 0);
    for (uint8_t i = 0; i < 8; i++) {
        _send(lcd, charmap[i], 1);
    }
}

// Función auxiliar para obtener el puerto y pin a partir de un número de pin
static void _getPortAndPin(uint8_t pin, volatile uint8_t **port, uint8_t *pin_num) {
    if (pin >= 0 && pin <= 7) {
        *port = &PORTD;
        *pin_num = pin;
    } else if (pin >= 8 && pin <= 13) {
        *port = &PORTB;
        *pin_num = pin - 8;
    } else if (pin >= 14 && pin <= 19) {
        *port = &PORTC;
        *pin_num = pin - 14;
    } else {
        // Por defecto, usar PORTD pin 0
        *port = &PORTD;
        *pin_num = 0;
    }
}

// Función de inicialización
void LiquidCrystal_init(LiquidCrystal_t *lcd, 
                       volatile uint8_t *rs_port, uint8_t rs_pin,
                       volatile uint8_t *enable_port, uint8_t enable_pin,
                       volatile uint8_t *d4_port, uint8_t d4_pin,
                       volatile uint8_t *d5_port, uint8_t d5_pin,
                       volatile uint8_t *d6_port, uint8_t d6_pin,
                       volatile uint8_t *d7_port, uint8_t d7_pin) {
    // Inicializar variables miembro
    lcd->rs_port = rs_port;
    lcd->rs_pin = rs_pin;
    lcd->enable_port = enable_port;
    lcd->enable_pin = enable_pin;
    lcd->d4_port = d4_port;
    lcd->d4_pin = d4_pin;
    lcd->d5_port = d5_port;
    lcd->d5_pin = d5_pin;
    lcd->d6_port = d6_port;
    lcd->d6_pin = d6_pin;
    lcd->d7_port = d7_port;
    lcd->d7_pin = d7_pin;
    
    // Configurar pines como salida
    *(rs_port - 1) |= (1 << rs_pin);
    *(enable_port - 1) |= (1 << enable_pin);
    *(d4_port - 1) |= (1 << d4_pin);
    *(d5_port - 1) |= (1 << d5_pin);
    *(d6_port - 1) |= (1 << d6_pin);
    *(d7_port - 1) |= (1 << d7_pin);
    
    // Inicializar punteros a funciones
    lcd->begin = _begin;
    lcd->clear = _clear;
    lcd->home = _home;
    lcd->setCursor = _setCursor;
    lcd->print = _print;
    lcd->printChar = _printChar;
    lcd->cursor = _cursor;
    lcd->noCursor = _noCursor;
    lcd->blink = _blink;
    lcd->noBlink = _noBlink;
    lcd->display = _display;
    lcd->noDisplay = _noDisplay;
    lcd->scrollDisplayLeft = _scrollDisplayLeft;
    lcd->scrollDisplayRight = _scrollDisplayRight;
    lcd->autoscroll = _autoscroll;
    lcd->noAutoscroll = _noAutoscroll;
    lcd->createChar = _createChar;
}

// Constructor simplificado
void LiquidCrystal(LiquidCrystal_t *lcd, uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
    volatile uint8_t *rs_port, *enable_port, *d4_port, *d5_port, *d6_port, *d7_port;
    uint8_t rs_pin, enable_pin, d4_pin, d5_pin, d6_pin, d7_pin;
    
    // Obtener los puertos y pines correspondientes
    _getPortAndPin(rs, &rs_port, &rs_pin);
    _getPortAndPin(enable, &enable_port, &enable_pin);
    _getPortAndPin(d4, &d4_port, &d4_pin);
    _getPortAndPin(d5, &d5_port, &d5_pin);
    _getPortAndPin(d6, &d6_port, &d6_pin);
    _getPortAndPin(d7, &d7_port, &d7_pin);
    
    // Inicializar el LCD con los pines obtenidos
    LiquidCrystal_init(lcd, 
                      rs_port, rs_pin,
                      enable_port, enable_pin,
                      d4_port, d4_pin,
                      d5_port, d5_pin,
                      d6_port, d6_pin,
                      d7_port, d7_pin);
}

