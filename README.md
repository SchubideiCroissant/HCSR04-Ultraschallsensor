# HCSR04-Ultraschallsensor
 Abstandsmessung mit Sensor mithilfe eines Rasperry Pico W

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
