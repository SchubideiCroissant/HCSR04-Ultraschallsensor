#include "pico/stdlib.h"
#include <stdio.h>
#include <cmath>

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
const uint SPEAKER_PIN = 16;
const float CONTINUOUS_BEEP_THRESHOLD = 3.0;
const float MAX_DISTANCE = 70.0; // Distanz ab der Beep Töne kommen

void init_ultrasonic() {
    // Trig-Pin als Ausgang festlegen
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    
    // Echo-Pin als Eingang festlegen
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}
void init_speaker() {
    gpio_init(SPEAKER_PIN);
    gpio_set_dir(SPEAKER_PIN, GPIO_OUT);
    gpio_put(SPEAKER_PIN, false);
}

void play_sound(float distance, int wait_time){
    if(distance < MAX_DISTANCE)
    {
    gpio_put(SPEAKER_PIN, true);
    sleep_ms(50);  // Piepen für 100 ms (oder anpassbar)
    if(wait_time != 0) gpio_put(SPEAKER_PIN, false);
    // Wartezeit nach dem Piepen
    sleep_ms(wait_time);
    }
}

int calculate_wait_time(float distance) {
    // Definiere minimale und maximale Wartezeit
    const int min_wait_time = 5;   // Minimale Wartezeit z.B. 10 ms
    const int max_wait_time = 800; // Maximale Wartezeit z.B. 1000 ms

    // Wenn das Objekt extrem nah ist (z.B. < 3 cm), gib 0 zurück für durchgängigen Piepton
    if (distance <= CONTINUOUS_BEEP_THRESHOLD) {
        return 0; // Durchgehender Piepton
    }

    // Berechne die Wartezeit basierend auf der Distanz
    int wait_time = min_wait_time + (int)((max_wait_time - min_wait_time) * (distance / MAX_DISTANCE));

    // Sicherstellen, dass die Wartezeit innerhalb der definierten Grenzen bleibt
    if (wait_time < min_wait_time) wait_time = min_wait_time;
    if (wait_time > max_wait_time) wait_time = max_wait_time;

    return wait_time;
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
    stdio_init_all();
    printf("System gestartet!\n");
    init_ultrasonic();
    init_speaker();

    while (true) {
        float distance = measure_distance();
        printf("Entfernung: %.2f cm\n", distance);
        play_sound(distance, calculate_wait_time(distance));
        sleep_ms(100);  // kurze Wartezeit
    }
}
