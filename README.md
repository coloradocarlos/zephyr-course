# Zephyr Training Environment

Welcome to the Zephyr RTOS training! This repository includes a ready-to-use
development environment based on Zephyr 4.3.0, which you can set up in one of
three ways:

---

## Manual Zephyr Setup

Follow the following guide:
- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).

Make sure to select appropriate OS and to perform all steps till
[Build the Blinky Sample](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#build-the-blinky-sample).

## Board setup

This builds for the Octavo OSD32MP1-BRK board. See https://docs.zephyrproject.org/latest/boards/oct/osd32mp1_brk/doc/osd32mp1_brk.html. To build:

### Octavo Systems OSD32MP1-BRK

```bash
$ west build --board osd32mp1_brk app -p
```

### Doorstep Technologies "My Little Board" (my-little-board) custom board

```bash
$ west build -b my_little_board app -p always
```

### Doorstep Technologies "My Better Board" (my-better-board) custom board

```bash
$ west build -b my_better_board app -p always
```

## Using the UART shell on the OSD32MP1

The Linux A7 console interface is assigned to dedicated pins on the board. This pins are labeled GND, RX, and TX. These are assigned to `uart4` on pins PB2 (UART4_RX) and PG11 (UART4_TX) which are aliased to `serial0` in the OSD32MP1 device tree. Reference the schematic at https://octavosystems.com/docs/osd32mp1-brk-schematics/

For using the Zephyr M4 UART shell, we need to allocate some of the breakout pins and not use the A7 default UART console on `uart4` to avoid conflicts. This requires using another serial port such as `uart7` even though it is shared with the A7. First, solder 3 header pins for GND, PA15 (UART7_TX), and PB3 (UART7_RX). Then use a second serial UART TTL cable to connect to these pins to open the console (default baud rate 115200 and 8N1). You should see a prompt using picocom or minicom:

```
uart:~$
uart:~$ kernel uptime -p
uptime: 0 days, 0 hours, 36 minutes, 26 seconds, 682 milliseconds
```

See https://octavosystems.com/wp-content/uploads/2025/05/Default-Pin-Mapping-1.pdf for the PCB and pin layout.

## Sensor root shell

To enable the sensor commands, set CONFIG_SENSOR=y in the prj.conf file.

- `sensor`: brief description
- `sensor fetch`: returns a single value (fetch)
- `sensor read`: returns a pair of values (get) along with a return code
- `sensor info`: prints the device name and ready state

Invoking `sensor fetch` and `sensor read` will toggle the green user LED.

## Custom extension root shell

To enable the custom extension commands, set CONFIG_SENSOR=n in the prj.conf file.

Note: while the command is named `sensor`, the driver implementation does not use the Zephyr sensor API. Instead, it uses the custom `doorstep_do_this` driver call.

- `sensor`: brief description
- `sensor set`: set values `foo` and `bar` in memory. At start up, the default values are 0.
- `sensor get`: returns the sum of `foo` and `bar` previously set

Invoking `sensor get` will toggle the green user LED.

Setting `foo` and `bar` to negative values is valid. It simulates an error condition and displays red letters on the shell console.

## Unit tests

To run the Twister test suites in the `modules` directory:

```bash
$ cd zephyr-course
$ west twister -T tests/calculator -p native_sim
$ west twister -T tests/ring_buf -p native_sim
```

## Change Log

| Date | Change |
| ---- | ------ |
| 27 MAR 26 | Lesson 2 - implement Blinky on the OSD32MP1. |
| 07 APR 26 | Lesson 3 - add LED config to Kconfig. |
| 21 APR 26 | Lesson 4 - add app devicetree to support both red and green LED's. |
| 26 APR 26 | Lesson 5 task 1 - Use copy and rename method to create "my_little_board" new board definition. This also builds the hello world sample instead of blinky. |
| 28 APR 26 | Lesson 5 task 2 - Use "from scratch" method to create "my_better_board" new board definition. This also prints "Board initialized" before entering the main entry point. |
| 04 MAY 26 | Lesson 6 task 1 - Add sensor_sample_fetch and sensor_channel_get to turn on and off the green LED. |
| 06 MAY 26 | Lesson 6 task 2 - Add custom API functions called doorstep_do_this() and doorstep_do_that() with custom parameters. See doorstep.h for API usage. |
| 11 MAY 26 | Lesson 7 task 1 - Enable uart shell and command `sensor`. To use the LED-based task 1 sensor API, set CONFIG_SENSOR=y. |
| 12 MAY 26 | Lesson 7 task 2 - Implement `sensor set` and `sensor get`. |
| 23 MAY 26 | Lesson 8 task 1 - Add Twister unit tests to exercise the `ring_buf` module. |
