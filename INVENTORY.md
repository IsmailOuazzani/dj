# Inventory

Components currently on hand for prototyping the DJ controller (see [SPEC.md](SPEC.md)). Build is iterative — we'll source new parts as each subsystem reaches the point of needing them, not all up front.

## On hand

| Item | Qty | Link | Notes |
|---|---|---|---|
| Breadboards | 2 | | Prototyping. |
| Jumper wires | Plenty | | Mixed M/M, M/F, F/F assumed. |
| LEDs | Plenty | | Through-hole, assorted. Useful for placeholder LED feedback on play/sync/FX before RGB pads arrive. |
| Resistors (assorted) | Plenty | | Assorted values. For LED current limiting and pull-ups/pull-downs. |
| Tactile buttons (kit) | Plenty | | 4-pin through-hole momentary switches. Stand-ins for every button during early prototyping. |
| Small blue potentiometers | 1–2 | | Hard end-stops (~270° travel). Cermet/trim-style stand-ins. Superseded on the panel by the PT01 pots below, kept as bench probes. |
| Arduino Nano RP2040 Connect | 1 | https://www.digikey.ca/en/products/detail/arduino/ABX00053/14552391 | Main MCU. Native USB-MIDI class-compliant. Pinout + project conventions: [hardware-docs/nano-rp2040-connect.md](hardware-docs/nano-rp2040-connect.md). |
| Adafruit 5346 MCP23017 STEMMA QT breakout | 1 | https://www.digikey.com/en/products/detail/adafruit-industries-llc/5346/15913270 | I²C GPIO expander at 0x20 (A0/A1/A2 straps un-bridged). Adds 16 GPIO for button scanning. STEMMA QT sockets on both edges. Delivered 2026-07-10. |
| SparkFun CD74HC4067 16-channel analog mux breakout (BOB-09056) | 1 | https://www.digikey.ca/en/products/result?keywords=1568-09056-ND | Analog input scaling — 16 mux channels into 1 ADC pin. Delivered 2026-07-10. |
| Same Sky PT01-D115D-B103 rotary pot (10 kΩ B-taper, 6 mm D-shaft) | 11 | https://www.digikey.ca/en/products/result?keywords=PT01-D115D-B103 | Panel-mount pots for EQ / cue / master / hp / FX-level positions. Linear taper, no detent — the SPEC-preferred detent-filter variant (PT01-D225D-B103) is not in this batch. 2 spares over v0 need of 9. Delivered 2026-07-10. |
| Same Sky PTNS2-6015M-B10 slider (60 mm, 10 kΩ B-taper) | 4 | https://www.digikey.ca/en/products/result?keywords=PTNS2-6015M-B10 | Tempo + channel-volume sliders. ⚠ **2.5 mm metric pin pitch — will NOT drop into 0.1" breadboard cleanly**. Solder hookup-wire pigtails before mounting. Delivered 2026-07-10. |
| E-Switch TL1105SPF160Q tactile (6 × 6 mm, 160 gf, 100k cycles) | 25 | https://www.digikey.ca/en/products/detail/e-switch/TL1105SPF160Q/271558 | Panel-mount tactiles for play, sync, cue, PFL, load, shift, loop in/out/exit, FX on/off, hotcues-during-v0. 1RBLK cap-compatible. 20 needed for v0 subset, 5 spares. Delivered 2026-07-10. |
| Bourns PEC11R-4215F-S0024 encoder (24 PPR, 24 detents, w/ push) | 3 | https://www.digikey.ca/en/products/detail/bourns-inc/PEC11R-4215F-S0024/4499665 | For beat-loop A/B + browse (or as jog-wheel proxy in v0). 6 mm flatted shaft. Delivered 2026-07-10. |
| Adafruit 4209 STEMMA QT to male-header jumper (JST-SH 4-pin, 100 mm) | 1 | https://www.digikey.ca/en/products/detail/adafruit-industries-llc/4209/10230003 | Connects MCP23017 STEMMA socket to Nano I²C on breadboard (red→3V3, black→GND, blue→A4/SDA, yellow→A5/SCL). Delivered 2026-07-10. |
| Stackpole CF14JT4K70 4.7 kΩ resistor (1/4 W, 5%, axial) | 2 | https://www.digikey.ca/en/products/detail/stackpole-electronics-inc/CF14JT4K70/1741428 | Reserve for I²C pull-ups if bus rise time gets ugly on scope. Nano onboard 4.7 kΩ + 5346 onboard 10 kΩ already pull the rail; default is do-not-populate. Delivered 2026-07-10. |
| Kemet C320C104K5R5TA 0.1 µF ceramic bypass cap (X7R 50 V, 2.54 mm lead pitch) | 1 | https://www.digikey.ca/en/products/detail/kemet/C320C104K5R5TA/818040 | Local decoupling — place across 3V3/GND at CD4067 VCC pin. Delivered 2026-07-10. |
| E-Switch 1RBLK tactile button cap (round black push-on) | 1 | https://www.digikey.ca/en/products/detail/e-switch/1RBLK/271579 | Fitment validation for TL1105SPF160Q. Delivered 2026-07-10. |
| Cliff CL170822BR knob (soft-touch, 6 mm D-shaft, push-fit + set screw) | 1 | https://www.digikey.ca/en/products/result?keywords=4654-CL170822BR-ND | Fitment validation for PT01 6 mm D-shaft. Delivered 2026-07-10. |
| Sullins PRPC040SAAN-RC 40-pin snappable header (2.54 mm, tin) | 1 | https://www.digikey.ca/en/products/detail/sullins-connector-solutions/PRPC040SAAN-RC/2775214 | Snap into sticks for BOB-09056 (24 pins) and Adafruit 5346 (if header-less from Digikey). 16 pins spare after both breakouts. Delivered 2026-07-10. |

## Philosophy

Prototype each subsystem on the breadboard before committing to a part for the final build. Order in which we'll likely need to source things (rough — confirmed as we go):

1. **Phase 1 — Input prototyping (covered):** button reading (kit tactiles + TL1105), pot reading (PT01 + trim pots), single LED feedback, USB-MIDI enumeration on the RP2040.
2. **Phase 2 — Encoders & jog (partly covered):** have 3 × PEC11R for browse / beat-loop and v0 jog proxy. Still need 2 × AS5600 + diametric magnets for real jog wheels, and 1 no-push PEC11R for FX-select.
3. **Phase 3 — Scaling I/O (covered for v0):** CD4067 mux + MCP23017 expander on hand. Full v1 needs 2 more MCP23017 breakouts (3 total) for all 27 buttons.
4. **Phase 4 — Real controls:** long-throw pitch faders (100 mm PTL01), crossfader, full-size panel-mount pots with knob caps, RGB pads (NeoKey 1×4 modules + MX switches).
5. **Phase 5 — Enclosure & panel:** laser-cut / 3D-printed top panel, mounting hardware, final wiring harness.

Each phase's exact BOM gets pinned down as it starts — kept here so we don't pre-buy parts that turn out to be the wrong choice.

## Gaps to track

Things SPEC.md / BOM.md require that we don't yet have. Don't buy until the relevant phase starts.

- 1 more no-push rotary encoder for FX-select (`PEC11R-4215F-N0024-ND` https://www.digikey.ca/en/products/result?keywords=PEC11R-4215F-N0024-ND).
- 2 × AS5600 magnetic encoder breakouts for jog wheels (`1528-6357-ND` https://www.digikey.ca/en/products/detail/adafruit-industries-llc/6357/26832926) + 2 × diametric magnets (`469-1024-ND` https://www.digikey.ca/en/products/detail/radial-magnets-inc/8995/5126077).
- 2 × 100 mm long linear pitch faders (`PTL01-15R0-103B1-ND` https://www.digikey.ca/en/products/detail/bourns-inc/PTL01-15R0-103B1/6605956) — the 60 mm PTNS2 sliders on hand are usable for channel-volume + tempo v0 mock, not for the SPEC's 60–100 mm pitch faders in v1.
- 1 × crossfader (`118-PTL45-15O0-103B2-ND` https://www.digikey.ca/en/products/detail/bourns-inc/PTL45-15O0-103B2/16356348) plus 2 channel faders (same SKU).
- 1 more panel pot for the full 12-pot v1 layout (PT01 batch on hand is 11, need 12 total). Also: 2 of those 12 should be the center-detent filter variant `2223-PT01-D225D-B103-ND` https://www.digikey.ca/en/products/result?keywords=PT01-D225D-B103 — we have 0 detent pots.
- 8 RGB MX-style hotcue keys: 2 × NeoKey 1×4 modules (`1528-4980-ND` https://www.digikey.ca/en/products/detail/adafruit-industries-llc/4980/14319123) + 8 × Cherry MX2A-G1NB switches (`1644-MX2A-G1NB-ND` https://www.digikey.ca/en/products/detail/cherry-americas-llc/MX2A-G1NB/21738375) + Adafruit relegendable keycap pack (`1528-5039-ND` https://www.digikey.ca/en/products/result?keywords=1528-5039-ND).
- 2 more MCP23017 breakouts for the full 27-button v1 (need 3 total).
- 2 more 4.7 kΩ resistors + 2 more 0.1 µF caps for the second I²C bus + per-MCP decoupling.
- 15 more Cliff CL170822BR knobs (need 16 total for pots + non-jog encoders).
- 18 more 1RBLK button caps (need 19 total for tactiles).
- USB-B (or USB-C) panel-mount jack + cable for the enclosure.
- Enclosure material.
