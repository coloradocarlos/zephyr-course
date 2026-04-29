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

### Doorstep Technologies "My Little Board" (my-little-board)

```bash
$ west build -b my_little_board app -p always
```

### Doorstep Technologies "My Better Board" (my-better-board)

```bash
$ west build -b my_better_board app -p always
```

## Change Log

| Date | Change |
| ---- | ------ |
| 27 MAR 26 | Lesson 2 - implement Blinky on the OSD32MP1. |
| 07 APR 26 | Lesson 3 - add LED config to Kconfig. |
| 21 APR 26 | Lesson 4 - add app devicetree to support both red and green LED's |
| 26 APR 26 | Lesson 5 task 1 - Use copy and rename method to create "my_little_board" new board definition. This also builds the hello world sample instead of blinky. |
 28 APR 26 | Lesson 5 task 2 - Use "from scratch" method to create "my_better_board" new board definition. This also prints "Board initialized" before entering the main entry point. |
