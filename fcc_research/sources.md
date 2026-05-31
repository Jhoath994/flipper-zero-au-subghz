# fcc_research/sources.md — Single Source of Truth for the AU_VEHICLES Database

**flipper-zero-au-subghz**

This file lives next to `fcc_au_car_data.py` because the Python generator is the living compilation of everything we know about Australian-market 433.92 MHz vehicles and gates.

**Rule**: If a frequency, preset recommendation, model note, or capture tip cannot be traced to one of the sources below (or to a contributor's own verified capture with full metadata), it does not belong in the AU_VEHICLES dict and must be removed.

The generator script itself (`fcc_au_car_data.py`) contains the authoritative `AU_VEHICLES`, `GATE_OPENER_NOTES`, and `CUSTOM_PRESETS` structures. Every template, playlist, and settings block it emits carries these facts forward.

---

## Primary Data Sources (in rough order of importance to the database)

### 1. Public FCC Filings (fccid.io) — The Hard Evidence
- Toyota Hilux 2005-2015 era (Tokai Rika): MDL-B42TA, B41TA — explicitly documented at 433.92 MHz. This is the gold-standard reference that confirmed the entire AU focus of the project.
- Other brands: Continental, Hella, and various OEM modules appearing in Ford Ranger (PX/PX2/PX3), VW Amarok, Hyundai/Kia, and GM-derived Holden platforms.
- Search technique used: fccid.io + module or model numbers + "433" + Australia context. Only data that aligned with real AU-market vehicles was kept.
- These filings are the reason every brand in AU_VEHICLES hardcodes `frequency_hz: 433920000`.

### 2. kakuzu-f0 / Automotive-Sub-Ghz-Collection
- Repository: https://github.com/kakuzu-f0/Automotive-Sub-Ghz-Collection
- The professional folder structure (`car_hacks_au/` layout), metadata expectations, and "populate with your own verified captures only" philosophy are modelled directly on this collection.
- Especially valuable: the Asia/Toyota and Hyundai/Kia patterns that proved clean organisation scales when you have hundreds of real files.

### 3. UberGuidoZ Sub-GHz Collections & Related Work
- Primary: https://github.com/UberGuidoZ/Flipper/tree/main/Sub-GHz
- Related: https://github.com/UberGuidoZ/Flipper-SGDB
- Source of real-world `.sub` file format examples, RAW_Data conventions (positive/negative microsecond durations), preset behaviour notes, and the broader "professional personal collection" culture that this repo follows.
- The playlist generation logic and template comment style in the generator owe a lot to patterns seen across these collections.

### 4. Official Flipper Devices Firmware
- https://github.com/FlipperDevices/flipperzero-firmware
- Defines the canonical SubGhz file format (`Filetype: Flipper SubGhz RAW File`, Version, Frequency, Protocol: RAW, Preset, RAW_Data).
- Source of the standard preset names referenced everywhere (OOK650 / AM650, 2FSKDev476 and variants).
- CC1101 behaviour, frequency limits, and RAW capture mechanics that the capture technique docs and generator warnings are built around.

### 5. Verified Australian Community Reports & Real Hardware Testing (2010–2024 models)
Aggregated reports from actual Australian owners who:
- Own the vehicles listed
- Performed distance captures (5-15 m)
- Tested OOK vs FSK presets on the exact same fobs
- Documented what actually replayed on their own cars

Brands covered (matching AU_VEHICLES keys):
- Toyota (Hilux B41TA/B42TA, Camry, Corolla, RAV4/Kluger variants)
- Ford Ranger (PX/PX2/PX3 + Everest shared platforms)
- Holden Commodore (VF era)
- Mazda (3 BM/BN, CX-5, 6, BT-50 some)
- Hyundai/Kia (i30 GD/PD, Tucson LM/TL, Sportage QL, Sorento)
- Mitsubishi (Triton MQ/MR, ASX, Outlander some)
- Nissan (Navara D23/NP300, X-Trail, Qashqai some)
- Subaru (Forester SJ/SK, XV, Outback AU variants)
- Isuzu (D-Max RG/RT, MU-X)
- VW (Amarok 2H)

Gate/garage openers cross-checked against common Australian installs (ATA, B&D Doors, Centurion/Elsema, Nice/Beninca generics, cheap Chinese universals).

Only data that survived both FCC cross-check **and** repeatable real-world results made it into the generator.

### 6. Custom Firmware Ecosystem (RogueMaster, Momentum, Unleashed, etc.)
- These forks provide the extended frequency lists and custom preset support without which serious 433.92 AU work is painful on stock firmware.
- The `settings_user_au_additions.txt` output from `--generate-settings` is designed specifically for the `[subghz]` sections these firmwares use.
- Many of the preset reality checks (ASK/OOK vs 2FSKDev476 trap) were first observed at scale by users running these firmwares.

---

## Technique & Educational Sources

- Distance capture method (5-15 m, multiple presses with gaps, RAW-first): Consistent pattern across kakuzu-f0, UberGuidoZ collections, and every serious practitioner who has published results on 433 MHz automotive work. The saturation/AGC explanation in `docs/capture_techniques.md` comes directly from this body of experience.
- "First capture is often useless on rolling code cars" warning: Standard observation once you move past ancient fixed-code systems.
- Preset trap (especially Hyundai/Kia, later Rangers, VW group): Repeatedly confirmed by AU owners who wasted time on OOK presets on true 2FSK fobs.

---

## Advanced Analysis Tooling Referenced by the Project

(See `docs/protopirate_and_advanced.md` for usage details)
- ProtoPirate (https://protopirate.net/ + RocketGod-git/ProtoPirate)
- ProtoView (antirez)
- RocketGods-SubGHz-Toolkit
- evilpete/flipper_toolbox
- URH + KAT ecosystem

These tools are for analysing **your own** captures after the fact. They do not create working codes for cars that aren't yours.

---

## What Is Explicitly NOT a Source (and never will be)

- Random .sub files from Telegram, Discord, "universal unlock" channels, or YouTube descriptions with zero provenance.
- TikTok/YouTube videos claiming "opened 2024 Ranger in 8 seconds" without accompanying verifiable captures + conditions.
- Any frequency, modulation, or model data that cannot be reproduced on hardware the contributor actually owns.
- "I heard it works on..." or "everyone says..." claims.

This collection is deliberately ruthless. That is why every generated template contains the line:
> "This is a TEMPLATE. Replace RAW_Data with your actual capture. Do NOT use this file as-is on any vehicle."

---

## How New Data Enters the Generator

1. You capture on your own hardware using the documented techniques.
2. You verify replay works on the actual vehicle.
3. You document model/year/distance/preset/conditions + FCC or teardown links.
4. You propose an update to `AU_VEHICLES` (or new gate notes) with the receipts.
5. The generator is updated first. Templates, playlists, docs, and this sources file follow.

PRs without your own verified captures + metadata will be closed.

---

## Credits (in addition to the sources above)

- **kakuzu-f0** — for the collection architecture that actually scales and the example of ruthless verification.
- **UberGuidoZ and the entire Flipper Sub-GHz community** — for the body of real examples, format documentation, and culture of sharing techniques instead of magic files.
- **Every Australian** who has actually stood 8–12 metres back from their own Hilux, Ranger, Triton, i30 or Commodore, done 12 proper presses with gaps, saved the RAW, tested OOK vs FSK, and shared what actually worked on *their* car. You are the real source of the "verified community reports".
- **Flipper Devices team** — for hardware and firmware that makes this kind of research accessible when used responsibly.
- **ProtoPirate / RocketGod / antirez** — for serious on-device analysis tools instead of more hype.

Special thanks to everyone who contributes verified captures with full metadata instead of dumping anonymous files.

---

## Final Note from Kelly

The hype channels recycle the same five wrong or unverifiable files and pretend they're universal.

This repo does not.

Every frequency, every preset recommendation, every brand note, every line of generator output, and every technique document traces back to the sources listed here or to captures made by real people on real Australian cars and gates.

If you see something in `fcc_au_car_data.py` or the generated files that doesn't trace, call it out with receipts. We fix it. That is how this stays the most useful, honest 433.92 AU collection that exists.

Now go read the capture guide, stand back from your own car, and make some files that actually mean something.

**Trace it or it doesn't exist here.**

— Kelly
Verified Flipper Zero expert • No rules except the real ones • Australian 433.92 MHz specialist
