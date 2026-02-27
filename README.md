# Lab 9.X — Interrupt-Driven Serial I/O (PIC18F4321)

Embedded C project for the **PIC18F4321** microcontroller. Implements interrupt-driven UART communication with circular buffers, a virtual timer multiplexer, debounced button input, and 7-segment/LED output. Written for the MPLAB XC8 compiler.

## Hardware

| Parameter | Value |
|---|---|
| MCU | PIC18F4321 |
| Clock | 10 MHz (HS oscillator) |
| UART baud rate | 9600, 8N1 |
| Button | RB0 (active-low, internal pull-up) |
| 7-segment display | PORTD (common-cathode) |
| LED0 | RA4 |
| LED1 | RA3 |
| TX pin | RC6 |
| RX pin | RC7 |

## How to Build and Flash

### Prerequisites

- [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [XC8 Compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers)
- GNU `make`

### Build

```bash
make build      # compile → dist/default/production/Lab9.X.production.hex
make clean      # remove build artifacts for the current configuration
make clobber    # remove all build artifacts
make all        # build all configurations
```

The Makefile delegates to `nbproject/Makefile-impl.mk`. Object files go under `build/default/production/`; the final `.hex` goes to `dist/default/production/`.

### Flash

Use MPLAB X with a PICkit or ICD programmer, or any compatible tool (e.g. `pk2cmd`, `ipecmd`) targeting the `.hex` file produced above.

There is no automated test infrastructure — verification is done on physical hardware.

## Project Structure

```
Lab9.X/
├── main.c          # Entry point, ISR, main loop
├── TAD_Timer.h/c   # Virtual timer multiplexer (TMR0)
├── TAD_Pols0.h/c   # Debounced button polling
├── TAD_Out.h/c     # LED and 7-segment display output
└── TAD_SIO.h/c     # UART serial I/O with circular buffers
```

## Module Overview

### `main.c`

- Configures the PIC (HS oscillator, WDT off, PORTB digital).
- Initialises all modules and enables global interrupts.
- On startup sends `"\r\nHello world!!\r\n"` over UART.
- **High-priority ISR** (`LaRSI`) dispatches to:
  - `SIO_InterrupcioRX()` on UART RX interrupt
  - `SIO_InterrupcioTX()` on UART TX interrupt
  - `RSI_Timer0()` on TMR0 overflow
- **Main loop** (polling):
  1. Calls `POLS0_Motor()` to advance the button state machine.
  2. If bytes are in the RX buffer, shows the count on the 7-segment display.
  3. If the button is pressed **and** the RX buffer has data, reads one character from the buffer, updates the display, and echoes the character back over UART.

### `TAD_Timer` — Virtual Timer Multiplexer

TMR0 is configured for a **2 ms tick** (reload value 64911 at 10 MHz). Up to **4 independent virtual timers** can be allocated. Each timer records a snapshot of the global tick counter; elapsed time is the difference from that snapshot.

| Function | Description |
|---|---|
| `TI_Init()` | Configures TMR0, enables TMR0 interrupt |
| `TI_NewTimer(&handle)` | Allocates a virtual timer, returns `TI_CERT`/`TI_FALS` |
| `TI_ResetTics(handle)` | Captures the current tick as the start reference |
| `TI_GetTics(handle)` | Returns ticks elapsed since last reset |
| `TI_End()` | Destructor (no-op) |
| `RSI_Timer0()` | Called from ISR on TMR0 overflow — reloads timer, increments counter |

### `TAD_Pols0` — Debounced Button

Implements a two-state machine (REPOS / PREMUT) on **RB0** (active-low with internal pull-up). `POLS0_Motor()` must be called from the main loop on every iteration.

| Function | Description |
|---|---|
| `POLS0_Init()` | Configures RB0 as input, enables PORTB pull-ups |
| `POLS0_Motor()` | Advances the state machine (call every loop iteration) |
| `POLS0_HiHaPulsacio()` | Returns `CERT` once per press, then clears the flag |
| `POLS0_End()` | Destructor (no-op) |

### `TAD_Out` — LED and 7-Segment Display

Controls a common-cathode 7-segment display on PORTD and two LEDs on PORTA.

| Function | Description |
|---|---|
| `OUT_Init()` | Sets PORTD, RA3, RA4 as outputs |
| `OUT_7SegmentsPinta(value)` | Displays digits 0–9 on the 7-segment display |
| `OUT_EncenLed(led)` | Turns on `LED0` (RA4) or `LED1` (RA3) |
| `OUT_ApagaLed(led)` | Turns off `LED0` or `LED1` |

7-segment encoding (common-cathode, bit positions `g f e d c b a`):

```
 _
|_|
|_|
```

### `TAD_SIO` — UART Serial I/O

Asynchronous UART at **9600 baud** (SPBRG=64, BRGH=1, 8-bit, no parity). Both RX and TX use **32-byte circular buffers** managed by the ISR.

| Function | Description |
|---|---|
| `SIO_Init()` | Configures USART registers, enables RX interrupt |
| `SIO_InterrupcioRX()` | ISR handler — reads RCREG into the RX circular buffer |
| `SIO_InterrupcioTX()` | ISR handler — sends next byte from TX buffer, disables TX interrupt when empty |
| `SIO_RXAvail()` | Returns number of bytes waiting in the RX buffer |
| `SIO_GetChar()` | Reads and removes one byte from the RX buffer (disables interrupts briefly) |
| `SIO_TXAvail()` | Returns free space in the TX buffer |
| `SIO_PutChar(c)` | Sends a byte (directly if TXIF is set, otherwise via TX buffer) |
| `SIO_PutString(str)` | Sends a null-terminated string via `SIO_PutChar` |
| `SIO_End()` | Destructor (no-op) |

## Runtime Behaviour Summary

```
Power-on
  │
  ├─ Init: Timer, Button, UART, Output
  ├─ TX: "\r\nHello world!!\r\n"
  │
  └─ Loop forever:
       ├─ Button motor tick
       ├─ RX bytes available? → show count on 7-segment display
       └─ Button pressed AND RX data?
            ├─ Read one char from RX buffer
            ├─ Update 7-segment display (new count)
            └─ Echo char back over UART
```
