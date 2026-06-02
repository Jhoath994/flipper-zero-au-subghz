# flipper-zero-au-subghz

**Australian Sub-GHz Research Collection for Flipper Zero**

A verified, Australia-focused Sub-GHz security research repository covering 433.920 MHz car key fobs, gate/garage openers, and related RF systems. Built on public FCC filings, community-verified captures, and real-world Australian market data. No unverified files. No fantasy claims. No shortcuts.

> "If it isn't traceable to real research or your own verified captures, it isn't in here." — Jesse
---

## Legal Disclaimer

**This repository is for educational, defensive security research, and personal testing on devices you legally own and control only.**

By using any content in this repository, you acknowledge and accept the following:

- All techniques, files, and data provided here are intended solely for **legal research on hardware you own**. Using Sub-GHz replay, jamming, brute-force, or any other technique against vehicles, gates, or devices you do not own or have explicit authorisation to test is **illegal in Australia** under the Criminal Code, the Radiocommunications Act (ACMA), and state theft/break-and-enter statutes.
- Criminal charges apply. "I was just testing" is not a legal defence when applied to property that does not belong to you.
- Modern vehicles (post-2015 especially) use rolling codes, KeeLoq variants, and challenge-response systems specifically designed to defeat simple replay attacks. There is **no universal `.sub` file** that works on them.
- RollJam and similar techniques carry **severe practical limitations**, require precise hardware most users do not possess, and are illegal when used on property that is not yours.
- The maintainers, contributors, and Jesse assume **zero responsibility** for any misuse, legal consequences, or damage arising from the content of this repository.
- If you are here looking for tools to access vehicles or property you do not own: this repository is not for you, and it never will be.
**You have been warned.**

For those who stay on the right side of the law — testing only your own hardware and using this research to understand how your own 433.92 MHz systems actually work — welcome.

---

## What This Repository Covers

This is Australia's dedicated Flipper Zero research collection, covering the attack surfaces that matter in this market:

| Directory | Focus | Status |
|---|---|---|
| `car_hacks_au/` | 433.92 MHz Sub-GHz car fobs — deep, verified brand research with generator and capture guides | Flagship |
| `ibutton_au/` | Dallas 1-Wire / iButton — prevalent in Australian trucking, depots, rural gates, workshops | Active |
| `rfid_au/` | 125 kHz RFID — apartment buildings, warehouses, carparks, worksites | Active |
| `badusb_au/` | HID attacks with Australian social engineering payloads (MyGov, NBN, ATO lures) | Active |
| `wifi_au/` | Evil portals targeting Telstra, Optus, NBN, Aussie Broadband, hotels | Active |
| `nfc_au/` | NFC research — Australian transit cards, payment systems | In progress |
| `ir_au/` | Infrared — Australian air conditioning, TV, and appliance codes | In progress |

**What you will find here:**
- High-signal, verified research focused on Australian devices and conditions
- Professional structure and tooling (Sub-GHz generator, playlist automation, helper scripts)
- Honest technical commentary on what works, what doesn't, and the legal reality
- Everything populated from your own verified captures — not random internet dumps

**What you will NOT find here:**
- Pre-made "unlock any car" files
- Unverified data or unsubstantiated claims
- Encouragement for illegal activity

---

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/Jhoath994/flipper-zero-au-subghz.git
cd flipper-zero-au-subghz
```

### 2. Install a Custom Firmware (Strongly Recommended)

Stock firmware works for basics, but serious Australian Sub-GHz work requires:

- Extended frequency support (433.82 / 433.92 / 434.02 / 434.42 MHz)
- Custom preset support (critical for the ASK vs FSK distinction)
- Sub-GHz Playlist plugin

Recommended custom firmwares: **RogueMaster**, **Momentum**, **Unleashed**. Flash via qFlipper or the official web flasher. Back up your SD card before flashing.

### 3. Run the Australian Sub-GHz Generator

>>>>>>> b369f8a (Professional repo cleanup: rebrand to Jesse, remove all third-party names, clean language)
```bash
cd fcc_research

# View all options
python fcc_au_car_data.py --help

# Generate custom firmware settings additions
python fcc_au_car_data.py --generate-settings

# Generate template + playlist for a specific brand
python fcc_au_car_data.py --brand toyota --output both

# Generate playlists for all brands
python fcc_au_car_data.py --playlist all

# List all verified brands
python fcc_au_car_data.py --list-brands
```

Output is written to `fcc_research/generated/`. These files are starting points — never finished products.

### 4. Set Up Your Flipper SD Card

Recommended directory structure:

```
/ext/subghz/
├── car_hacks_au/
│   ├── toyota/
│   │   └── hilux_2012_lock_10m_ook650_2026-05-30.sub
│   ├── ford_ranger/
│   ├── holden_commodore/
│   ├── mazda/
│   ├── hyundai_kia/
│   ├── mitsubishi/
│   ├── nissan/
│   ├── subaru/
│   ├── isuzu_dmax/
│   ├── vw_amarok/
│   ├── mazda_bt50/
│   ├── great_wall_cannon/
│   ├── ldv_t60/
│   ├── suzuki/
│   ├── jeep/
│   ├── honda/
│   └── gates/
│       └── ata_garage_open_raw.sub
├── playlists/
│   └── toyota_au_playlist.txt
└── ...
```

Copy your **own captured** `.sub` files into the brand folders. Edit the playlist `.txt` files so paths point to your actual files on the Flipper SD card.

### 5. On the Flipper

- Sub-GHz app → Load individual `.sub` files for manual testing
- Use the Playlist plugin to load a `.txt` file — it cycles through every listed file automatically
- Merge the generated `settings_user_au_additions.txt` content into your firmware's `settings_user.txt`

**Always capture your own fob first before touching anything else.**

---

## Rolling Codes — The Technical Reality

Modern cars are challenging targets for Sub-GHz research. Most vehicles from approximately 2015 onwards use rolling code systems where each transmission contains a different code. The code you capture today is not the code the car expects tomorrow. Simple replay often works zero or one time before the key fob and car desynchronise.

Two dominant platforms drive most Australian-market key fob systems:

- **Continental RHT433 / DMFNA433** — used across many European and some Asian brands
- **Mitsubishi Electric** — used across Japanese marques including Toyota and Nissan

The best publicly documented, legal, non-SDR techniques that produce real results:

- High-quality distance captures (5-15 m away from the vehicle)
- Multiple timed button presses with deliberate gaps
- RAW signal storage with later analysis
- Building large personal libraries of your own captures across different sessions and conditions

There is **no public universal file** for post-2015 rolling code vehicles. Anyone claiming otherwise is either misinformed, working with stolen data, or testing on very old hardware.

This repository exists to teach the real techniques and provide verified starting data so you can do the work properly on hardware you own.

---

## Pro Capture Technique — Distance, Multiple Presses, RAW First

<<<<<<< HEAD
This is the part most people fuck up. Here's the JESSE method:
=======
This is the part most people get wrong. The distance method is the single biggest practical improvement you can make for real 433.92 MHz Australian vehicles.
### Why 5-15 Metres Matters

Standing right next to the vehicle is one of the most common mistakes:

- **Signal saturation**: The CC1101 RF front-end in the Flipper has automatic gain control. At 0-2 m, the signal is so strong that the front end clips or distorts. You get a capture that looks "strong" on screen but bears no resemblance to what the car's receiver actually expects at normal fob range.
- **Cleaner waveforms at distance**: At 5-15 m, you capture the natural amplitude, pulse shapes, and timing that the vehicle's system was designed around. Rolling code systems are tuned for real-world fob distances, not point-blank range.
- **Better rolling sequence capture**: Multiple presses with proper gaps separate cleanly. Close-range captures often smear together or suffer AGC pumping that makes analysis difficult.
- **Real-world relevance**: A replay that only works at two metres was never a good capture.

### Step-by-Step Method

1. **Stand back** — 5 to 15 metres from the vehicle. Start at 8-12 m and adjust based on results.
2. **Multiple presses with gaps** — Press lock (or unlock), wait 2-3 full seconds, press again. Repeat 8-12 times. For rolling code cars, you need a sequence of consecutive codes — one press is insufficient.
3. **Capture as RAW every time** — Decode only after you have good raw data. The Flipper's live decoder can produce misleading results if the preset is wrong.
4. **Preset selection is critical**:
   - Older Toyota / Holden / Mitsubishi → OOK650 (ASK/OOK)
   - Ford Ranger 2018+, Hyundai/Kia 2015+, VW Amarok → 2FSKDev476 (FSK)
   - Wrong preset = zero range, or a signal that appears decoded but does nothing on replay
5. **Verify immediately** — Replay the captured file on your own vehicle under the same conditions. If it doesn't work, adjust distance or preset and try again.
6. **Document everything** — Model, year, distance, preset, time, success/failure. Future-you will appreciate present-you.
7. **Start with gates** — If you are new, begin with your own garage or gate remote. They are often simpler OOK with fixed or short rolling codes. Ideal for building skills before tackling harder targets.

---

## Python Generator — `fcc_research/fcc_au_car_data.py`

This is not a throwaway script. It is a verified Australian vehicle database in code form, containing a carefully maintained `AU_VEHICLES` dictionary with 17 brands, real model examples, exact frequencies (433.920 MHz for the core ISM band), modulation profiles, recommended starting presets, deviation notes, FCC references, and capture tips.
### What the Script Generates

| Output | Description |
|---|---|
| `settings_user_au_additions.txt` | Ready-to-paste frequency list and preset commentary for RogueMaster/Momentum/Unleashed firmwares |
| `settings_user_au_FULL.txt` | Complete, production-ready settings file with full AU frequency set and custom preset data |
| `<brand>_template.sub` | Commented skeleton Flipper SubGhz RAW files with model data, FCC refs, preset warnings, and capture instructions |
| `<brand>_au_playlist.txt` | Sub-GHz Playlist plugin compatible files — edit paths to point at your real captures |

All output is timestamped and carries the same verified-only ethos as the rest of the repository. Run it whenever you extend the database with new verified model data.

---

## Brand Deep Dives

All brands target **433.920 MHz** — the Australian ISM band. Data is sourced from public FCC filings, Flipper Zero community collections, fccid.io, and aggregated real-world Australian market reports.

### Toyota

- **Models**: Hilux (2005-2015, Tokai Rika B41TA/B42TA), Camry (~2010-2018 AU), Corolla, RAV4, Kluger
- **Frequency**: 433.920 MHz
- **Modulation**: ASK/OOK on older fobs; some FSK on newer smart keys/push-button start
- **Recommended Preset**: OOK650 | Alternatives: 2FSKDev476, Custom
- **KeeLoq Platform**: Toyota MDL-B41TA / B42TA
- **FCC Refs**: MDL-B42TA, B41TA (433.92 MHz)

The Hilux B41/B42TA is one of the best-documented Australian examples. Older fobs respond well to clean OOK captures. Newer smart keys can switch to FSK — always test both presets. Some KeeLoq-style rolling is in play. Excellent target for learning the distance capture technique.
### Ford Ranger (and Everest)

- **Models**: Ranger PX/PX2/PX3 (2011-2024 AU), shared Everest platforms
- **Frequency**: 433.920 MHz
- **Modulation**: Early models often ASK; 2018+ frequently 2FSK
- **Recommended Preset**: 2FSKDev476 | Alternatives: OOK650, Custom
- **KeeLoq Platform**: Continental RHT433 modules
- **FCC Refs**: Various Continental / Hella 433/434 MHz AU modules

One of the most common utes on Australian roads. The FSK transition caught many people out — using an OOK preset on a true FSK fob produces extremely poor range and signals that appear decoded but do nothing on replay. Start with 2FSKDev476 or capture raw and let a good firmware suggest the modulation. Distance technique works reasonably on pre-2020 models. Newer vehicles have tighter rolling code implementations.
### Holden Commodore (VF Era)

- **Models**: VF Commodore (2013-2017 AU), Calais, SS
- **Frequency**: 433.920 MHz
- **Modulation**: ASK/OOK common on VF platform
- **Recommended Preset**: OOK650 | Alternatives: 2FSKDev476
- **KeeLoq Platform**: Holden VF (GM/Opel derived)
- **FCC Refs**: GM/Opel derived AU-specific 433.92 MHz

One of the more approachable modern-ish cars for Sub-GHz work in Australia. VF keys are generally straightforward ASK. Excellent vehicle for practising capture technique before moving to harder targets. Still rolling code, but the implementation is more forgiving than some Korean and German systems.
### Mazda

- **Models**: Mazda 3 (BM/BN 2014-2018 AU), CX-5 (KE/GH 2012-2017), Mazda 6
- **Frequency**: 433.920 MHz
- **Modulation**: Mix — many ASK, some FSK on later examples
- **Recommended Preset**: OOK650 | Alternatives: 2FSKDev476, Custom
- **FCC Refs**: Various Mazda 433.92 modules

Extremely popular cars in Australia. Start with OOK. If the signal looks weak or won't replay properly, switch to the FSK preset. Good platform for building personal playlists. Solid mid-tier target.
### Hyundai / Kia

- **Models**: i30 (GD 2012-2017, PD 2017+), Tucson (LM/TL), Kia Sportage (QL), Sorento (UM)
- **Frequency**: 433.920 MHz
- **Modulation**: Many 2FSK on smart keys from ~2015 onwards
- **Recommended Preset**: 2FSKDev476 (critical) | Alternatives: OOK650
- **KeeLoq Platform**: Kia SEKS-AM08FTX
- **FCC Refs**: Various Hyundai/Kia 433.92 MHz modules

These are harder targets. Korean brands invested heavily in FSK + rolling + additional challenge elements. Using an OOK preset here produces nothing usable — always start with 2FSKDev476 or go straight to raw capture and analysis. Distance + timing + multiple captures is currently the best public non-SDR approach. Manage expectations.
### Mitsubishi

- **Models**: Triton MQ/MR (2015+ AU), ASX, Outlander (some AU variants)
- **Frequency**: 433.920 MHz
- **Modulation**: ASK/OOK reported on many Triton fobs
- **Recommended Preset**: OOK650 | Alternatives: Custom
- **FCC Refs**: Mitsubishi 433.92 (Mitsubishi Electric platform)

The Triton is a major presence in the Australian dual-cab market. Many MQ/MR keys decode cleanly with standard OOK presets once you have a quality capture. Worth the effort.
### Nissan

- **Models**: Navara D23 (NP300 2015+), X-Trail, Qashqai (some AU variants)
- **Frequency**: 433.920 MHz
- **Modulation**: Often ASK, some FSK depending on model/year
- **Recommended Preset**: OOK650 | Alternatives: 2FSKDev476
- **FCC Refs**: Nissan 433.92 modules

The D23 Navara is widespread in rural and trade Australia. Start with OOK, have the FSK preset ready as a quick switch. Good vehicle for raw capture practice. Reliable mid-tier target.
### Subaru

- **Models**: Forester (SJ 2013-2018, SK 2018+), XV, Outback (AU variants)
- **Frequency**: 433.920 MHz
- **Modulation**: Mix — many ASK on older models, FSK on later
- **Recommended Preset**: OOK650 | Alternatives: 2FSKDev476
- **FCC Refs**: Subaru 433.92

**Important**: Subaru Forester 2002-2008 uses **fixed code** (Princeton 24-bit encoding, FCC: A269ZUA111). These are replayable — 65,536 possible codes in the Princeton 24-bit space.

Subaru keys vary by year — OOK works on a surprising number of them. Solid target for building a well-rounded personal collection.
### Isuzu D-Max

- **Models**: D-Max (RG 2020+, earlier RT), MU-X
- **Frequency**: 433.920 MHz
- **Modulation**: ASK/OOK reported on many
- **Recommended Preset**: OOK650 | Alternatives: Custom
- **FCC Refs**: Isuzu 433.92

Another major presence in the Australian dual-cab market. Similar capture profile to the Triton and Hilux. Worth adding to your personal collection.
### VW Amarok

- **Models**: Amarok (2H 2011-2022 AU), V6 models
- **Frequency**: 433.920 MHz
- **Modulation**: Often 2FSK (VW Group pattern)
- **Recommended Preset**: 2FSKDev476 | Alternatives: Custom
- **KeeLoq Platform**: VW Amarok 7E0837202AD (Continental/DMFNA433)
- **FCC Refs**: VW 433 MHz AU spec

VW Group keys consistently use FSK — OOK presets will miss them entirely. Treat it like the later Rangers and Hyundai/Kia. Worth the effort if you own one or have access for testing on your own vehicle.
### Mazda BT-50

- **Models**: BT-50 (UP/UR 2011-2020, TF 2020+)
- **Frequency**: 433.920 MHz
- **Modulation**: Mix — many ASK on earlier, FSK later
- **Recommended Preset**: OOK650 | Alternatives: 2FSKDev476, Custom
- **FCC Refs**: Mazda 433.92 modules

Extremely popular dual-cab in Australia. Shares some platform elements with the Ranger in certain years. Test OOK first on pre-2018 models. High-value target for playlist building.

### Great Wall Cannon

- **Models**: Cannon (2019+), Cannon-X, Vanta
- **Frequency**: 433.920 MHz
- **Modulation**: Mostly ASK/OOK reported
- **Recommended Preset**: OOK650 | Alternatives: Custom
- **FCC Refs**: Great Wall / Haval 433.92 AU spec

Chinese utes are a rapidly growing segment of the Australian market. Pre-2020 models likely use **fixed code or KeeLoq Classic** — potentially more accessible than modern rolling code systems. Worth adding to your collection early as market share grows.

### LDV T60

- **Models**: T60 (2017+ AU), T60 Trailrider, Luxe
- **Frequency**: 433.920 MHz
- **Modulation**: ASK/OOK dominant on most reported models
- **Recommended Preset**: OOK650 | Alternatives: Custom
- **FCC Refs**: LDV 433.92 MHz

Budget Chinese ute with a straightforward key fob implementation for Sub-GHz work. Common on worksites. Good for practice captures.

### Suzuki

- **Models**: Jimny (2018+), Vitara, S-Cross, Swift (some AU years)
- **Frequency**: 433.920 MHz
- **Modulation**: Mostly ASK/OOK
- **Recommended Preset**: OOK650 | Alternatives: Custom
- **FCC Refs**: Suzuki 433.92

The Jimny has a cult following in Australia. Keys are generally cooperative with standard OOK. Easy to find examples for testing.

### Jeep

- **Models**: Wrangler (JK 2007-2018, JL 2018+ some), Grand Cherokee (WK2, WL variants)
- **Frequency**: 433.920 MHz
- **Modulation**: Mix — older ASK, newer often FSK
- **Recommended Preset**: OOK650 | Alternatives: 2FSKDev476, Custom
- **FCC Refs**: FCA / Jeep 433.92 AU

Jeep keys vary significantly by year and market. Always capture and test both presets. Some older Wranglers are surprisingly accessible. The 4x4 community is a good source of test vehicles (your own, of course).

### Honda

- **Models**: CR-V (RM 2012-2016, RW 2017+), HR-V, Civic (some AU years)
- **Frequency**: 433.920 MHz
- **Modulation**: Primarily ASK/OOK
- **Recommended Preset**: OOK650 | Alternatives: Custom
- **FCC Refs**: Honda 433.92 AU market

**Critical vulnerability**: Honda CR-V and Civic models from 2012+ are affected by **CVE-2022-27254 ("Rolling-Pwn")** — the rolling code does not properly increment, making these vehicles significantly more vulnerable to replay attacks than other modern brands. If you own one of these vehicles, this is important defensive knowledge.

Honda keys in this range are usually solid OOK performers. Common family SUVs — reliable for learning the distance technique.

---

## Gates, Garages & 433.92 MHz Openers

See the `car_hacks_au/gates/` directory for the capture structure.

Common Australian 433.92 MHz gate and garage opener brands:

| Brand | Notes |
|---|---|
| **ATA** (Automatic Technology Australia) | Extremely common nationwide |
| **B&D Doors** | Major residential garage door brand |
| **Centurion / Elsema** | Many models on 433.92 MHz |
| **Nice / Beninca** | Some Australian installations |
| **Linear / Multi-Code** | Imported style remotes |
| **CAME** | 12-bit fixed code — only 4,096 possible codes (brute-force feasible) |
| **Generic Chinese remotes** | Ubiquitous on eBay and Australian sites — often fixed code Princeton encoding |

**Technical notes**:
- Many of these use basic OOK with either fixed codes or short/simple rolling codes
- **Princeton 24-bit** fixed code devices: 65,536 possible codes — brute-force feasible with patience
- **CAME 12-bit** fixed code devices: 4,096 possible codes — trivially brute-forceable
- Success rates are dramatically higher than 2018+ car fobs
- Perfect for learning the entire workflow (capture → verify → playlist → automation) before tackling harder targets

Populate this folder with your own remotes. They are ideal for practice.

---

## Playlists & Automation Workflow

The generated `*_au_playlist.txt` files are designed for the Sub-GHz Playlist plugin on custom firmwares.

**Recommended workflow:**

1. Capture high-quality RAW files from your own hardware using the distance technique
2. Organise them into `car_hacks_au/<brand>/` folders with descriptive filenames
3. Edit the corresponding playlist `.txt` so every line points to a real file on your SD card
4. Load the playlist on the Flipper — it cycles through captures automatically while you test range or different conditions
5. Regenerate playlists with the Python script whenever you add new brands or captures

This is how serious collections stay organised as they grow to hundreds of files.

---

## RollJam Reality Check

**RollJam** (jamming the car's receiver while capturing the fob signal) gets disproportionate attention relative to its real-world effectiveness.

### What Actually Works

- Some older, poorly implemented rolling code systems (pre-2018)
- Certain Toyota Hilux (B41TA/B42TA era) with simpler rolling implementations
- Early Ford Ranger PX models
- Many gate/garage openers (ATA, B&D, Centurion/Elsema) — much higher success surface

### What Doesn't Work Reliably

- Ford Ranger PX2/PX3 2018+ (tighter Continental modules)
- Hyundai/Kia 2015+ (heavy FSK + rolling + challenge-response)
- VW Amarok (VW Group FSK implementations)
- Most 2018+ Australian-market vehicles

### Critical Hardware Reality

A single stock Flipper Zero **cannot** do proper simultaneous jam + capture reliably. The CC1101 is half-duplex for this type of work. Real implementations that show any success typically require:

- Two devices (one dedicated to jamming, one receiving)
- SDR hardware (HackRF, etc.) with full-duplex capability and better timing control
- Very specific custom firmware hacks with precise timing that most users never get stable

### Legal Status

**Jamming is illegal in Australia** under the Radiocommunications Act (ACMA). Using these techniques on property you do not own compounds the offence with theft/break-and-enter charges.

For the majority of vehicles in this database, investing the same hours into high-quality distance RAW captures of your own fobs will produce more usable results than pursuing RollJam.

**Focus on what works: clean distance captures on your own hardware.**

---

## SD Card Best Practices

- Keep a clean backup of your SD card before any significant experiments
- Use the `/ext/subghz/car_hacks_au/` structure consistently — it scales
- Name files with context: `hilux_2012_lock_10m_ook650_2026-05-30.sub`
- After every successful capture session, immediately copy files off the Flipper
- Test one variable at a time when changing presets or firmware settings
- Maintain a personal notes file documenting what worked and what didn't

---

## Contributing

This repository maintains its quality by being selective about what it accepts.

**Contributions must meet these standards:**

- Only submit `.sub` files you captured yourself from hardware you own
- Include complete metadata: model, year, distance, preset, button, success notes
- Use the generator templates as your starting point
- If you discover better presets, new model data, or improved capture methods verified on real Australian hardware, update the Python script and open a PR with supporting evidence
- Document your FCC filing links, teardowns, and references in `fcc_research/sources.md`

**Contributions that will be rejected:**

- Files copied from Telegram, Discord, or other channels with zero provenance
- Unverified claims without supporting capture data
- Anything that cannot be reproduced on hardware the contributor actually owns

A pull request that says "Captured on my own 2019 Ranger PX3 at 12 m using 2FSKDev476, lock and unlock both work reliably" is exactly what this repository needs.

---

## Sources & Credits

Everything in this repository is traceable. No exceptions.

- **Flipper Zero community / Automotive-Sub-Ghz-Collection** — gold standard structure and massive verified automotive Sub-GHz collection. Especially the Asia/Toyota and Hyundai folders.
- **AU Sub-GHz Research collections** — extensive real-world `.sub` file examples, technique documentation, and professional organisation patterns.
- **fccid.io public filings** — primary method for confirming real frequencies, modules, and manufacturers. Toyota MDL-B42TA, B41TA confirmed at 433.92 MHz.
- **Official Flipper Devices firmware** — defines the canonical `.sub` file format, standard presets (OOK650, 2FSKDev476), and Sub-GHz application behaviour.
- **Australian market vehicle teardowns and community reports (2010-2024)** — real-world testing on actual AU-market vehicles. Only data that aligned with FCC filings and produced repeatable results made it into the database.
- **ProtoPirate / RocketGod ecosystem and ProtoView (antirez)** — serious on-device analysis tools for the community.
- **CVE-2022-27254 ("Rolling-Pwn")** — public disclosure of Honda rolling code implementation failure.

The Python generator (`fcc_research/fcc_au_car_data.py`) is the living compilation of all these sources.

Special thanks to every Australian who actually tests this on their own vehicles instead of posting unverified files.

---

## Final Words from Jesse
I built this repository because I was tired of seeing the same recycled, unverified, often incorrect Sub-GHz files floating around the Australian Flipper scene. 433.92 MHz is real here. The cars are real. The rolling code challenge is real.

This repository gives you the verified starting data, the professional structure, the generator that keeps everything honest, and the straightforward capture education.

What it cannot give you is working codes for cars that aren't yours. That does not exist in the public domain for modern vehicles, and anyone telling you otherwise is either misinformed or about to get you in trouble.

Use this to understand your own vehicles. Capture your own signals. Build your own playlists. Protect your own property by knowing exactly how these attacks work.

Stay legal. Stay verified. Build real skills.

If this repository helped you, star it, contribute your verified captures when you have them, and help the next person coming along.

Now get out there, stand ten metres back from your own Hilux or Ranger, press that fob a dozen times with proper gaps, and make some quality `.sub` files.

— Jesse
Australian 433.92 MHz Sub-GHz Research | [github.com/Jhoath994](https://github.com/Jhoath994)
---

**flipper-zero-au-subghz** — v1.0
Verified research only. Australian-focused. 433.92 MHz.

If you find errors, open an issue with evidence. We fix them. That is how this stays reliable.