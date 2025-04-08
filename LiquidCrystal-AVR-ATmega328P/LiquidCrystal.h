#ifndef LIQUIDCRYSTAL_H
#define LIQUIDCRYSTAL_H

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

// Definir F_CPU para las funciones de delay
#ifndef F_CPU
#endif

// Definición de la estructura para almacenar los pines y métodos
typedef struct LiquidCrystal_t {
    // Variables miembro
    volatile uint8_t *rs_port;
    uint8_t rs_pin;
    volatile uint8_t *enable_port;
    uint8_t enable_pin;
    volatile uint8_t *d4_port;
    uint8_t d4_pin;
    volatile uint8_t *d5_port;
    uint8_t d5_pin;
    volatile uint8_t *d6_port;
    uint8_t d6_pin;
    volatile uint8_t *d7_port;
    uint8_t d7_pin;
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _numlines;
    uint8_t _currline;
    
    // Métodos
    void (*begin)(struct LiquidCrystal_t*, uint8_t, uint8_t);
    void (*clear)(struct LiquidCrystal_t*);
    void (*home)(struct LiquidCrystal_t*);
    void (*setCursor)(struct LiquidCrystal_t*, uint8_t, uint8_t);
    void (*print)(struct LiquidCrystal_t*, const char*);
    void (*printChar)(struct LiquidCrystal_t*, char);
    void (*cursor)(struct LiquidCrystal_t*);
    void (*noCursor)(struct LiquidCrystal_t*);
    void (*blink)(struct LiquidCrystal_t*);
    void (*noBlink)(struct LiquidCrystal_t*);
    void (*display)(struct LiquidCrystal_t*);
    void (*noDisplay)(struct LiquidCrystal_t*);
    void (*scrollDisplayLeft)(struct LiquidCrystal_t*);
    void (*scrollDisplayRight)(struct LiquidCrystal_t*);
    void (*autoscroll)(struct LiquidCrystal_t*);
    void (*noAutoscroll)(struct LiquidCrystal_t*);
    void (*createChar)(struct LiquidCrystal_t*, uint8_t, const uint8_t*);
} LiquidCrystal_t;

// Prototipos de funciones
void LiquidCrystal_init(LiquidCrystal_t *lcd, 
                       volatile uint8_t *rs_port, uint8_t rs_pin,
                       volatile uint8_t *enable_port, uint8_t enable_pin,
                       volatile uint8_t *d4_port, uint8_t d4_pin,
                       volatile uint8_t *d5_port, uint8_t d5_pin,
                       volatile uint8_t *d6_port, uint8_t d6_pin,
                       volatile uint8_t *d7_port, uint8_t d7_pin);

void LiquidCrystal(LiquidCrystal_t *lcd, uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

// Funciones auxiliares privadas (declaradas como static)
static void _send(struct LiquidCrystal_t *lcd, uint8_t value, uint8_t mode);
static void _write4bits(struct LiquidCrystal_t *lcd, uint8_t value);
static void _pulseEnable(struct LiquidCrystal_t *lcd);
static void _digitalWrite(volatile uint8_t *port, uint8_t pin, uint8_t value);
static void _getPortAndPin(uint8_t pin, volatile uint8_t **port, uint8_t *pin_num);

#endif /* LIQUIDCRYSTAL_H */
