# DJ Controller — SPEC

A from-scratch, 2-deck DJ controller that acts as a USB-MIDI input device for [Mixxx](https://mixxx.org/) running on a PC. No onboard audio, no screens, no scratching. Mid-size bench unit (DDJ-400 / FLX4 class, target footprint ~48 × 27 cm).

---

## Goals

- 2-deck mixing workflow with full per-deck performance controls (loops, hotcues, sync, EQ, filter, tempo, jog).
- Plug-and-play over USB MIDI on macOS/Linux/Windows. Class-compliant — no driver install.
- Primary host integration: **Mixxx**. The mapping is the source of truth for what each control does.
- Tactile feel approaching commercial entry/mid-range controllers (long pitch fader, real crossfader, real channel faders, clicky encoders).
- LED feedback for state that the user can't otherwise see (which hotcues are set, play/sync engaged).
- **Breadboard-prototype-able.** Every component selected for v1 must be breadboard-compatible — through-hole DIP, breakout-mounted SMD, or panel-mount with through-hole pins — so each subsystem can be proved on breadboard before a PCB is committed to. SMD-only parts without an available breakout are disqualified.
- **Budget: ≤ $200 CAD total** for all v1 parts, including spares and mistakes. Sourcing is restricted to digikey.ca.

## Non-goals (v1)

- **No scratching.** Jog wheels are rotation-only (encoder). No capacitive touch top.
- **No onboard audio.** No USB sound card, no master/booth outs, no headphone amp on the device. Host computer's existing audio interface handles all audio.
- **No screens / displays.** All feedback is via LEDs.
- **No microphone input.** No XLR/TRS preamp on the controller.
- **No 4-deck support.** Two physical decks, two channels, no deck-switching.
- **No standalone mode.** Device is useless without a host running Mixxx (or another DJ app that accepts generic MIDI).
- **No wireless.** USB-MIDI only.
- **No external MIDI / sync out.** USB only; no 5-pin DIN, no TRS MIDI, no clock out.
- **No pad-mode switching.** The 8 pads per deck are hotcues only — no sampler / loop-roll / beat-jump modes.
- **No external power.** USB bus-powered only (no barrel jack, no PSU).
- **No dedicated pitch-range buttons.** Pitch range (±6/10/16/25 %) is set globally in Mixxx config.

---

## Control surface

### Per-deck (×2, mirrored left/right)

| Control | Type | Notes |
|---|---|---|
| Jog wheel | Rotary encoder (high-res) | Rotation only. Used for pitch bend / nudge and library scroll when shifted. No touch sensing. |
| Pitch fader | Long linear fader (~60–100 mm) | Tempo. Analog read via ADC. |
| Channel volume | Linear fader (~45–60 mm) | Per-channel level. |
| EQ HIGH | Rotary potentiometer | 3-band isolator-style EQ. |
| EQ MID | Rotary potentiometer | |
| EQ LOW | Rotary potentiometer | |
| Filter | Rotary potentiometer | Center-detent preferred. HP above center, LP below. |
| PLAY / PAUSE | Backlit button | LED reflects playing state. |
| SYNC | Backlit button | LED reflects sync engaged. |
| CUE (transport) | Button | Standard DJ cue point return / preview. |
| PFL / Headphone cue | Button | Routes deck to headphones in Mixxx. |
| LOOP IN | Button | Sets loop in point. |
| LOOP OUT | Button | Sets loop out point. |
| LOOP EXIT / REENTER | Button | Exits an active loop or re-enters last loop. |
| Beat-loop encoder | Rotary encoder w/ push | Twist to set loop length (¼, ½, 1, 2, 4… beats). Push to engage/disengage auto beat-loop. |
| Hotcues 1–8 | 8 × RGB backlit pads | LED off = unset, lit = set. Color may indicate state (see Feedback). |
| LOAD | Button | Loads currently-selected library track to this deck. |
| SHIFT | Button | Held modifier. Re-maps other controls to secondary functions (see Behaviors). |

### Shared / mixer section

| Control | Type | Notes |
|---|---|---|
| Crossfader | Linear fader (~45 mm) | Standard A↔B crossfade. |
| Master volume | Rotary potentiometer | Master out level (sent to Mixxx; host audio interface still controls final analog level). |
| Headphone volume | Rotary potentiometer | Headphone level in Mixxx. |
| Headphone cue/mix | Rotary potentiometer | Blend between PFL'd deck(s) and master in the headphones. |
| Browse encoder | Rotary encoder w/ push | Twist to scroll library; push to expand/collapse or focus. (Load A / Load B happen via per-deck LOAD buttons, not the encoder.) |
| FX SELECT | Rotary encoder (no push) | Twist to cycle through Mixxx's effect list. No push; FX ON/OFF stays a dedicated button. |
| FX LEVEL | Rotary potentiometer | Wet/dry (Mixxx super knob). |
| FX ON/OFF | Backlit button | Enables/disables the effect unit. LED reflects state. |

### Counts (v1 BOM-ish summary)

- Buttons: 16 hotcue pads (RGB) + 2 play + 2 sync + 2 cue + 2 PFL + 2 load + 2 shift + 6 loop (3 per deck) + 1 FX on/off = **35 buttons** (16 RGB + 19 plain or single-color).
- Faders: 2 pitch + 2 channel + 1 crossfader = **5 faders**.
- Pots: 6 EQ + 2 filter + 1 master + 1 hp vol + 1 hp cue/mix + 1 FX level = **12 potentiometers**.
- Encoders: 2 jog + 2 beat-loop (w/ push) + 1 browse (w/ push) + 1 FX select (no push) = **6 encoders** (3 with push-switch).

---

## Behaviors & modes

### Transport
- **Play/Pause**: toggles deck play state. LED lit while playing.
- **Cue**: standard DJ cue behavior (return to cue point on release; hold to preview from cue). Mapped via Mixxx's default cue handling.
- **Sync**: toggles beat-sync against the other deck / master sync in Mixxx. LED lit while sync engaged.

### Looping
- **Manual loop**: LOOP IN sets the in point; LOOP OUT sets the out point and immediately starts looping; LOOP EXIT exits the active loop (and re-enters last loop on second press, per Mixxx's `reloop_toggle`).
- **Beat loop**: rotating the beat-loop encoder selects a length (¼, ½, 1, 2, 4, 8, 16, 32 beats). Pushing the encoder engages a beat-loop of the currently-selected length; pushing again exits.
- Manual and beat-loops should not fight each other — engaging one cancels the other (Mixxx handles this by default).

### Jog
- Free rotation: pitch bend (nudge) while the deck is playing.
- SHIFT + jog: scroll library (see Shift).

### Shift (per deck)
SHIFT is a held modifier. While held on a given deck, that deck's controls take on secondary functions. v1 planned mappings:

- SHIFT + jog → library scroll (alternative to the browse encoder).
- SHIFT + hotcue pad → delete that hotcue.
- SHIFT + SYNC → set this deck as master sync.
- SHIFT + LOOP IN / LOOP OUT → beat-jump backward / forward by the current beat-loop length.
- SHIFT + PLAY → reverse play (Mixxx `reverse`).

SHIFT is per-deck (not shared) so the user can hold SHIFT on deck A while normally operating deck B.

### EQ / Filter
- Three-band EQ per deck. Cut-only or kill-style behavior is up to the Mixxx EQ preset.
- Filter knob: center = bypass, CW = high-pass sweep, CCW = low-pass sweep (Mixxx Quick Effects "Filter" preset).

### FX
- One shared FX unit assigned to whichever deck(s) the user chooses (Mixxx FX1 by default, with both decks assigned).
- **FX SELECT** (encoder): twist to cycle Mixxx's effect list for the active unit. CW = next, CCW = previous.
- **FX LEVEL** = super-knob (wet/dry + tied parameters).
- **FX ON/OFF** toggles the unit. LED reflects state.

### Tempo / pitch
- Pitch fader sets deck tempo offset.
- Pitch range (±6/10/16/25 %) is set globally in Mixxx config. No controller-side range button.

### Browse / load
- Browse encoder scrolls the library tree/list in Mixxx.
- Browse encoder push: expand/collapse current item.
- LOAD A / LOAD B (per deck): load currently-highlighted track to that deck.

### Headphones
- Per-deck PFL buttons route their deck into the headphone cue bus.
- HEADPHONE VOLUME sets cue bus level in Mixxx.
- HEADPHONE CUE/MIX blends between PFL and master in the cue bus.

---

## Feedback (LEDs)

LED feedback is scoped to what the user genuinely can't infer from knob/fader position:

- **Hotcue pads (RGB)**: off when unset, lit when set. Color reserved for future state encoding (e.g. loop hotcues vs regular). v1 may ship a single color.
- **PLAY**: lit when the deck is playing.
- **SYNC**: lit when sync is engaged.
- **FX ON/OFF**: lit when the effect unit is enabled.
- All other buttons (loop in/out/exit, PFL, load, cue, beat-loop encoder push) may be backlit for visibility but are **not** required to reflect state in v1.

The host (Mixxx) drives LED state via outgoing MIDI messages defined in the mapping. The firmware is a dumb translator — it does not maintain its own state machine for LEDs.

---

## Appendix A — Technical decisions (to be filled in)

These will be decided as hardware selection progresses. Listed here so they have a home.

- **Microcontroller / dev board**: TBD. Needs USB-MIDI class-compliant support, enough GPIO/ADC channels for the matrix + analog reads, and an ecosystem the user is comfortable with. Candidates to consider: Raspberry Pi Pico (RP2040), Teensy 4.x, STM32 (Blue/Black Pill class).
- **Button scanning (35 buttons)**: matrix vs. shift-register vs. I/O expander (MCP23017 etc.). Depends on chosen MCU's pin budget.
- **Analog inputs (12 pots + 2 pitch faders + 2 channel faders + 1 crossfader = 17 analog signals)**: depending on MCU ADC channel count, may require a CD4051/CD4067 analog mux.
- **Encoders (6 total, 2 high-res jog)**: jog wheels likely benefit from dedicated quadrature decoding (PIO on RP2040, or a hardware encoder peripheral on STM32). Beat-loop, browse, and FX-select encoders can be polled.
- **RGB LEDs (16 hotcue pads)**: WS2812 chain is the cheap path; a dedicated driver is the robust path. USB bus power budget (500 mA on USB 2.0) constrains LED brightness — current limiting in firmware is mandatory.
- **Enclosure**: TBD. Likely laser-cut acrylic + 3D-printed knob caps, or a milled aluminum panel.
- **Firmware framework**: TBD. Arduino, PlatformIO, or bare HAL — depends on MCU.

## Appendix B — Open questions

*All v1 layout questions resolved. Remaining unknowns are hardware-implementation choices captured in Appendix A.*
