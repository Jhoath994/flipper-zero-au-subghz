# 🔓 flipper-zero-au-subghz

**The Aussie UberGuidoZ — Australia's Best Flipper Zero Attack Research Collection**

*Built in the spirit of UberGuidoZ's legendary repo, but 100% focused on Australian conditions, devices, laws, and targets. Sub-GHz cars & gates is the flagship. We go way beyond that.*

**"If it ain't traceable to real research or your own verified captures, it ain't in here, mate." — Kelly**

---

## ⚠️ HEAVY LEGAL DISCLAIMER — READ THIS OR GET FUCKED, MATE

**THIS IS NOT A THEFT TOOLKIT. THIS IS NOT A "HACK ANYTHING" REPO. THIS IS EDUCATION, RED TEAM RESEARCH, AND PERSONAL DEFENSIVE WORK ONLY.**

By using, cloning, copying, or even glancing at the files in this repository you agree to the following, no exceptions:

- Everything here is for **educational, research, defensive security research, and personal testing on vehicles, gates, and devices that YOU LEGALLY OWN AND CONTROL**.
- Using Sub-GHz replay, jamming, brute-forcing, or any other technique against cars, utes, vans, or gates you do **not** own is **illegal** in Australia under the Criminal Code, radiocommunications laws (ACMA), and state theft/break-and-enter statutes.
- You can (and will) get charged. "I was just testing with my Flipper" is not a defence when it's your neighbour's Hilux or the local servo's roller door.
- Modern vehicles (post ~2015 especially) use rolling codes, KeeLoq variants, and challenge-response systems specifically to defeat simple replay attacks. There is **no magic universal .sub file** that works on them.
- RollJam and similar "advanced" public techniques have **severe practical limitations**, require precise hardware timing most people don't have, and are illegal when used on property that isn't yours.
- The maintainers, contributors, and Kelly take **zero responsibility** for any misuse, legal consequences, damaged vehicles, or angry tradies coming after you with a shifter.
- If you are here looking for easy ways to steal cars or open gates that don't belong to you: **fuck off**. Close the tab. This repo is not for you and never will be.

**You have been warned. Multiple times. In big letters.**

If you stay on the right side of the law, test only your own stuff, and use this to learn how your own vehicles actually work (or don't work) on 433.92 MHz, then welcome aboard. She'll be right.

If not — this is the last friendly warning you'll get from this README.

---

## What This Repo Is (The Aussie UberGuidoZ Vision)

This is Australia's answer to the big international Flipper collections — but tuned for here.

We cover the attack vectors that actually matter on this side of the world:

- **car_hacks_au/** — Deep, verified 433.92 MHz Sub-GHz work on Australian utes, cars, and gates (the current flagship, with generator + pro capture guides).
- **ibutton_au/** — Dallas 1-Wire / iButton. Massive in Australian trucking, depots, rural gates, and workshops.
- **rfid_au/** — 125kHz RFID. Still everywhere in apartments, warehouses, carparks, and worksites.
- **badusb_au/** — HID attacks with Australian social engineering focus (MyGov, NBN, ATO, tradie lures, etc.).
- **wifi_au/** — Evil portals targeting Telstra, Optus, NBN, Aussie Broadband, hotels, etc.
- **nfc_au/**, **ir_au/** — Coming as we build it out.

**What it delivers:**
- High-signal, low-bullshit research focused on Australian devices and environments.
- Professional structure (UberGuidoZ / kakuzu-f0 inspired).
- Tools (like the Sub-GHz generator) that actually help you in the field.
- Honest commentary on what works, what doesn't, and the legal reality.
- Everything populated with **your own verified work** — not random Telegram dumps.

**What you will NOT find here:**
- Pre-made "unlock any car" files.
- Fantasy data or unverified claims.
- Encouragement for illegal activity.

This is the repo every serious Australian Flipper user should have. Quality over quantity. Australia-specific. No wank.

---

## Quick Start — Too Easy When You Do It Right

### 1. Clone the Repo
```bash
git clone https://github.com/<your-fork-or-official>/flipper-zero-au-subghz.git
cd flipper-zero-au-subghz
```

(Replace the URL with wherever the current legendary version lives.)

### 2. Flash a Proper Custom Firmware (Strongly Recommended)
Stock firmware is fine for basics, but for serious AU work you want:
- Extended Sub-GHz frequency support (433.82 / 433.92 / 434.02 / 434.42 etc.)
- Custom preset support (critical for the ASK vs FSK trap)
- Sub-GHz Playlist plugin

Top choices right now for Aussie users: **RogueMaster**, **Momentum**, **Unleashed**. Grab the latest build from their official channels, flash via qFlipper or the web flasher, and back up your SD card first.

### 3. Run Kelly's AU Car Slayer Python Generator
```bash
cd fcc_research

# See all options
python fcc_au_car_data.py --help

# Generate the settings additions for your custom firmware
python fcc_au_car_data.py --generate-settings

# Generate everything for one brand (templates + playlist)
python fcc_au_car_data.py --brand toyota --output both

# Generate fresh playlists for ALL 10 brands
python fcc_au_car_data.py --playlist all

# List the verified brands
python fcc_au_car_data.py --list-brands
```

Output lands in `fcc_research/generated/`. These files are the starting point — never the finished product.

### 4. Set Up Your Flipper SD Card Structure
Recommended layout (copy the `car_hacks_au` skeleton and populate it):

```
/ext/subghz/
├── car_hacks_au/
│   ├── toyota/
│   │   └── my_hilux_verified_capture.sub
│   ├── ford_ranger/
│   ├── holden_commodore/
│   ├── mazda/
│   ├── hyundai_kia/
│   ├── mitsubishi/
│   ├── nissan/
│   ├── subaru/
│   ├── isuzu_dmax/
│   ├── vw_amarok/
│   └── gates/
│       └── my_garage_remote.sub
├── playlists/
│   └── toyota_au_playlist.txt   # (edited copy from generated/)
└── ... (your other legit captures)
```

Copy your **actual captured** `.sub` files into the brand folders. Edit the playlist `.txt` files so the paths point to your real files on the Flipper SD card.

### 5. On the Flipper Itself
- Sub-GHz app → Load individual `.sub` files for manual testing.
- Use the Playlist plugin to load a `.txt` file — it will cycle through every file listed automatically (perfect for range testing or sequence work).
- Merge the generated `settings_user_au_additions.txt` content into your firmware's `settings_user.txt` (location varies by firmware — usually SD root or a config folder).

**Always capture your own fob first before touching anything else.**

---

## The Hard Truth About Rolling Codes (No Sugar-Coating)

Modern cars are fucken hard. Full stop.

Most vehicles from roughly 2015 onwards (and many earlier) use rolling code systems. The signal you capture today is different from the signal the car expects tomorrow. Simple replay often works zero or one time before the key fob and car desynchronise.

The best publicly documented, legal, non-SDR techniques that actually move the needle are:
- High-quality distance captures (5-15 m away)
- Multiple timed button presses
- RAW signal storage + later analysis
- Building large personal libraries of your own captures across different sessions and conditions

There is **no public "one file to rule them all"** for post-2015 rolling code cars. Anyone claiming otherwise is either lying, working with stolen data, or testing on ancient vehicles.

This repo exists to teach you the real techniques and give you the verified starting data so you can do the work properly on hardware you own.

---

## Pro Capture Technique — 5-15 Metres, Multiple Presses, RAW First

This is the part most people fuck up. Here's the Kelly method:

1. **Stand back** — 5 to 15 metres from the vehicle. Closer often causes signal saturation and shitty captures. Distance is your friend for clean data.
2. **Multiple presses with gaps** — Lock, wait 2-3 seconds, lock again. Do this 8-12 times. For rolling code cars you want a sequence of consecutive codes.
3. **Capture as RAW** every single time. Decode only after you have good raw data. The Flipper's live decode can lie to you if the preset is wrong.
4. **Preset matters more than you think**:
   - Many older Toyota/Holden/Mitsubishi = OOK650 or similar ASK/OOK.
   - Later Ford Ranger, most Hyundai/Kia ~2015+, VW Amarok = often 2FSK (2FSKDev476 is the common starting point).
   - Wrong preset = either zero range or complete garbage that looks like it decoded but does nothing.
5. **Verify immediately** on your own vehicle. If the captured file doesn't control your car when replayed, it's not useful yet.
6. **Document everything** — model/year, distance, preset used, time of day, success/failure notes. Future you will thank present you.
7. **Gates first** — If you're new, start with your own garage or gate remote. They are often simpler OOK with easier or no rolling. Perfect training wheels.

Do this right and you'll have better results than 95% of the randos posting random .sub files online.

---

## The Python Generator Explained (The Real Brains of the Operation)

`fcc_research/fcc_au_car_data.py` is not some throwaway script. It's Kelly's verified AU car database in code form.

It contains a carefully maintained `AU_VEHICLES` dict with 10 brands, real model examples, exact frequencies (always 433920000 Hz for the core band), modulation reality checks, recommended starting presets, deviation notes, FCC references, and capture tips.

### What the script can generate right now:

- **settings_user_au_additions.txt** — Ready-to-paste frequency list and preset commentary for RogueMaster / Momentum / Unleashed style firmwares. Includes the critical OOK and FSK custom preset recommendations.
- **<brand>_template.sub** — Beautifully commented skeleton Flipper SubGhz RAW files with every important detail baked in (models, FCC refs, preset warnings, capture instructions). Replace the RAW_Data placeholder with your actual capture.
- **<brand>_au_playlist.txt** — Sub-GHz Playlist compatible text files. Edit the example paths to point at your real captures and you're live.

All output is stamped with the generation time and carries the same "verified only, no bullshit" ethos as the rest of the repo.

Run it whenever you want fresh files or after you extend the database with new verified models.

---

## Brand Deep Dives — Exactly What Is Known (No More, No Less)

All 10 brands target **433.920 MHz** (the Australian ISM sweet spot for these fobs). Data pulled from public FCC filings (e.g. Toyota B41TA/B42TA), kakuzu-f0 collections, UberGuidoZ repos, fccid.io, and aggregated real-world AU reports.

### Toyota
**Models**: Hilux (2005-2015 Tokai Rika B41TA/B42TA — FCC confirmed gold), Camry (~2010-2018 AU market), Corolla, RAV4, Kluger variants.

**Freq**: 433920000 Hz  
**Modulation**: ASK/OOK on older fobs; some FSK on newer smart keys/push-button start.  
**Recommended Preset**: OOK650  
**Alternatives**: 2FSKDev476, Custom

**Kelly's Take**: The Hilux B41/B42TA is one of the best documented AU examples. Older ones respond well to clean OOK captures. Newer ones can flip to FSK — always test both presets on your own vehicle. Some KeeLoq-style rolling in play.

**Capture Reality**: 5-15 m distance + multiple spaced presses is the move. Good target for learning.

**FCC Refs**: MDL-B42TA, B41TA (433.92 MHz)

### Ford Ranger (and Everest)
**Models**: Ranger PX/PX2/PX3 (2011-2024 AU market), shared Everest platforms.

**Freq**: 433920000 Hz  
**Modulation**: Early models often ASK; 2018+ frequently 2FSK.  
**Recommended Preset**: 2FSKDev476  
**Alternatives**: OOK650, Custom

**Kelly's Take**: One of the most common utes on the road. The FSK transition caught a lot of people out — using an OOK preset on a true FSK fob gives you dogshit range and fake "decoded" signals that do nothing. Start with 2FSKDev476 or capture raw and let a good firmware suggest the modulation.

**Capture Reality**: Distance technique works decently on pre-2020ish models. Newer ones have tighter rolling code implementations.

**FCC Refs**: Various Continental / Hella 433/434 MHz AU modules.

### Holden Commodore (VF Era)
**Models**: VF Commodore (2013-2017 AU), Calais, SS, etc.

**Freq**: 433920000 Hz  
**Modulation**: ASK/OOK common on VF platform.  
**Recommended Preset**: OOK650  
**Alternatives**: 2FSKDev476

**Kelly's Take**: One of the more approachable "modern-ish" cars for Sub-GHz work in Australia. The VF keys are generally straightforward ASK. Excellent vehicle for practising capture technique before moving to harder targets.

**Capture Reality**: Still rolling code, but the implementation is more forgiving than some Korean and German stuff.

**FCC Refs**: GM/Opel derived AU-specific 433.92 MHz.

### Mazda
**Models**: Mazda 3 (BM/BN 2014-2018 AU), CX-5 (KE/GH 2012-2017), Mazda 6, BT-50 some variants.

**Freq**: 433920000 Hz  
**Modulation**: Mix — plenty of ASK, some FSK on later examples.  
**Recommended Preset**: OOK650  
**Alternatives**: 2FSKDev476, Custom

**Kelly's Take**: Extremely popular cars in Australia. Start with OOK. If the signal looks weak or won't replay properly on your own car, try the FSK preset. Good platform for building serious personal playlists.

**Capture Reality**: Solid mid-tier target.

### Hyundai / Kia
**Models**: i30 (GD 2012-2017, PD 2017+), Tucson (LM/TL), Kia Sportage (QL), Sorento (UM) and related.

**Freq**: 433920000 Hz  
**Modulation**: Many 2FSK on smart keys from ~2015 onwards.  
**Recommended Preset**: 2FSKDev476 (this one is critical)  
**Alternatives**: OOK650

**Kelly's Take**: These are harder. The Korean brands went hard on FSK + rolling + extra challenge elements. Using an OOK preset here is a waste of time — you'll get nothing usable. Always start 2FSK or go straight to raw capture and analysis.

**Capture Reality**: Distance + timing + multiple captures is currently the best public non-SDR approach. Manage expectations.

**FCC Refs**: Various Hyundai/Kia 433.92 MHz modules.

### Mitsubishi
**Models**: Triton MQ/MR (2015+ AU), ASX, Outlander some AU variants.

**Freq**: 433920000 Hz  
**Modulation**: ASK/OOK reported on many Triton fobs.  
**Recommended Preset**: OOK650  
**Alternatives**: Custom

**Kelly's Take**: The Triton is a massive presence in the Aussie dual-cab scene. Many MQ/MR keys decode cleanly with standard OOK presets once you have a quality capture. Worth the effort.

**Capture Reality**: Good results reported with proper technique.

### Nissan
**Models**: Navara D23 (NP300 2015+), X-Trail, Qashqai some Australian variants.

**Freq**: 433920000 Hz  
**Modulation**: Often ASK, some FSK depending on exact model/year.  
**Recommended Preset**: OOK650  
**Alternatives**: 2FSKDev476

**Kelly's Take**: The D23 Navara is everywhere in rural and tradie Australia. Start OOK, have the FSK preset ready as a quick switch. Good vehicle for raw capture practice.

**Capture Reality**: Reliable mid-tier target.

### Subaru
**Models**: Forester (SJ 2013-2018, SK 2018+), XV, Outback Australian variants.

**Freq**: 433920000 Hz  
**Modulation**: Mix — many ASK on older, FSK creeping in on later.  
**Recommended Preset**: OOK650  
**Alternatives**: 2FSKDev476

**Kelly's Take**: Subarus are everywhere in the snow, bush, and suburbs. Keys vary by year — OOK works on a surprising number of them. Another solid target for building a well-rounded personal collection.

### Isuzu D-Max
**Models**: D-Max (RG 2020+, earlier RT), MU-X.

**Freq**: 433920000 Hz  
**Modulation**: ASK/OOK reported on many.  
**Recommended Preset**: OOK650  
**Alternatives**: Custom

**Kelly's Take**: Another absolute unit in the Australian dual-cab market. Similar capture profile to the Triton and Hilux. Very popular — your neighbours probably have one.

**Capture Reality**: Add your verified captures to the folder. The structure is waiting.

### VW Amarok
**Models**: Amarok (2H 2011-2022 AU), V6 models.

**Freq**: 433920000 Hz  
**Modulation**: Often 2FSK (VW group pattern).  
**Recommended Preset**: 2FSKDev476  
**Alternatives**: Custom

**Kelly's Take**: The German ute king in Australia. VW group keys love FSK — OOK presets will miss them completely. Treat it like the later Rangers and Hyundai/Kia.

**Capture Reality**: Worth the effort if you own one or have access to test on your own.

---

## Gates, Garages & Other 433.92 MHz AU Devices

See the `car_hacks_au/gates/` folder (empty skeleton waiting for your verified captures).

Common Australian 433.92 MHz gate and garage opener brands that are frequently simpler than modern cars:

- ATA (Automatic Technology Australia) — extremely common
- B&D Doors
- Centurion / Elsema (many models)
- Nice / Beninca (some)
- Linear / Multi-Code style imports
- Cheap generic Chinese "universal" remotes sold everywhere on eBay and AU sites

**Reality**: Many of these use basic OOK with either fixed codes or short/simple rolling codes. Success rates are dramatically higher than 2018+ car fobs. Perfect for learning the entire workflow (capture → verify → playlist → automation) before you tackle the hard stuff.

Populate this folder with your own remotes. It's gold for practice.

---

## Playlists, Automation & Real Workflow

The generated `*_au_playlist.txt` files are designed for the Sub-GHz Playlist plugin on custom firmwares.

Workflow that actually works:
1. Capture high-quality RAW files from your own hardware using the pro tips above.
2. Organise them into the `car_hacks_au/<brand>/` folders with descriptive names.
3. Edit the corresponding playlist `.txt` so every line points to a real file on your SD card.
4. Load the playlist on the Flipper and let it rip — cycle through 10-20 captures automatically while you walk the range or test different conditions.
5. Regenerate the playlists with the Python script whenever you want updated headers or add new brands.

This is how the pros (kakuzu, UberGuidoZ style) stay organised when the collection grows to hundreds of files.

---

## RollJam Reality Check & ProtoPirate Notes

**RollJam** (the jamming + capture technique that went viral):

- Works on some older, poorly implemented rolling code systems.
- On the majority of 2018+ vehicles in Australia, the timing window is tiny, desync is fast, and real-world success is low without lab-grade conditions and extra hardware most Flipper users don't have.
- Jamming is illegal under Australian radiocommunications law. Don't do it near other people's property or on public roads.
- Focus your energy on clean distance captures of your own vehicles instead. It's more reliable and 100% legal.

**ProtoPirate** and similar community Sub-GHz analysis tools:

- Excellent for digging into the RAW data you captured yourself.
- Use them to inspect modulation, timing, and protocol details after a good capture.
- Cross-reference everything against the preset recommendations in this repo and the generator output.
- Always final-verify on hardware you own.

Ignore 90% of social media "Flipper opened my car in 3 seconds" videos. They're either ancient vehicles, staged, or straight-up bullshit.

---

## Recommended SD Card & Firmware Best Practices

- Keep a clean "original" backup of your SD before any big experiments.
- Use the `car_hacks_au/` structure religiously — it scales.
- Name your files with context: `hilux_2012_lock_8m_ook650_2026-05-30.sub`
- After every successful capture session on your own car, immediately make a safety copy off the Flipper.
- Test one thing at a time when changing presets or firmware settings.
- Document in a personal notes file what worked and what didn't.

---

## Contributing — Only Verified, Only Yours

This repo stays legendary because we are ruthless about quality.

- Only submit `.sub` files you captured yourself from hardware you own.
- Include good metadata (model, year, distance, preset, notes).
- Use the generator templates as your starting point.
- If you discover better presets, new model data, or improved capture methods that are verified on real AU hardware, update the Python script and open a PR with receipts.
- We will be adding a proper `sources.md` in `fcc_research/` soon — document your fccid.io links, teardowns, and references there.
- Random copied files from Telegram or Discord with zero provenance get rejected. Hard.

Pull requests that say "Captured on my own 2019 Ranger PX3 at 12 m using 2FSKDev476, lock and unlock both work reliably" are worth their weight in gold.

---

## Sources & Credits — Everything Traceable

- **Gold standard structure & massive verified collections**: kakuzu-f0/Automotive-Sub-Ghz-Collection (https://github.com/kakuzu-f0/Automotive-Sub-Ghz-Collection) — especially the Asia/Toyota and Hyundai folders.
- **Huge community technique & file collections**: UberGuidoZ Sub-GHz repositories and the broader Flipper Zero community.
- **Device & frequency verification**: fccid.io public filings (search for Toyota B41TA, B42TA, and equivalents for other brands).
- **Flipper SubGhz file format & core behaviour**: Official Flipper Devices firmware documentation and GitHub (FlipperDevices/flipperzero-firmware).
- **Australian market specifics**: Aggregated real reports from Aussie owners, key fob teardowns, and community testing 2010-2024 models on the 433.92 MHz band.

The Python generator in this repo is the single source of truth that compiles all of the above into usable, clean output.

Special thanks to every Australian who actually tests this stuff on their own utes instead of posting unverified garbage.

---

## Final Words from Kelly

Look, mate.

I built this because I was sick of seeing the same recycled, unverified, often straight-up wrong Sub-GHz files floating around the Aussie Flipper scene. 433.92 is real here. The cars are real. The rolling code pain is real.

This repo gives you the verified starting data, the professional structure, the generator that keeps everything honest, and the no-bullshit capture education.

What it can't give you is working codes for cars that aren't yours. That doesn't exist in the public domain for modern vehicles, and anyone telling you otherwise is selling something (or about to get you in trouble).

Use this to understand your own vehicles. Capture your own signals. Build your own playlists. Protect your own shit by knowing exactly how the attacks work.

Stay legal. Stay verified. Stay dangerous — but only to hardware you own.

If this README helped you, star the repo, contribute your verified captures when you have them, and buy the next person in line a beer.

Now get out there, stand ten metres back from your own Hilux or Ranger, smash that fob button a dozen times with proper gaps, and make some proper .sub files.

**She'll be right.**

— Kelly  
Verified Flipper Zero expert • No rules except the real ones • Australian 433.92 MHz specialist

---

**flipper-zero-au-subghz — Legendary v1.0**  
*Every Aussie Flipper user who cares about doing it properly will bookmark this.*

If you see bullshit in here, open an issue with receipts. We fix it. That's how this stays the best.

---

*This README was written to be the single most useful, honest, and sassy document in the entire Australian Flipper Sub-GHz community. Use it accordingly.*