# Ford Ranger — Kelly's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No bullshit, no made-up codes." — Kelly**

This folder is for **your own Ford Ranger** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- Ranger PX/PX2/PX3 (2011-2024 AU market)
- Everest (some shared platforms)

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. One of the most common utes on the road.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: Often 2FSK on later models, ASK on early

**The ASK/FSK trap is brutal on Rangers.** Early models often ASK. 2018+ frequently 2FSK. Using an OOK preset on a true FSK fob gives you dogshit range and fake "decoded" signals that do absolutely nothing when replayed.

---

## Best Starting Preset + Why

**Recommended preset: 2FSKDev476**

**Why 2FSKDev476 first?**
- Many 2018+ Ranger fobs are FSK. This is the commonly reported effective starting point (deviation around 47.6 kHz).
- Starting with OOK650 on these = complete waste of time and battery.
- Capture RAW and let a good custom firmware (RogueMaster/Momentum/Unleashed) suggest modulation if you're unsure.

**Alternative presets to try if 2FSKDev476 gives poor results:**
- OOK650 (for pre-2018 or early PX models)
- Custom

**Deviation notes from the verified database:**
"Many 2018+ Ranger fobs are FSK. Using OOK preset on FSK signal = garbage. Start with 2FSKDev476 or capture and let Flipper suggest."

---

## Real Capture Advice (Kelly Method — Do This or Waste Your Time)

1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation fucks your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* Ranger. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model/year (PX1/PX2/PX3 critical), distance, preset used, button, conditions.

**FCC refs (traceable):** Various Continental / Hella 433/434 MHz modules for AU.

**Capture tips from database:** "Rangers are common targets. Distance technique works on pre-2020ish models. Newer ones have better rolling code implementations."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `ranger_px3_2021_lock_12m_2fskdev476_2026-05-30.sub`
- `ranger_px2_2016_unlock_6m_ook650_raw.sub`
- `everest_2019_boot_10m_fsk.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `ford_ranger_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **2011-2017ish (early PX/PX2)**: More likely to be ASK/OOK. OOK650 can work well.
- **2018+ (PX3 and later)**: Strong shift to 2FSK on many fobs. The preset trap caught a lot of people out.
- **Pre-2020 models**: Distance technique (5-15m + multiple spaced presses) works decently for capture.
- **2020+ / newer**: Tighter rolling code implementations. Success rates drop. Manage expectations.
- Everest sharing platforms: Same rules as the Ranger of the same era.

Rolling codes are present. Simple replay often works zero or one time before the key fob and car desynchronise.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a Ranger or Everest you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's ute or the work truck.
- Rolling codes exist specifically to stop exactly this shit. Most modern replays fail fast anyway.
- The repo maintainers and Kelly take **zero responsibility** for you being a fuckwit and getting charged.

If you're here looking to steal utes: **fuck off**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. She'll be right.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["ford_ranger"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations. Cross-reference real FCC filings for AU modules yourself.

Now stop reading and go capture your own Ranger properly at 10 metres. Multiple presses. Start with the right preset for the year.

— Kelly
Verified only • Australian 433.92 specialist • No rules except the real ones
