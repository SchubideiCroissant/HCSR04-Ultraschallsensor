# Abstandsmessung mit Raspberry Pi Pico W und Parksensor

Dieses Projekt verwendet einen Ultraschallsensor (HC-SR04) und einen Buzzer, um einen Parksensor zu erstellen. Der Raspberry Pi Pico W misst die Entfernung eines Objekts mithilfe des Ultraschallsensors und gibt entsprechende Pieptöne über den Buzzer aus. Die Pieptöne werden schneller, je näher das Objekt am Sensor ist.
Der Buzzer ist optional, es wird über den Seriellen Monitor auch die gemessene Entfernung vom Sensor ausgegeben.

## Komponenten

- **Raspberry Pi Pico W**: Mikrocontroller für die Steuerung.
- **HC-SR04 Ultraschallsensor**: Misst die Entfernung des Objekts.
- **Aktiver Buzzer**: Gibt akustische Signale aus, um auf die Distanz zu reagieren.
- **Widerstände**: Zur Anpassung der Spannung des Echo-Pins (5V auf 3.3V).

## Schaltplan

- **HC-SR04**:
  - `VCC` -> 5V (oder 3.3V, wenn es stabil funktioniert)
  - `GND` -> GND
  - `Trig` -> GPIO 0 (zum Auslösen des Sensors)
  - `Echo` -> GPIO 1 (zum Empfangen des Echos, durch Spannungsteiler auf 3.3V reduzieren)
  
- **Buzzer**:
  - `S` -> GPIO 16
  - `mittlerere Anschluss` -> VCC
  - `-` -> GND

- **Spannungsteiler** für den Echo-Pin:
  - Widerstand 1: 1kΩ zwischen Echo-Pin und GPIO 1.
  - Widerstand 2: 2kΩ zwischen GPIO 1 und GND.
  - 
## Schaltplan
```

 HC-SR04            Raspberry Pi Pico
+---------+          +-----------------+
|         |          |                 |
|  VCC    |----------| 3.3V            |
|         |          |                 |
|  TRIG   |----------| GPIO 0 (TRIG)   |
|         |          |                 |
|  ECHO   |--+[1k]---| GPIO 1 (ECHO)   |
|         |  |       |                 |
|         |  |       |                 |
|         | [2k]     |                 |
|         |  |       |                 |
|         |  |       |                 |
|  GND    |--+-------| GND             |
|         |          |                 |
+---------+          +-----------------+

```
