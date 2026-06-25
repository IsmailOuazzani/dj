# Arduino Nano RP2040 Connect

Primary MCU for prototyping the controller (see [../INVENTORY.md](../INVENTORY.md)).

## Official references

Bare URLs — triple-click to copy.

- Product / docs hub: https://docs.arduino.cc/hardware/nano-rp2040-connect/
- Full pinout PDF: https://docs.arduino.cc/resources/pinouts/ABX00053-full-pinout.pdf
- Datasheet PDF: https://docs.arduino.cc/resources/datasheets/ABX00053-datasheet.pdf
- Schematic PDF: https://docs.arduino.cc/resources/schematics/ABX00053-schematics.pdf
- Digikey product page (CAD): https://www.digikey.ca/en/products/detail/arduino/ABX00053/14552391
- RP2040 datasheet (Raspberry Pi): https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf

## Board basics

- **MCU**: RP2040 — dual-core ARM Cortex-M0+ @ 133 MHz, 264 KB SRAM, 16 MB QSPI flash on-board (AT25SF128A).
- **USB**: Micro-USB connector. Native USB → class-compliant USB-MIDI is possible (TinyUSB / Adafruit_TinyUSB / Arduino MIDIUSB).
- **Logic voltage**: **3.3 V**. ⚠️ GPIOs are **NOT 5 V tolerant** — max GPIO input is 3.3 V. Max I/O supply 3.63 V. Max total sink current across all GPIO+QSPI pins is 50 mA.
- **Onboard peripherals**: NINA-W102 (WiFi + BT), LSM6DSOXTR 6-axis IMU, MP34DT06JTR PDM mic, ATECC608A crypto chip, RGB LED (driven via the NINA module).
- **`LED_BUILTIN`**: routed through the NINA-W102, not a direct RP2040 GPIO. `digitalWrite(LED_BUILTIN, …)` still works.

## Analog inputs

The RP2040 ADC has 4 channels. Only **A0–A3** are real analog inputs on this board.

| Arduino label | RP2040 GPIO | Analog? | Notes |
|---|---|---|---|
| A0 | GPIO26 / ADC0 | ✅ true ADC | First choice for pots/faders. |
| A1 | GPIO27 / ADC1 | ✅ true ADC | |
| A2 | GPIO28 / ADC2 | ✅ true ADC | |
| A3 | GPIO29 / ADC3 | ✅ true ADC | |
| A4 | GPIO12 | ❌ digital only | Default I²C0 SDA — shared with onboard IMU + mic. |
| A5 | GPIO13 | ❌ digital only | Default I²C0 SCL — shared with onboard IMU + mic. |
| A6 | GPIO25 | ❌ digital only | |
| A7 | QSPI_CSn (internal) | ❌ reserved | Don't use. |

- **ADC resolution**: 12-bit native (0–4095). Arduino's `analogRead()` defaults to **10-bit (0–1023)** — call `analogReadResolution(12)` to get full resolution.
- **ADC reference**: 3.3 V. The `AREF` pin is tied internally; no external reference circuit is provided on this board.
- **Implication for SPEC.md**: with only 4 native ADC channels and 17 analog signals required (12 pots + 5 faders), an analog mux (CD4067 already picked in BOM) is mandatory once we scale beyond prototype.

## Digital pins

| Arduino label | RP2040 GPIO | Default peripheral |
|---|---|---|
| D0 / RX | GPIO0 | UART0 RX |
| D1 / TX | GPIO1 | UART0 TX |
| D2 | GPIO25 | — |
| D3 | GPIO15 | PWM7 B |
| D4 | GPIO16 | I²C0 SDA alt |
| D5 | GPIO17 | I²C0 SCL alt |
| D6 | GPIO18 | SPI0 SCK alt |
| D7 | GPIO19 | SPI0 TX alt |
| D8 | GPIO20 | |
| D9 | GPIO21 | |
| D10 | GPIO5 | SPI0 CSn (default) |
| D11 / COPI | GPIO7 | SPI0 TX (MOSI) |
| D12 / CIPO | GPIO4 | SPI0 RX (MISO) |
| D13 / SCK | GPIO6 | SPI0 SCK (also routes to LED_BUILTIN via NINA) |

## Default bus pin assignments

- **I²C (Wire)**: SDA = A4 / GPIO12, SCL = A5 / GPIO13 — **shared with onboard IMU + mic**.
- **SPI**: SCK = D13 / GPIO6, COPI = D11 / GPIO7, CIPO = D12 / GPIO4, CS = D10 / GPIO5.
- **UART (Serial1)**: TX = D1 / GPIO0, RX = D0 / GPIO1. (USB CDC `Serial` is separate.)

## Power pins

- **VIN**: 5–21 V external supply input.
- **+5V**: 5 V from USB. Only exposes voltage on the header if solder jumper **SJ1** on the underside is shorted (default is open).
- **+3V3**: regulated 3.3 V output. Use this as the reference rail for pots, mux V<sub>CC</sub>, pull-ups, etc.
- **GND**: ×2.
- **REC**: boot/recovery (RP2040 QSPI_CSn). Not a general-purpose pin.
- **AREF**: tied to 3.3 V — don't drive it.

## Reserved / shared pins — leave alone

- **GPIO12 / GPIO13** (A4 / A5): I²C bus to onboard **LSM6DSOXTR** IMU and **MP34DT06JTR** mic. Safe to share if used as I²C; risky as plain GPIO.
- **GPIO22 / GPIO23**: onboard PDM mic (CLK / DOUT). Not broken out.
- **QSPI bus** (QSPI_SCK / SD0–3 / CSn): onboard 16 MB flash. Off-limits.
- **NINA-W102 SPI/UART**: WiFi/BT module — irrelevant for v1 since the controller is USB-only.

## v1 wiring conventions for this project

- Pot/fader wiper → **A0–A3** (one of the 4 true ADC pins). Outer pins to **+3V3** and **GND** — never +5V.
- 12-bit ADC (`analogReadResolution(12)`) for smoother fader/pot response.
- Once analog signals exceed 4, route them through the CD4067 mux; the mux's 4 select lines and 1 signal line will eat 5 GPIO + 1 ADC channel total.
- For LED/button I/O scaling: MCP23017 over I²C (A4/A5) — already on the IMU/mic bus, so add address pull-ups carefully.

## Toolchain notes

- PlatformIO board ID: `nanorp2040connect` (board: `nanorp2040connect`, platform: `raspberrypi`, framework: `arduino`).
- Bootloader mode: double-tap RESET to enter UF2 mass-storage mode for manual flashing.
- USB-MIDI: Adafruit TinyUSB stack works on this board — preferred over MIDIUSB because the latter has gaps on the RP2040 core.
