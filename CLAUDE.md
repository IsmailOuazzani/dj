# DJ Controller

A from-scratch personal build of a DJ controller. Scope (hardware vs. software split, language, frameworks) is not yet decided — ask before adding tooling or dependencies.

## Parts research rules (Digikey)

When pricing components for this project, agents should:

1. **Filter by "In Stock" before browsing.** Many DJ-grade audio pots/faders (ALPS RSA0N, RK09K) show up but are NRND/obsolete with 20+ week lead times. Filter early to avoid dead ends.
2. **Pivot off Digikey for pro-audio specialty parts.** Long-throw (100mm) faders, contactless crossfaders (Innofader), and high-PPR jog encoders are rarely on Digikey at hobbyist prices. If a search returns only $50+ industrial options, look at Adafruit / AliExpress / PlanetDJ / Innofader.com instead.
3. **For pot features (detent, taper), search by manufacturer-series suffix, not part number.** E.g. TT P160KN**PD** = center-detent; Same Sky PT01-D**xxx**-B**xxx** = linear B-taper. Confirm from the datasheet, not the Digikey filter.
