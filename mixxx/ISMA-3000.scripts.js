// Script bridge for the ISMA 3000 mapping.
//
// The deck B jog wheel is a rotary encoder (not a touch-sensitive platter), so
// Mixxx's built-in <SelectKnob/> path can only drive `jog` (subtle pitch bend
// on a playing track) — no visible scrub. To scrub the waveform we drive
// `scratch2` via engine.scratchTick, which needs enable/disable calls around
// bursts of movement. This bridge auto-enables on the first tick and disables
// after a short idle window since a rotary encoder has no touch signal.
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

ISMA3000.deckB = {
    scratching: false,
    idleTimer: 0,
};

ISMA3000.init = function (id, debug) {};

ISMA3000.shutdown = function () {
    if (ISMA3000.deckB.scratching) {
        engine.scratchDisable(2, false);
        ISMA3000.deckB.scratching = false;
    }
};

ISMA3000.jogB = function (channel, control, value, status, group) {
    // Two's-complement MIDI relative decode: 1..63 = +1..+63, 65..127 = -63..-1.
    var delta = (value < 64) ? value : value - 128;

    if (!ISMA3000.deckB.scratching) {
        engine.scratchEnable(2, ISMA3000.INTERVALS_PER_REV, ISMA3000.RPM,
                             ISMA3000.ALPHA, ISMA3000.BETA);
        ISMA3000.deckB.scratching = true;
    }
    engine.scratchTick(2, delta);

    if (ISMA3000.deckB.idleTimer !== 0) {
        engine.stopTimer(ISMA3000.deckB.idleTimer);
    }
    ISMA3000.deckB.idleTimer = engine.beginTimer(ISMA3000.SCRATCH_IDLE_MS, function () {
        engine.scratchDisable(2, true);
        ISMA3000.deckB.scratching = false;
        ISMA3000.deckB.idleTimer = 0;
    }, true);
};
