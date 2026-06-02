# Nissan — Jesse's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No unverified claims, no fabricated codes." — Jesse**
This folder is for **your own Nissan** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- Navara D23 (NP300 2015+)
- X-Trail, Qashqai (some AU)

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. The D23 Navara is everywhere in rural and tradie Australia.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: Often ASK, some FSK depending on exact model/year

**The ASK/FSK trap applies.** Start OOK, switch to FSK if no joy. Navara D23 is very common in AU.

---

## Best Starting Preset + Why

**Recommended preset: OOK650**

**Why OOK650 first?**
- Often ASK on many examples.
- Good candidate for raw capture practice.
- Start OOK, have the FSK preset ready as a quick switch if the signal is weak or won't replay properly.

**Alternative presets to try if OOK650 gives poor results:**
- 2FSKDev476

**Deviation notes from the verified database:**
"Navara D23 is very common in AU. Start OOK, switch to FSK if no joy."

---

## Real Capture Advice (Jesse Method — Do This or Waste Your Time)
1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation degrades your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* Nissan. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model (D23 Navara / X-Trail / Qashqai), year, distance, preset used (note if you switched to FSK), button, conditions.

**FCC refs (traceable):** Nissan 433.92 modules

**Capture tips from database:** "Good candidate for raw capture practice."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `navara_d23_2019_lock_10m_ook650_2026-05-30.sub`
- `navara_d23_2021_unlock_12m_2fskdev476_raw.sub`
- `xtrail_2018_ook650_8m.sub`
- `qashqai_2017_boot_9m_ook650.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `nissan_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **Navara D23 (NP300 2015+)**: Very common in rural and tradie Australia. Often ASK but some FSK depending on exact year and market variant. Start OOK650.
- X-Trail and Qashqai (some AU): Similar 433.92 profile — mix of ASK and occasional FSK.
- Rolling codes present. Reliable mid-tier target for building experience.
- Good vehicle for practising the "capture RAW → try both presets → verify on own car" loop.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a Nissan you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's Navara or the family X-Trail.
- Rolling codes exist specifically to prevent exactly this type of attack. Most modern replays fail fast anyway.
- The repo maintainers and Jesse take **zero responsibility** for you misusing this information and getting charged.
If you're here looking to steal Nissans: **leave now**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. you will be fine.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["nissan"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations.

Now stop reading and go capture your own D23 Navara properly at 10 metres. Start OOK650, switch to 2FSKDev476 if needed. Multiple presses. RAW first.

— JesseVerified only • Australian 433.92 specialist • No rules except the real ones

