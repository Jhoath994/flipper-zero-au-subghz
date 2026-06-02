# Subaru — Jesse's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No unverified claims, no fabricated codes." — Jesse**
This folder is for **your own Subaru** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- Forester (SJ 2013-2018, SK 2018+)
- XV, Outback (AU variants)

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. Subarus are everywhere in the snow, bush, and suburbs.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: Mix, many ASK on older, FSK later

**The ASK/FSK trap applies.** Forester keys vary by year. OOK works on plenty of them. Later models more likely to need FSK.

---

## Best Starting Preset + Why

**Recommended preset: OOK650**

**Why OOK650 first?**
- Many ASK on older Forester (SJ era especially).
- OOK works on a surprising number of them.
- If no joy, quick switch to 2FSKDev476 for later SK models or variants.

**Alternative presets to try if OOK650 gives poor results:**
- 2FSKDev476

**Deviation notes from the verified database:**
"Forester keys vary by year. OOK works on plenty of them."

---

## Real Capture Advice (Jesse Method — Do This or Waste Your Time)
1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation degrades your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* Subaru. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model (Forester SJ/SK / XV / Outback), year, distance, preset used, button, conditions.

**FCC refs (traceable):** Subaru 433.92

**Capture tips from database:** "Subaru AWD utes and wagons everywhere in AU. Solid target."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `forester_sj_2016_lock_10m_ook650_2026-05-30.sub`
- `forester_sk_2020_unlock_9m_2fskdev476_raw.sub`
- `xv_2018_ook650_8m.sub`
- `outback_2017_boot_11m_ook650.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `subaru_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **Forester SJ (2013-2018)**: Many ASK examples. OOK650 is the right starting point for a surprising number.
- **Forester SK (2018+)**: Higher chance of FSK. Have 2FSKDev476 ready.
- XV and Outback AU variants: Similar mix depending on exact year. Solid target for building a well-rounded personal collection.
- Rolling codes present. Another good AWD wagon/ute for the collection.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a Subaru you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's Forester or the family XV.
- Rolling codes exist specifically to prevent exactly this type of attack. Most modern replays fail fast anyway.
- The repo maintainers and Jesse take **zero responsibility** for you misusing this information and getting charged.
If you're here looking to steal Subarus: **leave now**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. you will be fine.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["subaru"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations.

Now stop reading and go capture your own Forester or XV properly at 10 metres. Start OOK650, switch to 2FSK if the year demands it. Multiple presses. RAW first.

— JesseVerified only • Australian 433.92 specialist • No rules except the real ones

