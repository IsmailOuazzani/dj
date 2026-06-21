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

| Subsystem | Qty | Pick | Digikey P/N | Cost (USD)* | Notes |
|---|---|---|---|---|---|
| Pitch faders | 2 | Bourns **PTL01-15R0-103B1** (100 mm, 10 kΩ, B-taper linear) | `PTL01-15R0-103B1-ND` | $6.10 ea / $12.20 | Full 100 mm DJ-fader length, on Digikey. Pro-audio PTL series body. |
| Channel faders | 2 | TBD — likely Bourns `PTL45-15O0-104A3` (~$2.14 ea on Digikey) | TBD | TBD | 45 mm linear, conductive plastic. |
| Crossfader | 1 | Bourns **PTL45-15O0-103B2** (45 mm, 10 kΩ, linear) | `118-PTL45-15O0-103B2-ND` | $2.57 | Best Digikey-orderable crossfader. Carbon-element wiper — will scratch eventually; route panel cutout to standard 45 mm Mini-Innofader footprint as a v2 upgrade path. |

## Encoders

| Subsystem | Qty | Pick | Digikey P/N | Cost (USD)* | Notes |
|---|---|---|---|---|---|
| Beat-loop + browse (w/ push) | 3 | TBD — likely Bourns `PEC11R-4215F-S0024` (24 PPR w/ push, ~$2.38 ea) | TBD | TBD | Mechanical, breadboard-friendly. |
| FX select (no push) | 1 | TBD — likely Bourns `PEC11R-4015F-S0024` (24 PPR no push, ~$2.30 ea) | TBD | TBD | |
| Jog wheels | 2 | Adafruit **AS5600 STEMMA QT breakout** (PID 6357) | `1528-6357-ND` | $5.95 ea / $11.90 | Magnetic angle sensor, 4096 steps/rev, I²C. Contactless = no detent, unlimited life. **Caveat:** requires a diametric 6 mm magnet glued to the jog spindle (sold separately on Digikey, ~$1–2 ea); mechanical bearing for the platter is on the builder. Breadboard-friendly (headers ship un-soldered). |

## Hotcue pads (RGB)

**Open decision — re-research in progress.** Earlier candidate (Adafruit NeoTrellis 4×4) was rejected because it ships as a bare PCB; the silicone elastomer that gives it pad-feel is a separate SKU (Adafruit PID 1611). Looking for alternatives where the **button and LED are integrated as a single part, ready to press out of the box**. Candidates being evaluated: NeoTrellis + elastomer combo, RGB illuminated tactile switches (single-SKU button+LED), arcade-style RGB pushbuttons, and any other pre-assembled RGB-pad modules carried by digikey.ca.

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

\* All prices in this document quoted as **USD from digikey.com**, from the research agents' findings. Actual ordering happens on **digikey.ca in CAD** — rough conversion: USD × 1.35 ≈ CAD. Budget per SPEC.md is **≤ $200 CAD total**.

| Status | Total USD | Approx CAD |
|---|---|---|
| Locked subtotal so far (MCP23017 + CD4067 breakout + pitch faders + crossfader + AS5600 jogs) | **~$38.78** | ~$52 |
| Plus likely Digikey-only basic remaining (pots, encoders, channel faders, tactiles, USB, hotcue pads TBD) | **~$70 total all-in** | ~$95 |
| Headroom against $200 CAD budget | | **~$105 CAD** |

## Open decisions

1. **Hotcue pads** — re-research in progress for integrated-button-and-LED options (see Hotcue pads section above).
