# Inventory

Components currently on hand for prototyping the DJ controller (see [SPEC.md](SPEC.md)). Build is iterative — we'll source new parts as each subsystem reaches the point of needing them, not all up front.

## On hand

| Item | Qty | Notes |
|---|---|---|
| Breadboards | 2 | Prototyping. |
| Jumper wires | Plenty | Mixed M/M, M/F, F/F assumed. |
| LEDs | Plenty | Through-hole, assorted. Useful for placeholder LED feedback on play/sync/FX before RGB pads arrive. |
| Resistors | Plenty | Assorted values. For LED current limiting and pull-ups/pull-downs. |
| Tactile buttons (kit) | Plenty | 4-pin through-hole momentary switches. Stand-ins for every button in the spec during prototyping. |
| Small blue potentiometers | 1–2 | Hard end-stops (~270° travel) → potentiometers, not encoders. Likely small cermet/trim-style. Stand-ins for any pot in the spec during prototyping (EQ, filter, FX level, master/headphone volume). |
| Arduino Nano RP (RP2040) | 1 | Main MCU for prototyping. Native USB → can act as a USB-MIDI device class-compliant via libraries (TinyUSB / Adafruit_TinyUSB / Arduino MIDIUSB ports). Plenty of GPIO + analog inputs for early prototypes. Pinout + project conventions: [hardware-docs/nano-rp2040-connect.md](hardware-docs/nano-rp2040-connect.md). |

## Philosophy

Prototype each subsystem on the breadboard before committing to a part for the final build. Order in which we'll likely need to source things (rough — confirmed as we go):

1. **Phase 1 — Input prototyping (covered by on-hand):** button reading (kit tactiles), pot reading (the blue pots), single LED feedback, USB-MIDI enumeration on the RP2040.
2. **Phase 2 — Encoders & jog:** at least one true (infinite-turn) rotary encoder for browse / beat-loop / FX-select prototyping. **None on hand** — first new purchase. Eventually a higher-resolution encoder pair (or optical) for the jog wheels.
3. **Phase 3 — Scaling I/O:** analog multiplexer(s) (CD4051/CD4067) once we exceed the RP2040's ADC channels, and/or shift registers or an I/O expander once buttons exceed direct GPIO.
4. **Phase 4 — Real controls:** linear pitch faders (long throw), linear channel faders, crossfader, full-size pots with knob caps, RGB pads (WS2812-based or driver-based).
5. **Phase 5 — Enclosure & panel:** laser-cut / 3D-printed top panel, mounting hardware, final wiring harness.

Each phase's exact BOM gets pinned down as it starts — kept here so we don't pre-buy parts that turn out to be the wrong choice.

## Gaps to track

Things SPEC.md requires that we don't yet have. Don't buy until the relevant phase starts.

- True (infinite-turn) rotary encoders — qty 6 for final build, plus push-switch on 3 of them.
- High-resolution / optical encoders for the 2 jog wheels.
- Long linear pitch faders (~60–100 mm).
- Linear channel faders (×2) and crossfader (×1).
- 12 panel-mount potentiometers (EQ ×6, filter ×2, master, hp vol, hp cue/mix, FX level).
- 16 RGB pads / pad PCB (hotcues).
- 35 panel-mount switches (final, not breadboard kit buttons).
- Analog mux (CD4051 or CD4067) — likely needed once we exceed Nano RP2040's ADC count.
- USB-B (or USB-C) panel-mount jack + cable for the enclosure.
- Enclosure material and knob caps.
