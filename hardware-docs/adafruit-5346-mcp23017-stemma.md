# Adafruit 5346 — MCP23017 STEMMA QT breakout

Product page: https://www.adafruit.com/product/5346
Digikey CA: https://www.digikey.ca/en/products/detail/adafruit-industries-llc/5346/15913270
Learn guide: https://learn.adafruit.com/adafruit-mcp23017-i2c-gpio-expander

## GPIO silkscreen mapping

The two long edges of the PCB expose all 16 GPIOs as through-hole pads.
Labels printed on the board and the chip-native pin they connect to:

| Silkscreen | Chip pin | Library index (`Adafruit_MCP23X17`) |
|:-:|:-:|:-:|
| A0 | GPA0 | 0 |
| A1 | GPA1 | 1 |
| A2 | GPA2 | 2 |
| A3 | GPA3 | 3 |
| A4 | GPA4 | 4 |
| A5 | GPA5 | 5 |
| A6 | GPA6 | 6 |
| A7 | GPA7 | 7 |
| B0 | GPB0 | 8 |
| B1 | GPB1 | 9 |
| B2 | GPB2 | 10 |
| B3 | GPB3 | 11 |
| B4 | GPB4 | 12 |
| B5 | GPB5 | 13 |
| B6 | GPB6 | 14 |
| B7 | GPB7 | 15 |

Use the silkscreen name in wiring instructions; use the library index in
firmware calls (`mcp.pinMode(3, INPUT_PULLUP)` = pad A3 = GPA3).

## I²C address

Default address is `0x20`. Solderable jumpers `D0` / `D1` / `D2` (on the
top face, near the chip) each add 1/2/4 to the base address, giving
`0x20` – `0x27`. Bridge to VIN to set. Ships un-bridged.

(The chip's die-native address pins are named A0/A1/A2 in the datasheet;
Adafruit renamed the silkscreen pads to D0/D1/D2 to avoid collision with
the GPIO pads A0–A7 on the edge.)

## Other pads

- `IA` / `IB` — interrupt outputs for PORTA / PORTB (INTA / INTB in the
  datasheet). Not general-purpose GPIO; only useful for interrupt-driven
  reads. Ignore for polled scanning.
- `RST` — active-low reset. Tie high or leave floating (has internal
  pull-up) for normal use.
- `VIN` / `3V` / `GND` / `SDA` / `SCL` — power + I²C bus, mirrored on
  both STEMMA QT connectors.
