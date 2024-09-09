#include "pico/stdlib.h"
#include <stdio.h>

/*
 * measure_distance() misst die Entfernung mithilfe des HC-SR04 Ultraschallsensors.
 * Der Trig-Pin sempfängt einen 10 Mikrosekunden langen Impuls damit der Sensor Ultraschallwellen aussendet.
 * Der Echo-Pin Geht HIGH, wenn der Sensor das zurückkommende Echo empfängt. Der Echo-Pin bleibt HIGH, solange das Echo empfangen wird und geht anschließend wieder auf LOW.
 * Die Entfernung wird durch die Schallgeschwindigkeit (343 m/s) und die Antwortzeit des Echo-Pins berechnet berechnet.
 *
 * Wichtig: Da der Echo-Pin des HC-SR04 eine Spannung von 5V ausgibt, während der Raspberry Pi Pico
 * nur 3.3V toleriert, muss ein Spannungsteiler verwendet werden. Ein Spannungsteiler ist eine einfache
 * Schaltung, die zwei Widerstände nutzt, um die Spannung zu reduzieren.
 *
 * Schaltung des Spannungsteilers:
 * - Ein Widerstand \(R_1\) von 1kΩ wird zwischen den Echo-Pin und den Eingangspin des Raspberry Pi Pico
 *   geschaltet.
 * - Ein zweiter Widerstand \(R_2\) von 2kΩ wird zwischen den Eingangspin des Raspberry Pi Pico und Ground (GND)
 *   geschaltet.
 * - Diese Widerstände teilen die Spannung von 5V so, dass der Pico nur etwa 3.3V empfängt.
 *
 * Spannungsteilungsformel:
 * V_out = V_in * (R2 / (R1 + R2))
 * In unserem Fall: V_out = 5V * (2kΩ / (1kΩ + 2kΩ)) = 5V * (2/3) = 3.33V
 *
 * Dadurch wird der Raspberry Pi Pico vor zu hoher Spannung geschützt, während der Echo-Wert korrekt
 * ausgelesen werden kann.
 */

// GPIO-Pins für den HC-SR04
const uint TRIG_PIN = 0;
const uint ECHO_PIN = 1;

void init_ultrasonic() {
    // Trig-Pin als Ausgang festlegen
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    
    // Echo-Pin als Eingang festlegen
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

float measure_distance() {
    // Triggersignal senden (10 µs HIGH)
    gpio_put(TRIG_PIN, 0);
    sleep_us(2);
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Warten, bis der Echo-Pin auf HIGH wechselt
    while (gpio_get(ECHO_PIN) == 0) {
        tight_loop_contents();  // Warten 
    }

    // Zeit messen, wie lange Echo-Pin HIGH ist
    uint64_t start_time = time_us_64();
    while (gpio_get(ECHO_PIN) == 1) {
        tight_loop_contents();  // Warten
    }
    uint64_t end_time = time_us_64();

    // Zeitunterschied berechnen (Dauer des Echos)
    uint64_t duration = end_time - start_time;

    // Umwandeln der Zeit in Entfernung (Schallgeschwindigkeit: 343 m/s)
    float distance = (duration * 0.0343) / 2;  // Zeit * Schallgeschwindigkeit / 2 , s = v * t

    return distance;
}

int main() {
    //stdio_init_all();
    stdio_usb_init();
    printf("System gestartet!\n");
    init_ultrasonic();

    while (true) {
        float distance = measure_distance();
        printf("Entfernung: %.2f cm\n", distance);
        sleep_ms(1000);  // Eine Sekunde warten
    }
}
