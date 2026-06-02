# Mitsubishi — Jesse's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No unverified claims, no fabricated codes." — Jesse**
This folder is for **your own Mitsubishi** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- Triton MQ/MR (2015+ AU)
- ASX, Outlander (some AU variants)

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. The Triton is a massive AU ute.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: ASK/OOK on many Triton fobs

Many MQ/MR keys are frequently reported as decodable with standard OOK presets after good capture. Similar profile to Hilux and D-Max.

---

## Best Starting Preset + Why

**Recommended preset: OOK650**

**Why OOK650 first?**
- Triton MQ/MR keys are frequently reported as decodable with standard OOK presets after good capture.
- Straightforward ASK on many examples.
- Good results reported with proper technique.

**Alternative presets to try if OOK650 gives poor results:**
- Custom

**Deviation notes from the verified database:**
"Triton MQ/MR keys are frequently reported as decodable with standard OOK presets after good capture."

---

## Real Capture Advice (Jesse Method — Do This or Waste Your Time)
1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation degrades your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* Mitsubishi. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model (Triton MQ/MR/ASX/Outlander), year, distance, preset used, button, conditions.

**FCC refs (traceable):** Mitsubishi 433.92

**Capture tips from database:** "Triton is a massive AU ute. Worth targeting."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `triton_mr_2020_lock_10m_ook650_2026-05-30.sub`
- `triton_mq_2017_unlock_8m_ook650_raw.sub`
- `asx_2018_ook650_9m.sub`
- `outlander_2016_boot_11m_ook650.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `mitsubishi_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **Triton MQ/MR (2015+)**: Massive presence in the Aussie dual-cab scene. Many keys decode cleanly with standard OOK presets once you have a quality capture. Worth the effort.
- ASX and Outlander (some AU variants): Similar 433.92 ASK profile on many examples.
- Rolling codes are present on most modern ones. Distance + multiple spaced presses remains the move.
- Shares some "big ute" capture characteristics with Hilux, D-Max, and Ranger (pre-FSK years).

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a Mitsubishi you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's Triton or the work ute.
- Rolling codes exist specifically to prevent exactly this type of attack. Most modern replays fail fast anyway.
- The repo maintainers and Jesse take **zero responsibility** for you misusing this information and getting charged.
If you're here looking to steal Mitsubishis: **leave now**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. you will be fine.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["mitsubishi"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations.

Now stop reading and go capture your own Triton properly at 10 metres. OOK650 is your friend here. Multiple presses. RAW first.

— JesseVerified only • Australian 433.92 specialist • No rules except the real ones

