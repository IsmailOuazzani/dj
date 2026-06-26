# DJ Controller

A from-scratch personal build of a DJ controller. Scope (hardware vs. software split, language, frameworks) is not yet decided — ask before adding tooling or dependencies.

## Working rules

- **Never `git commit` without being asked.** Make file edits freely, but leave them uncommitted. Wait for an explicit "commit" before staging.
- **Always include full pasteable URLs in hardware comparison tables.** Whenever a table compares hardware options (parts, modules, ICs, etc.), every row that names a specific part must include the **full URL** to its digikey.ca product page (or vendor product page if not on Digikey) — as **bare text**, NOT a markdown `[text](url)` hyperlink. A dedicated "Link" column is the clearest layout. The user should be able to triple-click the cell and copy the URL out.
- **When a tool/library/toolchain behaves unexpectedly, read the docs before guessing.** If a build flag, config option, or API doesn't work the way you assumed, stop iterating — fetch the official docs, search for the specific issue (GitHub issues, community forums), and only then change the approach. Don't burn cycles trial-and-erroring quoting, ordering, or flag combinations when the answer is one WebFetch / WebSearch away.
- **Red-flag any out-of-stock or lead-time part as a blocker.** When research or audit surfaces a part that is at 0 stock at Digikey Canada, NRND, Last Time Buy, Obsolete, or carries any non-zero manufacturer lead time, the agent MUST surface it as a 🛑 blocker at the top of its report — not as a passing "warning" or "info" footnote. Per SPEC, lead time of any duration disqualifies the part; treat it like a hard fail, not a soft caveat.
- **Subagent prompts MUST cite SPEC.md and CLAUDE.md and require they're read first.** When spinning up a subagent for any task touching this project, the spawning prompt MUST include the absolute paths `/Users/ismail.ouazzani/code/dj/SPEC.md` and `/Users/ismail.ouazzani/code/dj/CLAUDE.md` and explicitly instruct the subagent to read both files in full before doing anything else. Don't assume the subagent will discover the project rules on its own — non-negotiable, every time.

## Parts research rules (Digikey)

**Sourcing constraint: Digikey Canada (digikey.ca) only, prices in CAD.** All v1 parts must be orderable through digikey.ca. The user is in Canada — quote CAD, link to digikey.ca product pages, and don't recommend off-platform alternatives (AliExpress, eBay, Adafruit-direct, etc.) even when cheaper or better-feeling. Many Adafruit/SparkFun breakouts *are* carried by Digikey under their own P/Ns; check before declaring something off-platform.

When pricing components for this project, agents should:

1. **Filter by "In Stock" before browsing.** Many DJ-grade audio pots/faders (ALPS RSA0N, RK09K) show up but are NRND/obsolete with 20+ week lead times. Filter early to avoid dead ends.
2. **Check whether Adafruit/SparkFun breakouts are on Digikey before treating them as off-platform.** Many are (e.g. SparkFun BOB-09056 CD4067 breakout is Digikey `1568-09056-ND`). Search by SparkFun BOB/COM/PRT or Adafruit PID numbers.
3. **For pot features (detent, taper), search by manufacturer-series suffix, not part number.** E.g. TT P160KN**PD** = center-detent; Same Sky PT01-D**xxx**-B**xxx** = linear B-taper. Confirm from the datasheet, not the Digikey filter.
