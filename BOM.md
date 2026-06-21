# Bill of Materials

Component selections for v1. These are **not** part of [SPEC.md](SPEC.md) — they're the current best concrete choices given the breadboard-buildable constraint, hobbyist budget, and Digikey availability. Parts can change without changing the spec, as long as the new pick still satisfies the spec's requirements.

Each subsystem lists the chosen part, supplier, approximate cost, and a one-line rationale. "TBD" means we haven't picked yet (see open decisions at the bottom).

---

## MCU

| Part | Qty | Source | Cost | Rationale |
|---|---|---|---|---|
| Arduino Nano RP2040 Connect | 1 | On hand | — | Native USB-MIDI capable, dual-core Cortex-M0+ @ 133 MHz with PIO for jog quadrature decoding + WS2812 driving without burning CPU. |

## Buttons — digital input scaling

| Part | Qty | Source | Cost | Rationale |
|---|---|---|---|---|
| Microchip MCP23017-E/SP (DIP-28, I²C 16-bit GPIO expander) | 3 | Digikey `MCP23017-E/SP-ND` | $1.40 ea / $4.20 | 3 chips = 48 GPIO, covers 35 buttons with headroom. Internal pull-ups (no 35 external resistors). Interrupt-on-change pin → MCU only reads on actual button change. 2 pins total to the MCU (SDA, SCL). Bus can address up to 8 chips = 128 GPIO future headroom. |
| 0.1 µF ceramic bypass caps | 3 | Digikey `BC1148CT-ND` | $0.81 | One per chip. |
| 4.7 kΩ I²C pull-up resistors | 2 | Digikey `CFR-25JB-52-4K7CT-ND` | $0.20 | SDA, SCL pull-ups. |
| **Subtotal** | | | **~$5.21** | |

## Pots & faders — analog input scaling

| Part | Qty | Source | Cost | Rationale |
|---|---|---|---|---|
| SparkFun CD74HC4067 16-channel analog mux breakout (BOB-09056) | 1 | Digikey `1568-09056-ND` | $6.50 | One chip covers all 17 analog signals (16 via mux + 1 direct ADC for the 17th). Breadboard-friendly headers. CD4067 DIP is EOL, this is the cleanest breadboard-compatible path. |
| **Subtotal** | | | **~$6.50** | |

## Pots (the actual pots and faders)

| Subsystem | Qty | Pick | Notes |
|---|---|---|---|
| EQ + master + headphone + FX level pots | 10 | TBD — leaning Same Sky `PT01-D115D-B103` (10k linear, ~$0.79 ea on Digikey) | Through-hole panel-mount, B-taper (linear), adequate for non-critical pots. |
| Filter pots (center-detent) | 2 | TBD — leaning TT Electronics `P160KNPD-4FC20B10K` (~$1.98 ea on Digikey) | "PD" suffix = center detent, matches filter HP/LP centered behavior. |

## Faders

| Subsystem | Qty | Pick | Notes |
|---|---|---|---|
| Pitch faders | 2 | **Open decision** | 60mm Bourns PTL60 ($2.20 ea, Digikey) vs. 100mm ALPS RSA0N (NOS, ~$15–25 ea, off-platform). |
| Channel faders | 2 | TBD — likely Bourns `PTL45-15O0-104A3` (~$2.14 ea on Digikey) | 45 mm linear, conductive plastic. |
| Crossfader | 1 | **Open decision** | Cheap Bourns PTL45 ($2.57, Digikey) vs. Mini Innofader Pro ($60+, off-platform, contactless). |

## Encoders

| Subsystem | Qty | Pick | Notes |
|---|---|---|---|
| Beat-loop + browse (w/ push) | 3 | TBD — likely Bourns `PEC11R-4215F-S0024` (24 PPR w/ push, ~$2.38 ea on Digikey) | Mechanical, breadboard-friendly. |
| FX select (no push) | 1 | TBD — likely Bourns `PEC11R-4015F-S0024` (24 PPR no push, ~$2.30 ea) | |
| Jog wheels | 2 | **Open decision** | 24 PPR mechanical PEC11R ($2.27 ea, coarse but Digikey-stocked) vs. AS5600 magnetic modules ($3–5 ea, AliExpress, 4096-step). |

## Hotcue pads (RGB)

**Open decision.** Two paths under consideration — see decision below.

| Option | Cost | Notes |
|---|---|---|
| 2× Adafruit NeoTrellis 4×4 (PID 3954) | ~$34 | Pre-built PCB: 16 RGB LEDs + silicone-bumper buttons + I²C driver. No need for 16 separate switches *or* 16 individual NeoPixels. Breadboard-friendly via header pins. |
| 16× Adafruit NeoPixel 5mm through-hole (PID 1938) + 16 panel switches | ~$20 LEDs + switches counted under Misc Switches | Maximum layout freedom; significantly more soldering. |

## Misc switches (non-hotcue buttons)

| Part | Qty | Source | Cost | Rationale |
|---|---|---|---|---|
| E-Switch TL1100F160Q tactile (through-hole, 6mm) | 19 | Digikey `TL1100F160Q` | $0.42 ea / $7.98 | Standard through-hole tactile for play, sync, cue, PFL, load, shift, loop in/out/exit, FX on/off. Caveat: if hotcue pads end up using NeoTrellis, we still need these 19 separately. |

## Connectors

| Part | Qty | Source | Cost | Rationale |
|---|---|---|---|---|
| Adam Tech USB-B-S-RA panel/PCB receptacle | 1 | Digikey `USB-B-S-RA` | $0.48 | Rugged for a knockabout controller; through-hole right-angle, easy to panel-mount. |

---

## Running cost estimate

| Status | Total |
|---|---|
| Locked (MCU + button expanders + pot mux) | **~$11.71** (MCU already on hand) |
| Likely Digikey-only basic build (per BOM agent's full estimate) | **~$71** |
| Realistic w/ shortcuts (AS5600 jogs, NeoTrellis) | **$90–110** |
| Premium (Innofader, decent jog encoders) | **$150–180** |

## Open decisions

1. **Hotcue pads** — NeoTrellis (×2, ~$34) vs. individual NeoPixels + 16 panel switches. NeoTrellis simplifies the build massively but locks in its pad layout.
2. **Pitch fader length** — 60mm Bourns (Digikey, $2.20) vs. 100mm ALPS NOS (eBay/AliExpress, $15–25). Full DJ feel only at 100mm.
3. **Crossfader** — Bourns PTL45 (Digikey, $2.57, carbon-track, will scratch) vs. Mini Innofader Pro ($60+, contactless, lifetime).
4. **Jog encoder** — Bourns PEC11R 24-PPR mechanical (Digikey, $2.27 ea, coarse) vs. AS5600 magnetic modules from AliExpress ($3–5 ea, 4096-step, off-platform).
