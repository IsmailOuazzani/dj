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
// Docs:
//   https://github.com/mixxxdj/mixxx/wiki/Midi-Scripting
//   https://manual.mixxx.org/2.4/en/chapters/appendix/mixxx_controls.html

var ISMA3000 = {};

// Tuning constants.
ISMA3000.SCRATCH_IDLE_MS = 20;      // disable scratch after this long with no ticks
ISMA3000.INTERVALS_PER_REV = 96;    // 24-detent encoder, 4 quadrature transitions/detent
ISMA3000.RPM = 33 + 1 / 3;          // vinyl feel
ISMA3000.ALPHA = 1.0 / 8;           // scratch filter (Mixxx defaults)
ISMA3000.BETA  = ISMA3000.ALPHA / 32;

ISMA3000.deckA = {
    scratching: false,
    idleTimer: 0,
};

ISMA3000.deckB = {
    scratching: false,
    idleTimer: 0,
};

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

// Shared by jogA/jogB — deckNum is the Mixxx scratch channel (1 or 2), state is
// that deck's {scratching, idleTimer} bookkeeping object.
ISMA3000.jogTick = function (deckNum, state, value) {
    // Two's-complement MIDI relative decode: 1..63 = +1..+63, 65..127 = -63..-1.
    var delta = (value < 64) ? value : value - 128;

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
    ISMA3000.jogTick(1, ISMA3000.deckA, value);
};

ISMA3000.jogB = function (channel, control, value, status, group) {
    ISMA3000.jogTick(2, ISMA3000.deckB, value);
};
