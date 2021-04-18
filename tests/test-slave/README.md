# Firmware

This is the firmware for the controler-baord in the folder [controller_slave](./../controller_slave).

## configuration

Configuration is done in the file [config.h](./config.h)

## Parameters for compiling

The software was compiled in the Arduino-IDE. Therefore in the Board-Mannager the **Attiny85** was added. These are the parameters:

| Parameter | Value |
|:---:|:---:|
| Board | Attiny 25/45/85 (no Bootloader) |
| Chip | Attiny85 |
| Clock | 8 MHz (internal) |
| B.O.D. Level | 4,3V |
| Save EEPROM | EEPROM retained |
| Timer 1 Clock | CPU (CPU frequency) |
| LTO | enabled |
| `millis()` / `micros()` | enabled |


## To-Do list

- [x] Configuration via config-file
- [x] Implement Buffer
- [x] Interface for Shift-Registers
- [x] Interface for Timing
- [x] softwareSerial interface
- [ ] test everything
- [ ] remove bugs
- [ ] check for buffer overflow (Serial)
- [ ] check for buffer overflow (FIFO)
- [ ] possibility to group signals to prevent buffer overflow
- [ ] Possibilit to adjust configuration while the program runs
- [ ] Store Configuration in EEPROM (as a result of the previous note)