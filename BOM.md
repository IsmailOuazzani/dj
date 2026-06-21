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
| Channel faders | 2 | Bourns **PTL45-15O0-103A3** (45 mm, 10 kΩ, linear) | `118-PTL45-15O0-103A3-ND` | ~$7.52 CAD / pair | 45 mm linear, conductive plastic. 10 kΩ to match the crossfader's impedance (the alternative `-104A3` is 100 kΩ and would mix values across the panel). |
| Crossfader | 1 | Bourns **PTL45-15O0-103B2** (45 mm, 10 kΩ, linear) | `118-PTL45-15O0-103B2-ND` | $2.57 | Best Digikey-orderable crossfader. Carbon-element wiper — will scratch eventually; route panel cutout to standard 45 mm Mini-Innofader footprint as a v2 upgrade path. |

## Encoders

| Subsystem | Qty | Pick | Digikey P/N | Cost (USD)* | Notes |
|---|---|---|---|---|---|
| Beat-loop + browse (w/ push) | 3 | TBD — likely Bourns `PEC11R-4215F-S0024` (24 PPR w/ push, ~$2.38 ea) | TBD | TBD | Mechanical, breadboard-friendly. |
| FX select (no push) | 1 | TBD — likely Bourns `PEC11R-4015F-S0024` (24 PPR no push, ~$2.30 ea) | TBD | TBD | |
| Jog wheels | 2 | Adafruit **AS5600 STEMMA QT breakout** (PID 6357) | `1528-6357-ND` | $5.95 ea / $11.90 | Magnetic angle sensor, 4096 steps/rev, I²C. Contactless = no detent, unlimited life. **Caveat:** requires a diametric 6 mm magnet glued to the jog spindle (sold separately on Digikey, ~$1–2 ea); mechanical bearing for the platter is on the builder. Breadboard-friendly (headers ship un-soldered). |

## Hotcue keys (RGB)

Two NeoKey 1×4 modules — one per deck, 4 RGB-illuminated MX-style key switches each (8 hotcue keys total). MX hot-swap sockets mean switches push in without soldering.

| Part | Qty | Digikey CA P/N | CAD unit | Ext. | Link |
|---|---|---|---|---|---|
| Adafruit NeoKey 1×4 QT I²C breakout (PID 4980) | 2 | `1528-4980-ND` | $15.51 | $31.02 | https://www.digikey.ca/en/products/detail/adafruit-industries-llc/4980/14319123 |
| Cherry MX2A-G1NB RGB-slot key switch | 8 | `1644-MX2A-G1NB-ND` | $2.73 | $21.84 | https://www.digikey.ca/en/products/detail/cherry-americas-llc/MX2A-G1NB/21738375 |
| Adafruit relegendable MX keycaps, 10-pack (PID 5039) | 1 | `1528-5039` | ~$7.87 | $7.87 | https://www.digikey.ca/en/products/result?keywords=Adafruit+5039 |
| **Subtotal** | | | | **~$60.73 CAD** | |

Notes: NeoKey 1×4 runs Adafruit's seesaw firmware over I²C — chains on the same SDA/SCL pair as the MCP23017 button expanders, no extra MCU pins needed. Each board has solderable address-jumper pads to set a unique I²C address so the two boards don't collide. Assembly: solder 4-pin headers (I²C: SDA/SCL/3V3/GND) on each NeoKey PCB; push MX switches into the Kailh hot-swap sockets (no soldering); snap keycaps on top. Breadboard-friendly via header pins. Feel: mechanical keyboard click — different from a typical DJ silicone pad. Two unused keycaps from the 10-pack become spares.

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

All prices in CAD from digikey.ca (verified by research agents). Budget per SPEC.md: **≤ $200 CAD total**.

| Subsystem | CAD |
|---|---|
| Button I/O expanders (MCP23017 ×3 + caps + I²C pull-ups) | $9.47 |
| Analog mux (SparkFun BOB-09056 CD74HC4067 breakout) | $9.51 |
| Pitch faders (Bourns PTL01 ×2, 100 mm) | $17.84 |
| Crossfader (Bourns PTL45-15O0-103B2, 10 kΩ) | $3.76 |
| Jog encoders (Adafruit AS5600 ×2 + diametric magnets ×2) | $19.02 |
| Pots (10× Same Sky PT01 + 2× TT P160KNPD detented) | $19.50 |
| Channel faders (Bourns PTL45-15O0-103A3 ×2, 10 kΩ) | $7.52 |
| Beat-loop + browse + FX encoders (Bourns PEC11R ×4) | $17.98 |
| Tactile switches (E-Switch TL1100F160Q ×19) | $14.25 |
| USB-B jack (Adam Tech USB-B-S-RA) | $0.70 |
| Hotcue keys (NeoKey 1×4 ×2 + MX switches ×8 + keycaps 10-pk) | $60.73 |
| **Pre-tax pre-shipping subtotal** | **~$180.28 CAD** |
| Shipping (digikey.ca, free over $100 CAD) | $0 |
| Tax (ON HST 13 % example; varies by province) | ~$23.44 |
| **Estimated delivered total (ON)** | **~$203.72 CAD** |
| Budget cap | $200.00 |
| **Headroom** | **~$3.72 CAD over** |

All v1 component decisions are now locked. Path A (NeoKey + MX) was chosen over Path B (NeoTrellis + silicone) for design simplicity (8 vs 16 pads) — note this puts the build slightly over the $200 CAD cap once tax is included. Options: bump the spec budget, drop a different pick to a cheaper alternative, or order carefully and absorb the ~$4 overage.
