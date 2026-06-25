# Mixxx mapping

This directory holds the Mixxx controller mapping(s) for the ISMA 3000.
Mixxx is the target host app (see `../SPEC.md`).

## What's here

- `ISMA-3000.midi.xml` — v0 validation mapping. Binds the single firmware
  CC (channel 1, CC#21) to Deck 1's volume fader (`[Channel1]`, `volume`).
  Used to prove the USB-MIDI pipeline end-to-end before any other controls land.

## How it gets to Mixxx

Mixxx 2.5 on macOS is sandboxed (org.mixxx.mixxx) and reads user controller
mappings from:

```
~/Library/Containers/org.mixxx.mixxx/Data/Library/Application Support/Mixxx/controllers/
```

The sandbox **blocks symlinks pointing outside the container** — confirmed by
"Could not open xml file" warnings in `mixxx.log`. So `firmware/setup` copies
`ISMA-3000.midi.xml` into that folder instead. After editing the in-repo file,
re-sync with:

```sh
cd firmware && task mixxx:sync   # or just re-run ./firmware/setup
```

Both are idempotent: the copy is skipped if the destination is already
byte-identical.

Manual equivalent:

```sh
MIXXX_DIR="$HOME/Library/Containers/org.mixxx.mixxx/Data/Library/Application Support/Mixxx/controllers"
mkdir -p "$MIXXX_DIR"
cp mixxx/ISMA-3000.midi.xml "$MIXXX_DIR/"
```

## Enabling the mapping in Mixxx

After the firmware is flashed and the device is plugged in:

1. Launch Mixxx.
2. **Preferences -> Controllers**. You should see `ISMA 3000` in the
   sidebar (that's the USB product string the firmware advertises).
3. Click it, check **Enabled**.
4. In the **Load Mapping** dropdown, pick **ISMA 3000** (the `<info><name>`
   from the XML). If Mixxx auto-loaded it on first connect, this will already
   be selected.
5. Click **Apply**, then **OK**.
6. Move the pot. The Deck 1 volume fader on screen should track it.

## Gotchas

- **Soft-takeover.** Mixxx can be configured to ignore controller input until
  the physical control "catches up" with the on-screen value (so faders don't
  jump when you load a mapping mid-mix). For initial validation that's
  confusing — the fader won't move at all until you happen to cross the
  current value. If it looks dead, sweep the pot end-to-end once. To disable
  for testing: edit the mapping's `<options>` — `<Normal/>` (what we ship)
  does *not* engage soft-takeover; `<soft-takeover/>` does. Leave as `<Normal/>`
  for v0.
- **macOS device-name caching.** Mixxx matches mappings to devices by the USB
  product string ("ISMA 3000"). If you later change
  `board_build.usb_product` in `firmware/platformio.ini`, macOS and Mixxx will
  see that as a brand new device — the binding here will silently stop
  applying and you'll need to enable the mapping again under the new name.
  Treat "ISMA 3000" as a stable identifier.
- **Sandboxed path is the only one that works on 2.4.** The legacy
  `~/Library/Application Support/Mixxx/` path is from pre-2.3 Mixxx and is
  ignored by the current sandboxed build.

## References

Bare URLs (per `../CLAUDE.md` rule):

https://github.com/mixxxdj/mixxx/wiki/Midi-Controller-Mapping-File-Format
https://github.com/mixxxdj/mixxx/wiki/Controller-Mapping-File-Locations
https://github.com/mixxxdj/mixxx/wiki/Contributing-Mappings
https://manual.mixxx.org/2.4/en/chapters/controlling_mixxx.html
https://manual.mixxx.org/2.6/en/chapters/appendix/settings_directory
