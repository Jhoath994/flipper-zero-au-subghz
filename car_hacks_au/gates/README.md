# Gates, Garages & 433.92 MHz Openers — Jesse's Verified AU Training Targets

**"Gates first, mate. Cars are challenging. Gates are where you learn the actual workflow without getting frustrated." — Jesse**
This folder is for **your own gate/garage remote captures only**. Nothing else belongs here.

**Start here if you're new.** These are dramatically easier training targets than modern car fobs. Many use simple OOK with fixed codes or short/simple rolling codes. Success rates are way higher. Perfect for learning capture → verify → playlist → automation before you touch the hard stuff.

---

## Common Australian 433.92 MHz Gate/Garage Brands (From Verified Notes)

All target the same 433.92 MHz ISM band as the cars.

- **ATA (Automatic Technology Australia)** — extremely common. The default on a huge number of AU homes and industrial sites. Often straightforward OOK.
- **B&D (B&D Doors)** — very common residential and commercial. Many simple OOK or light rolling implementations.
- **Centurion / Elsema** — many models use 433.92. Elsema in particular has a long history of 433/434 MHz remotes in AU. Good training targets.
- **Nice / Beninca** (some models) — imported and local installs. Check for OOK.
- **Linear / Multi-Code style** (if imported) — older US-style but some 433 variants float around.
- **Generic Chinese "universal" remotes** sold on eBay/AU sites — cheap, often fixed code or very simple rolling. Excellent for first captures and brute force practice (where legal on your own property).

**Reality check from the database:**
"Many of these use simple OOK with fixed or 12-24 bit rolling codes. Much easier than modern car fobs. Excellent for learning and playlists."

---

## Frequency + Modulation Reality (Gates Edition)

- **Frequency**: 433920000 Hz (433.92 MHz ISM — same as the cars, plus nearby drift frequencies like 433.82 / 434.02 can be useful)
- **Common modulation**: Vast majority are ASK/OOK. Some older or specific models may have slight variations, but OOK650 or close is the winner 95% of the time.

The ASK/FSK trap is **much less of an issue** with gates than with 2018+ cars. You will rarely need 2FSKDev476 here.

---

## Best Starting Preset + Why

**Recommended preset: OOK650** (or the AU_Car_OOK_43392 custom if you've added the settings_user additions)

**Why OOK650 first for gates?**
- Simple OOK is the dominant modulation on ATA, B&D, Elsema, generics, etc.
- Clean captures with proper distance + multiple presses will often give you immediately usable files (especially on fixed-code or lightly rolling units).
- If range is poor, try a custom OOK preset with slightly tweaked bandwidth in your firmware.

**Alternative / nearby frequencies worth having in your settings_user.txt** (from the generator):
- 433820000
- 433920000 (main)
- 434020000
- 434420000
- Extras some people add for gates/older stuff: 433075000, 433775000, 434775000

**Deviation notes**: Most gates don't care about the fine FSK deviation games that destroy you on Rangers and i30s. OOK is your friend.

---

## Real Capture Advice — Even Easier Than Cars (But Still Do It Right)

1. **Stand 5-15 metres away** from the gate/garage motor or receiver. Same rules as cars — distance is often cleaner than standing right next to it.
2. **Multiple button presses with proper gaps**: Press the button, wait 2-3 seconds, press again. Do 8-12+ presses. Even on "fixed" code units this gives you solid data and lets you see consistency.
3. **Capture as RAW first, every time.** Decode after. Many of these will actually decode nicely once you have good raw.
4. **Verify immediately** on *your own* gate. Walk it, test range, test from inside the car, test at different angles. This is how you learn what actually works.
5. Document: brand (ATA/B&D/Elsema/etc), model if known, fixed or rolling (if you can tell), distance, preset, time of day, any quirks (e.g. only works from certain angle).

**Pro tip**: Gates are perfect for building your first playlists. Capture lock/unlock equivalents (open/close/stop), organise them, load the playlist, and walk the property testing range while it cycles.

**Gates vs Cars reality**:
- Cars (especially post-2015): Rolling + challenge-response + FSK = pain. Many captures give you one shot at best.
- Gates: Fixed code or short/simple rolling = you can often build working sequences and actually use them for automation/testing on your own property.

---

## What Users Should Put in This Folder

**Only your own verified captures from hardware you own.**

Good naming convention (steal this):
- `ata_garage_2022_open_8m_ook650_2026-05-30.sub`
- `bd_door_2018_close_10m_ook650_raw.sub`
- `elsema_remote_2015_open_6m_ook650.sub`
- `generic_universal_gate_12m_ook650_fixedcode.sub`
- `ata_industrial_2024_stop_15m_ook650.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of a gates playlist (you can generate/extend one using the Python script patterns) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for random internet gate files. Capture your own or it doesn't count.

---

## Known Quirks & Pro Tips for AU Gates (From Community + Database)

- **ATA**: Extremely common. Many respond beautifully to clean OOK captures. Some newer ones have light rolling — still far easier than cars.
- **B&D**: Very common on roller doors. Often forgiving. Great for learning the full "capture multiple + verify range" workflow.
- **Elsema / Centurion**: Long-time AU players. Many older models are basically textbook OOK. Some 12-24 bit codes that are easy to work with.
- **Generics from eBay/AU sites**: Often the absolute easiest. Fixed code or trivial rolling. Perfect first success stories. Also great for testing playlist automation and range mapping.
- Weather, metal gates, motor interference, and receiver location all affect real-world range. Document it.
- Many gate systems have a "learn" mode on the receiver — useful for defensive testing on your own setup (adding a spare remote you control).
- Some industrial/commercial setups use longer codes or slight frequency offsets — having the nearby frequencies in your settings helps.

These are the targets where you can actually build useful personal libraries and see the techniques working in real time.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT (GATES EDITION)

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, jamming, brute-forcing, or knowledge from here against gates, garages, or industrial doors you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply. Full stop.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's roller door or the servo's gate.
- Jamming is especially illegal and a great way to get ACMA and police attention fast.
- Even on simple fixed-code gates, replaying someone else's signal without permission is not a game.
- The repo maintainers and Jesse take **zero responsibility** for you misusing this information and getting charged.
If you're here looking to open gates that aren't yours: **leave now**. Close the folder. This is not for you.

If you own the gate/garage, test only on it, stay legal, learn the workflow, and use this as training wheels for the harder car work — welcome. you will be fine.

**Gates are for learning. Cars are the final boss. Stay on your own property.**

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `GATE_OPENER_NOTES` in `fcc_research/fcc_au_car_data.py` + the main repo README section on gates. No hallucinations, no made-up brands or frequencies.

Now stop reading and go capture your own ATA or B&D remote properly at 8-10 metres. OOK650. Multiple presses with gaps. RAW first. Verify it actually opens *your* gate. Then build a playlist. This is how you get good.

Once you've got a few gates working reliably, *then* go stand 10 metres back from your own Hilux or Ranger and apply the same discipline.

you will be fine.

— JesseVerified only • Australian 433.92 specialist • No rules except the real ones • Gates before cars, always

