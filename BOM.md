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
| Adafruit 5346 — MCP23017 STEMMA QT breakout (carries the MCP23017 silicon on a PCB with pre-soldered STEMMA QT JST-SH connectors, onboard bypass cap, address-strap jumpers A0/A1/A2) | 3 | Digikey `1528-5346-ND` | $8.70 ea / $26.10 | Swapped from the bare MCP23017-E/SP DIP because that part went to 0 stock with a 5-week Microchip lead time — disqualified by SPEC's in-stock-now rule. The breakout carries the same MCP23017 silicon and works with the identical Adafruit_MCP23X17 driver code (zero firmware change). STEMMA QT means **zero soldering** on these — chain to the Wire I²C bus via the cable kit (new section below). Address straps A0/A1/A2 give 8 addresses; bridge for 0x20/0x21/0x22 by jumper. 768 in stock, Active. https://www.digikey.ca/en/products/detail/adafruit-industries-llc/5346/15913270 |
| 0.1 µF ceramic bypass caps (Kemet C320C104K5R5TA — X7R 50V radial through-hole, 2.54 mm lead pitch) | 3 | Digikey `399-4264-ND` | $0.47 ea / $1.41 | One per MCP23017. Active lifecycle, ~170k in stock at Digikey CA. https://www.digikey.ca/en/products/detail/kemet/C320C104K5R5TA/818040 |
| 4.7 kΩ I²C pull-up resistors (Stackpole CF14JT4K70 — 5% 1/4 W axial through-hole carbon film) | 4 | Digikey `CF14JT4K70CT-ND` | $0.16 ea / $0.64 | 2× for primary I²C bus (Wire: SDA0/SCL0 — MCP23017s + NeoKeys + 1× AS5600), 2× for secondary I²C bus (Wire1: SDA1/SCL1 — second AS5600). Two buses resolve the AS5600 fixed-address (`0x36`) collision without an I²C mux IC. Active lifecycle, ~105k in stock. https://www.digikey.ca/en/products/detail/stackpole-electronics-inc/CF14JT4K70/1741428 |
| **Subtotal** | | | **~$6.25** | |

## Pots & faders — analog input scaling

| Part | Qty | Source | Cost | Rationale |
|---|---|---|---|---|
| SparkFun CD74HC4067 16-channel analog mux breakout (BOB-09056) | 1 | Digikey `1568-09056-ND` | $6.50 | One chip covers all 17 analog signals (16 via mux + 1 direct ADC for the 17th). Breadboard-friendly headers. CD4067 DIP is EOL, this is the cleanest breadboard-compatible path. |
| **Subtotal** | | | **~$6.50** | |

## Pots (the actual pots and faders)

| Subsystem | Qty | Pick | Notes |
|---|---|---|---|
| EQ + master + headphone + FX level pots | 10 | Same Sky `PT01-D115D-B103` (Digikey `2223-PT01-D115D-B103-ND`, ~$1.55 ea CAD qty-10) | Through-hole panel-mount, 10k B-taper (linear). Same family as the filter detent pots — single knob SKU fits all 12 positions. 3D STEP model on the Same Sky product page. 6mm D-shaft × 15mm length. https://www.digikey.ca/en/products/result?keywords=PT01-D115D-B103 |
| Filter pots (center-detent) | 2 | Same Sky `PT01-D225D-B103` (Digikey `2223-PT01-D225D-B103-ND`, $1.33 ea) | "D2xxD" prefix in Same Sky's PT01 naming = center detent. Same family as the other 10 pots — one knob SKU fits all 12 positions. 3D STEP model published on the Same Sky product page. 6mm D-shaft × 25mm length. https://www.digikey.ca/en/products/result?keywords=PT01-D225D-B103 |

## Faders

| Subsystem | Qty | Pick | Digikey P/N | Cost (USD)* | Notes |
|---|---|---|---|---|---|
| Pitch faders | 2 | Bourns **PTL01-15R0-103B1** (100 mm, 10 kΩ, B-taper linear) | `PTL01-15R0-103B1-ND` | $6.10 ea / $12.20 | Full 100 mm DJ-fader length, on Digikey. Pro-audio PTL series body. |
| Channel faders | 2 | Bourns **PTL45-15O0-103B2** (45 mm, 10 kΩ, linear B-taper) | `118-PTL45-15O0-103B2-ND` | $3.76 ea / $7.52 | Consolidated to the same SKU as the crossfader — one fader part across the deck strip. The previous `103A3` pick was both unavailable on Digikey CA and audio-taper (not linear, per the Bourns PTL datasheet "A vs B" code). Linear is correct for DJ channel volume; Mixxx applies its own fader curve. |
| Crossfader | 1 | Bourns **PTL45-15O0-103B2** (45 mm, 10 kΩ, linear) | `118-PTL45-15O0-103B2-ND` | $2.57 | Best Digikey-orderable crossfader. Carbon-element wiper — will scratch eventually; route panel cutout to standard 45 mm Mini-Innofader footprint as a v2 upgrade path. |

## Encoders

| Subsystem | Qty | Pick | Digikey P/N | Cost (USD)* | Notes |
|---|---|---|---|---|---|
| Beat-loop + browse (w/ push) | 3 | Bourns `PEC11R-4215F-S0024` (24 PPR, 24 detents, w/ push) | `PEC11R-4215F-S0024-ND` | $4.60 ea / $13.80 | Mechanical, breadboard-friendly. 6mm flatted shaft fits the Cliff CL170822BR knob. 3D model in Digikey EDA/CAD Models. https://www.digikey.ca/en/products/detail/bourns-inc/PEC11R-4215F-S0024/4499665 |
| FX select (no push) | 1 | Bourns `PEC11R-4215F-N0024` (24 PPR, 24 detents, no push) | `PEC11R-4215F-N0024-ND` | $3.73 | Detent feel for discrete effect-list cycling. Suffix corrected from the earlier `-S0024` placeholder — the `-S` suffix actually has a switch; `-N` is the true no-push variant. 3D model in Digikey EDA/CAD Models. https://www.digikey.ca/en/products/result?keywords=PEC11R-4215F-N0024-ND |
| Jog wheels | 2 | Adafruit **AS5600 STEMMA QT breakout** (PID 6357) | `1528-6357-ND` | $5.95 ea / $11.90 | Magnetic angle sensor, 4096 steps/rev, I²C. Contactless = no detent, unlimited life. **AS5600 has a hard-fixed I²C address (`0x36`) with no straps** — the two chips cannot share one bus. Split across the RP2040's two I²C peripherals: deck-A AS5600 on primary `Wire` (SDA0/SCL0, shared with MCP23017s + NeoKeys); deck-B AS5600 on secondary `Wire1` (SDA1/SCL1) on two free GPIOs. Adds 2× extra 4.7 kΩ pull-ups for the second bus (see Buttons section) and zero new ICs. Requires the matching diametric magnet (next row) glued axially to the jog spindle; mechanical bearing for the platter is on the builder. Breadboard-friendly (headers ship un-soldered). |
| Diametric jog magnet | 2 | Radial Magnets `8995` (6 mm × 2.5 mm, N35 NdFeB, **diametric** magnetization) | `469-1024-ND` | $0.62 ea / $1.24 | Mandatory for AS5600 to read angle — axial magnets give garbage output, this part is explicitly attribute-tagged "Diametric" on Digikey. Glue to the underside of the rotating jog shaft, centered ≤0.5 mm laterally / ≤1.5 mm vertically from the AS5600 IC face. https://www.digikey.ca/en/products/detail/radial-magnets-inc/8995/5126077 |

## Hotcue keys (RGB)

Two NeoKey 1×4 modules — one per deck, 4 RGB-illuminated MX-style key switches each (8 hotcue keys total). MX hot-swap sockets mean switches push in without soldering.

| Part | Qty | Digikey CA P/N | CAD unit | Ext. | Link |
|---|---|---|---|---|---|
| Adafruit NeoKey 1×4 QT I²C breakout (PID 4980) | 2 | `1528-4980-ND` | $15.51 | $31.02 | https://www.digikey.ca/en/products/detail/adafruit-industries-llc/4980/14319123 |
| Cherry MX2A-G1NB RGB-slot key switch | 8 | `1644-MX2A-G1NB-ND` | $2.73 | $21.84 | https://www.digikey.ca/en/products/detail/cherry-americas-llc/MX2A-G1NB/21738375 |
| Adafruit relegendable MX keycaps, 10-pack (PID 5039) | 1 | `1528-5039-ND` | ~$7.72 | $7.72 | https://www.digikey.ca/en/products/result?keywords=1528-5039-ND |
| **Subtotal** | | | | **~$60.73 CAD** | |

Notes: NeoKey 1×4 runs Adafruit's seesaw firmware over I²C — chains on the same SDA/SCL pair as the MCP23017 button expanders, no extra MCU pins needed. Each board has solderable address-jumper pads to set a unique I²C address so the two boards don't collide. Assembly: solder 4-pin headers (I²C: SDA/SCL/3V3/GND) on each NeoKey PCB; push MX switches into the Kailh hot-swap sockets (no soldering); snap keycaps on top. Breadboard-friendly via header pins. Feel: mechanical keyboard click — different from a typical DJ silicone pad. Two unused keycaps from the 10-pack become spares.

## Misc switches (non-hotcue buttons)

| Part | Qty | Source | Cost | Rationale |
|---|---|---|---|---|
| E-Switch TL1105SPF160Q tactile (through-hole, 6mm × 6mm, 160 gf, 100K cycles) | 19 | Digikey `EG1861-ND` | $0.47 ea / $8.93 | Standard through-hole tactile for play, sync, cue, PFL, load, shift, loop in/out/exit, FX on/off. **Swapped from TL1100F160Q** to fix 1RBLK cap fitment — per the E-Switch 1RBLK datasheet, the cap mates with TL1105SP / TL1105JA / TL3301SP / TL2201Z / TL4201Z series, **not** TL1100. The TL1105SPF160Q is the direct 6×6mm equivalent that's on the 1RBLK fitment list. https://www.digikey.ca/en/products/detail/e-switch/TL1105SPF160Q/271558 |

## Knobs & caps (user-touched mounted parts)

Required by SPEC: every rotary pot, fader, non-jog encoder, and tactile button ships with a cap mounted — no bare shafts or actuators on the finished panel.

| Part | Qty | Digikey CA P/N | CAD unit | Ext. | 3D model? | Link |
|---|---|---|---|---|---|---|
| Cliff CL170822BR — soft-touch rubber knob, black w/ white pointer, 6mm D push-fit | 16 | `4654-CL170822BR-ND` | $1.25 | $20.00 | Yes (Digikey EDA/CAD Models) | https://www.digikey.ca/en/products/result?keywords=4654-CL170822BR-ND |
| Davies Molding 1300-B — knurled plastic wedge fader cap | 5 | `1722-1326-ND` | $2.02 | $10.10 | **No (SPEC exception — model manually from datasheet)** | https://www.digikey.ca/en/products/detail/davies-molding-llc/1300-B/7908413 |
| E-Switch 1RBLK — round tactile button cap, black, snap-fit | 19 | `EG1882-ND` | $0.31 | $5.89 | Yes (Digikey EDA/CAD Models) | https://www.digikey.ca/en/products/detail/e-switch/1RBLK/271579 |
| **Subtotal** | | | | **~$35.99 CAD** | | |

Notes: One knob SKU covers all 12 pots (PT01-D115D + PT01-D225D detent variants) and all 4 non-jog encoders (Bourns PEC11R `F` suffix = 6mm flatted shaft, datasheet-confirmed). The Davies 1300-B is the only in-stock wide-flat slider cap on Digikey CA — fits the Bourns PTL lever (2.05×0.6mm blade) with a kapton or electrical-tape shim inside its 3.6×1.1mm cutout. No 3D model available anywhere for the 1300-B; cap dimensions (25×15×10mm wedge) come from the Davies datasheet for enclosure planning — this is the one SPEC-flagged exception in the BOM. The 1RBLK lifts the TL1100F160Q tactile actuator ~9mm above the PCB for a clean panel-protruding press surface and lets us color-code button function (loop in/out/exit, PFL, load, shift, etc.) later via swapping caps.

## I²C bus cables (STEMMA QT chain)

Connects the 3× Adafruit 5346 (MCP23017 STEMMA breakouts) + 2× NeoKey 1×4 + 1× AS5600 (deck-A jog) into one solderless daisy chain on the primary I²C bus (Wire / SDA0+SCL0). The 2nd AS5600 (deck-B jog) sits on the secondary Wire1 bus and uses its own jumper.

| Part | Qty | Digikey CA P/N | CAD unit | Ext. | Link |
|---|---|---|---|---|---|
| Adafruit 4209 — STEMMA QT JST-SH ↔ 4-pin male jumper cable, 150 mm | 2 | `1528-4209-ND` | $1.39 | $2.78 | https://www.digikey.ca/en/products/detail/adafruit-industries-llc/4209/10230003 |
| Adafruit 4210 — STEMMA QT JST-SH ↔ JST-SH, 100 mm | 5 | `1528-4210-ND` | $1.39 | $6.95 | https://www.digikey.ca/en/products/detail/adafruit-industries-llc/4210/10230021 |
| **Subtotal** | | | | **~$9.73 CAD** | |

Notes: 1× 4209 bridges the breadboard (RP2040 Wire SDA0/SCL0 + 3V3/GND) into the first STEMMA board on the Wire bus; 5× 4210 chain through the 6 STEMMA-equipped boards (3× 5346 + 2× NeoKey + 1× AS5600 deck-A). The second 4209 bridges the breadboard's Wire1 (SDA1/SCL1) directly into deck-B AS5600 (sole device on that bus, no chain needed). Both cable types verified Active and in stock at Digikey CA (3,087 and 13,070 units respectively).

## Connectors

| Part | Qty | Source | Cost | Rationale |
|---|---|---|---|---|
| Adafruit 937 panel-mount USB-B-female ↔ micro-B-male cable (1 ft / 30 cm, 2 screw locks) | 1 | Digikey `1528-1574-ND` | $6.16 | Replaces a separate PCB receptacle + internal pigtail in one assembly. User plugs a standard USB-B cable into the panel side; the internal micro-B male plugs straight into the Nano RP2040 Connect's onboard micro-USB. Avoids the soldering-to-test-pads risk that a panel-only USB-B receptacle would create (no plain USB-B-male → micro-B-male pigtail cable is stocked on Digikey CA). https://www.digikey.ca/en/products/detail/adafruit-industries-llc/937/5844688 |

---

## Running cost estimate

All prices in CAD from digikey.ca (verified by research agents). Budget per SPEC.md: **≤ $250 CAD total**.

| Subsystem | CAD |
|---|---|
| Button I/O expanders (Adafruit 5346 STEMMA ×3 + Kemet caps ×3 + Stackpole pull-ups ×4 for dual bus) | $28.15 |
| I²C bus cables (Adafruit 4209 ×2 + 4210 ×5 — STEMMA QT chain) | $9.73 |
| Analog mux (SparkFun BOB-09056 CD74HC4067 breakout) | $9.51 |
| Pitch faders (Bourns PTL01 ×2, 100 mm) | $17.84 |
| Crossfader (Bourns PTL45-15O0-103B2, 10 kΩ) | $3.76 |
| Channel faders (Bourns PTL45-15O0-103B2 ×2, 10 kΩ) | $7.52 |
| Jog encoders (Adafruit AS5600 ×2 + diametric magnets ×2) | $19.02 |
| Pots (10× Same Sky PT01-D115D-B103 + 2× Same Sky PT01-D225D-B103 detented) | $18.20 |
| Beat-loop + browse + FX encoders (3× Bourns PEC11R-4215F-S0024 + 1× PEC11R-4215F-N0024) | $17.53 |
| Tactile switches (E-Switch TL1105SPF160Q ×19) | $8.93 |
| USB-B panel cable (Adafruit 937, USB-B female ↔ micro-B male, panel-mount) | $6.16 |
| Diametric jog magnets (Radial Magnets 8995 ×2) | $1.24 |
| Hotcue keys (NeoKey 1×4 ×2 + MX switches ×8 + keycaps 10-pk) | $60.73 |
| Knob caps (Cliff CL170822BR ×16) | $20.00 |
| Fader caps (Davies 1300-B ×5) | $10.10 |
| Tactile button caps (E-Switch 1RBLK ×19) | $5.89 |
| **Pre-tax pre-shipping subtotal** | **~$244.31 CAD** |
| Shipping (digikey.ca, free over $100 CAD) | $0 |
| Tax (ON HST 13 % example; varies by province) | ~$31.76 |
| **Estimated delivered total (ON)** | **~$276.07 CAD** |
| Budget cap | $300.00 |
| **Headroom** | **~$23.93 CAD under** |

All v1 component decisions are locked, including the SPI/STEMMA swap for the I/O expanders forced by the new in-stock-now rule. Pre-tax sits ~$56 under the $300 CAD spec cap; delivered (with ON HST) sits ~$24 under, leaving margin for shipping variance, qty-tier price drift, or one impulse add at order time.
