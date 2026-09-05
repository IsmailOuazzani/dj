// Script bridge for the ISMA 3000 mapping.
//
// Both jog wheels are rotary encoders (not touch-sensitive platters), so
// Mixxx's built-in <SelectKnob/> path can only drive `jog` (subtle pitch bend
// on a playing track) — no visible scrub. To scrub the waveform we drive
// `scratch1`/`scratch2` via engine.scratchTick, which needs enable/disable
// calls around bursts of movement. This bridge auto-enables on the first tick
// and disables after a short idle window since a rotary encoder has no touch
// signal.
//
// Shift is a plain momentary button read on its own MCP pad — the firmware
// sends it as an independent NoteOn/NoteOff, not merged into any other
// message. So unlike the Pioneer DDJ-400 (whose onboard MCU emits a distinct
// CC for shift+jog at the hardware level), our shift state is tracked here in
// script and consulted by the jog/hotcue handlers below — no firmware change
// needed for either behavior.
//
// Docs:
//   https://github.com/mixxxdj/mixxx/wiki/Midi-Scripting
//   https://manual.mixxx.org/2.4/en/chapters/appendix/mixxx_controls.html
//   DDJ-400 jogSearch reference (shift+jog fast seek): https://github.com/mixxxdj/mixxx/blob/main/res/controllers/Pioneer-DDJ-400-script.js

var ISMA3000 = {};

// Tuning constants.
ISMA3000.SCRATCH_IDLE_MS = 20;      // disable scratch after this long with no ticks
ISMA3000.INTERVALS_PER_REV = 96;    // 24-detent encoder, 4 quadrature transitions/detent
ISMA3000.RPM = 33 + 1 / 3;          // vinyl feel
ISMA3000.ALPHA = 1.0 / 8;           // scratch filter (Mixxx defaults)
ISMA3000.BETA  = ISMA3000.ALPHA / 32;
ISMA3000.FAST_SEEK_SCALE = 150;     // shift+jog multiplier, mirrors DDJ-400's fastSeekScale

ISMA3000.deckA = {
    scratching: false,
    idleTimer: 0,
};

ISMA3000.deckB = {
    scratching: false,
    idleTimer: 0,
};

// Shift-held state per deck, keyed by Mixxx deck number (1 = A, 2 = B).
ISMA3000.shift = {1: false, 2: false};

ISMA3000.init = function (id, debug) {};

ISMA3000.shutdown = function () {
    if (ISMA3000.deckA.scratching) {
        engine.scratchDisable(1, false);
        ISMA3000.deckA.scratching = false;
    }
    if (ISMA3000.deckB.scratching) {
        engine.scratchDisable(2, false);
        ISMA3000.deckB.scratching = false;
    }
};

ISMA3000.shiftA = function (channel, control, value, status, group) {
    ISMA3000.shift[1] = (value !== 0);
};

ISMA3000.shiftB = function (channel, control, value, status, group) {
    ISMA3000.shift[2] = (value !== 0);
};

// Shared by jogA/jogB — deckNum is the Mixxx scratch channel (1 or 2), state is
// that deck's {scratching, idleTimer} bookkeeping object, group is the Mixxx
// group string (e.g. "[Channel1]") needed for the shift+jog fast-seek path.
ISMA3000.jogTick = function (deckNum, state, group, value) {
    // Two's-complement MIDI relative decode: 1..63 = +1..+63, 65..127 = -63..-1.
    var delta = (value < 64) ? value : value - 128;

    if (ISMA3000.shift[deckNum]) {
        // Shift held — fast seek instead of scratch, same idea as the DDJ-400's
        // jogSearch: drive `jog` directly, scaled up, rather than scratchTick.
        if (state.scratching) {
            engine.scratchDisable(deckNum, true);
            state.scratching = false;
            if (state.idleTimer !== 0) {
                engine.stopTimer(state.idleTimer);
                state.idleTimer = 0;
            }
        }
        engine.setValue(group, "jog", delta * ISMA3000.FAST_SEEK_SCALE);
        return;
    }

    if (!state.scratching) {
        engine.scratchEnable(deckNum, ISMA3000.INTERVALS_PER_REV, ISMA3000.RPM,
                             ISMA3000.ALPHA, ISMA3000.BETA);
        state.scratching = true;
    }
    engine.scratchTick(deckNum, delta);

    if (state.idleTimer !== 0) {
        engine.stopTimer(state.idleTimer);
    }
    state.idleTimer = engine.beginTimer(ISMA3000.SCRATCH_IDLE_MS, function () {
        engine.scratchDisable(deckNum, true);
        state.scratching = false;
        state.idleTimer = 0;
    }, true);
};

ISMA3000.jogA = function (channel, control, value, status, group) {
    ISMA3000.jogTick(1, ISMA3000.deckA, group, value);
};

ISMA3000.jogB = function (channel, control, value, status, group) {
    ISMA3000.jogTick(2, ISMA3000.deckB, group, value);
};

// Hotcues — normal press activates/jumps to the cue; shift+press clears it
// instead (Mixxx's hotcue_N_clear). Not something the DDJ-400 does (Mixxx's
// own comment on that mapping calls cue-delete "complex and not useful"), but
// straightforward to add here since shift state is already tracked above.
ISMA3000.hotcue = function (deckNum, n, value) {
    if (value === 0) return;  // only act on press, ignore the NoteOff release
    var group = "[Channel" + deckNum + "]";
    var key = ISMA3000.shift[deckNum] ? "hotcue_" + n + "_clear" : "hotcue_" + n + "_activate";
    engine.setValue(group, key, 1);
};

// Binds a hotcue pad to a specific (deck, number) pair — Mixxx's Script-Binding
// looks up handlers by name from the XML, so each pad needs its own named
// function rather than one parameterized handler.
ISMA3000.makeHotcueHandler = function (deckNum, n) {
    return function (channel, control, value, status, group) {
        ISMA3000.hotcue(deckNum, n, value);
    };
};

ISMA3000.hotcue1A = ISMA3000.makeHotcueHandler(1, 1);
ISMA3000.hotcue2A = ISMA3000.makeHotcueHandler(1, 2);
ISMA3000.hotcue3A = ISMA3000.makeHotcueHandler(1, 3);
ISMA3000.hotcue4A = ISMA3000.makeHotcueHandler(1, 4);
ISMA3000.hotcue1B = ISMA3000.makeHotcueHandler(2, 1);
ISMA3000.hotcue2B = ISMA3000.makeHotcueHandler(2, 2);
ISMA3000.hotcue3B = ISMA3000.makeHotcueHandler(2, 3);
ISMA3000.hotcue4B = ISMA3000.makeHotcueHandler(2, 4);
