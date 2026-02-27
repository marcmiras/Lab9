# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Embedded C project for the **PIC18F4321 microcontroller** (Microchip). An educational lab assignment focused on interrupt-driven serial I/O, timer management, and GPIO control. Comments and identifiers are in Catalan/Spanish.

Target: PIC18F4321 @ 10MHz (HS oscillator), compiled with MPLAB XC8.

## Build Commands

```bash
make build     # compile default configuration → dist/default/production/Lab9.X.production.hex
make clean     # remove built files for current configuration
make clobber   # remove all built files
make all       # build all configurations
```

The Makefile delegates to `nbproject/Makefile-impl.mk`. Build artifacts (`.p1`, `.d`, `.o`) go under `build/default/production/`; the final `.hex` goes to `dist/default/production/`.

There is no test infrastructure — testing is done on physical hardware.

## Architecture

The project uses a **TAD (Tipus Abstracte de Dades / Abstract Data Type)** module pattern. Each module has a `.h` interface and a `.c` implementation:

| Module | Files | Role |
|---|---|---|
| Main app | `main.c` | Initializes all modules; high-priority ISR (`LaRSI`) calls `TI_RSITimer0()`; main loop polls buttons and updates 7-segment display/LEDs |
| Timer | `TAD_Timer.h/.c` | Virtual timer multiplexer on TMR0 — `TI_Init`, `TI_NewTimer`, `TI_GetTics`, `TI_ResetTics`, `TI_RSITimer0`, `TI_End` |
| Button input | `TAD_Pols0.h/.c` | Debounced button polling — `POLS0_Init`, `POLS0_Motor` (call from main loop), `POLS0_HiHaPulsacio`, `POLS0_End` |
| Output | `TAD_Out.h/.c` | LED (RA3=LED1, RA4=LED0) and 7-segment display (LATD) control — `OUT_Init`, `OUT_7SegmentsPinta`, `OUT_EncenLed`, `OUT_ApagaLed` |
| Serial I/O | `TAD_SIO.h/.c` | UART at 9600 baud, RX/TX circular buffers (32 chars each) — `SIO_Init`, `SIO_InterrupcioRX`, `SIO_InterrupcioTX`, `SIO_RXAvail`, `SIO_GetChar`, `SIO_TXAvail`, `SIO_PutChar`, `SIO_PutString`, `SIO_End` |

## Implementation Status

All modules are fully implemented.

**`main.c` is complete** and calls all module functions listed above.
