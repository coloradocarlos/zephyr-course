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

```bash
$ west build --board osd32mp1_brk app -p
```

## Change Log

| Date | Change |
| ---- | ------ |
| 27 MAR 26 | Lesson 2 - implement Blinky on the OSD32MP1. |
