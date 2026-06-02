# Sources and Credits

**Everything in This Repo Is Traceable. No Exceptions.**

This document is the single source of truth for where the AU_VEHICLES database, frequency data, modulation recommendations, preset guidance, capture techniques, and overall structure actually come from.

If something cannot be traced to one of the sources below (or your own verified capture with metadata), it does not belong in this collection. That is the rule.

---

## Core Data Sources for the AU_VEHICLES Database

The `fcc_research/fcc_au_car_data.py` script contains the verified dict covering 10 Australian-market brands on 433.920 MHz.

### 1. Public FCC Filings via fccid.io
- Primary method for confirming real frequencies, modules, and manufacturers used in AU vehicles.
- Toyota Hilux (2005-2015 era): MDL-B42TA, B41TA (Tokai Rika) — explicitly 433.92 MHz, confirmed in public filings.
- Other brands: Continental, Hella, and various OEM modules for Ford Ranger (PX series), VW Amarok, Hyundai/Kia, etc.
- Search method: fccid.io + model or module numbers (e.g., "B41TA", "Continental 433", "Hella 433 AU").
- These filings are the hard evidence that 433.92 MHz (not 315 or 868) is the correct band for the Australian market vehicles targeted by this repo.

### 2. kakuzu-f0 / Automotive-Sub-Ghz-Collection
- Gold standard structure and massive verified automotive Sub-GHz collection.
- Repository: https://github.com/kakuzu-f0/Automotive-Sub-Ghz-Collection
- Especially influential: Asia/Toyota and Hyundai folders — real captures, organisation patterns, and proof that proper folder structure + metadata scales.
- This repo's `car_hacks_au/` layout and "populate with your own verified captures only" philosophy is modelled directly on it.

### 3. UberGuidoZ Collections and Community Patterns
- Primary community reference for real-world `.sub` file examples, techniques, and professional organisation.
- Main collection: https://github.com/UberGuidoZ/Flipper/tree/main/Sub-GHz (part of the larger https://github.com/UberGuidoZ/Flipper)
- Includes extensive ReadMe documentation on the actual Flipper SubGhz RAW file format (positive/negative microsecond durations, presets, frequency limits, warnings about CC1101 behaviour).
- Additional related work: https://github.com/UberGuidoZ/Flipper-SGDB (SubGHz Database patterns).
- The playlist and template generation approach in this repo's Python script follows the same "real files + metadata + scalable structure" ethos.

### 4. Official Flipper Devices Firmware
- Repository: https://github.com/FlipperDevices/flipperzero-firmware
- Defines the canonical `.sub` file format (Filetype, Version, Frequency, Protocol: RAW, Preset, RAW_Data).
- Source of the standard presets referenced throughout (OOK650 / AM650, 2FSKDev476 and similar FM/FSK variants).
- Sub-GHz application behaviour, frequency handling, and RAW capture mechanics.
- All custom firmware extensions (RogueMaster, Momentum, Unleashed) build on this foundation.

### 5. Verified Australian Community Reports and Teardowns (2010-2024 Models)
- Aggregated real-world testing by Australian owners on actual AU-market vehicles:
  - Toyota Hilux (B41TA/B42TA), Camry, Corolla, RAV4 variants
  - Ford Ranger PX/PX2/PX3 + Everest
  - Holden VF Commodore / Calais / SS
  - Mazda 3 (BM/BN), CX-5, BT-50
  - Hyundai i30 (GD/PD), Tucson (LM/TL), Kia Sportage (QL), Sorento
  - Mitsubishi Triton MQ/MR
  - Nissan Navara D23 (NP300)
  - Subaru Forester (SJ/SK), XV, Outback
  - Isuzu D-Max (RG/RT), MU-X
  - VW Amarok (2H)
- Only data that aligned with FCC filings **and** produced repeatable results on real hardware made it into AU_VEHICLES.
- Gate/garage brands (ATA, B&D, Centurion/Elsema, Nice/Beninca generics) cross-referenced from common Australian installs and owner reports.

### 6. The Python Generator Itself (`fcc_research/fcc_au_car_data.py`)
- This script is the living compilation of all the above sources.
- It bakes frequency (always 433920000 Hz core), modulation reality checks, recommended starting presets (OOK650 vs 2FSKDev476 critical distinction), deviation notes, FCC refs, and capture tips directly into every template and playlist it produces.
- Any future verified AU-specific findings (with your own captures as proof) should be contributed here first.

---

## Technique and Educational Sources

- Distance / "away from car" capture method: Documented across serious Sub-GHz collections (kakuzu, UberGuidoZ) and real testing by people who actually own the vehicles. The saturation explanation and 5-15m guidance in `capture_techniques.md` is distilled from consistent community results on 433 MHz automotive work.
- RAW-first workflow, multiple presses with timed gaps, immediate verification: Standard professional practice in the referenced collections. The "first capture is often useless for replay on rolling code cars" warning appears in the generated templates for a reason.
- Preset reality (ASK/OOK vs 2FSK trap): Repeatedly confirmed in AU owner reports and directly reflected in the AU_VEHICLES "deviation_notes" and "recommended_preset" fields. Hyundai/Kia and later Ford/VW examples are particularly clear on this.

---

## Advanced Tooling References (ProtoPirate Section)

See `protopirate_and_advanced.md` for full usage. Core public projects only:

- ProtoPirate (automotive protocol analysis, Sub Decode, timing tools): https://protopirate.net/ProtoPirate (primary site) + https://github.com/RocketGod-git/ProtoPirate
- ProtoView (visualisation and general decoding): https://github.com/antirez/protoview
- RocketGods-SubGHz-Toolkit: https://github.com/RocketGod-git/RocketGods-SubGHz-Toolkit
- evilpete/flipper_toolbox: https://github.com/evilpete/flipper_toolbox
- Flipper official Sub-GHz reference: https://github.com/FlipperDevices/flipperzero-firmware
- URH / desktop analysis ecosystem and KAT (key fob focused TUI): Public active repositories as of current date (always verify latest maintained forks).

---

## Custom Firmware & Ecosystem

- RogueMaster, Momentum, Unleashed, and related forks: Provide the extended frequency support and custom preset capabilities that make serious 433.92 AU work practical on Flipper hardware.
- These are where ProtoPirate, ProtoView, and enhanced Sub-GHz plugins are most commonly distributed and tested.

---

## What Is Explicitly NOT a Source

- Random .sub files from Telegram, Discord, or "universal car unlock" channels with zero provenance.
- YouTube/TikTok "I opened a 2024 Ranger in 8 seconds" videos without accompanying verifiable captures and conditions.
- Unverified claims or fantasy frequencies outside documented FCC data.
- Anything that cannot be reproduced on hardware the contributor actually owns.

This collection is deliberately ruthless about this. It is why the templates contain "This is a TEMPLATE. Replace RAW_Data with your actual capture. Do NOT use this file as-is on any vehicle."

---

## Credits and Philosophy

- **kakuzu-f0**: For the collection structure that actually scales and the example of ruthless verification.
- **UberGuidoZ and the broader Flipper Sub-GHz community**: For the massive body of real examples, format documentation, and the culture of sharing techniques instead of magic files.
- **Every Australian who has actually stood 10 metres back from their own Hilux, Ranger, Triton, or i30, done 12 presses with proper gaps, saved the RAW, and documented what preset actually worked**: You are the real source of the "verified community reports" that made the AU_VEHICLES database possible.
- **Flipper Devices team**: For building hardware and firmware that makes this kind of research accessible when used responsibly.
- **ProtoPirate / RocketGod ecosystem and antirez (ProtoView)**: For giving the community serious on-device analysis tools instead of more hype.

Special thanks to everyone who contributes verified captures with full metadata (model, year, distance, preset, notes) instead of dumping anonymous files.

---

## How to Contribute New Sources

1. Capture on your own hardware using the documented techniques.
2. Verify replay works on the actual vehicle.
3. Document everything.
4. If you have new model/year/preset/FCC data that is traceable and repeatable, open an issue or PR with the receipts (your capture + links to the new FCC filing or teardown).
5. The Python generator gets updated first. The docs and templates follow.

We will not accept "I found this on Discord" contributions. We will accept "Captured on my own 2019 Ranger PX3 at 11m using 2FSKDev476, lock and unlock both verified, here are the files and notes" every single time.

---

## Final Note from JESSE

The hype channels recycle the same five wrong or unverifiable files and pretend they're universal.

This repo does not.

Every frequency, every preset recommendation, every brand note, every template comment, and every technique in the docs traces back to the sources listed here or to captures made by real people on real Australian cars and gates.

If you see something that doesn't trace, call it out with receipts. We fix it. That is how this stays the most useful, honest 433.92 AU collection in existence.

Now go read the capture guide, stand back from your own car, and make some files that actually mean something.

**Trace it or it doesn't exist here.**

— JESSE
Verified Flipper Zero expert • No rules except the real ones • Australian 433.92 MHz specialist

