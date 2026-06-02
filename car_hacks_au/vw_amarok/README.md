# VW Amarok — Jesse's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No unverified claims, no fabricated codes." — Jesse**
This folder is for **your own VW Amarok** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- Amarok (2H 2011-2022 AU)
- V6 models

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. The German ute king in Australia.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: Often 2FSK (VW group)

**The ASK/FSK trap is non-negotiable.** VW group keys frequently FSK. OOK preset will miss them completely.

---

## Best Starting Preset + Why

**Recommended preset: 2FSKDev476**

**Why 2FSKDev476 first?**
- VW group keys frequently FSK.
- OOK presets will miss them completely — you get nothing.
- Treat it like the later Rangers and Hyundai/Kia.

**Alternative presets to try if 2FSKDev476 gives poor results:**
- Custom

**Deviation notes from the verified database:**
"VW group keys frequently FSK. OOK preset will miss them."

---

## Real Capture Advice (Jesse Method — Do This or Waste Your Time)
1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation degrades your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* Amarok. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model (Amarok 2H / V6), year, distance, preset used (**must note 2FSK**), button, conditions.

**FCC refs (traceable):** VW 433 MHz AU spec

**Capture tips from database:** "Amarok is the German ute king in Australia. Worth the effort."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `amarok_2h_2018_lock_10m_2fskdev476_2026-05-30.sub`
- `amarok_v6_2020_unlock_12m_2fskdev476_raw.sub`
- `amarok_2015_2fskdev476_9m.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `vw_amarok_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **Amarok 2H (2011-2022)**: VW group pattern — strong FSK preference on keys. OOK will fail you.
- V6 models: Same rules as the rest of the 2H range.
- Treat exactly like later Ford Rangers and Hyundai/Kia FSK examples: start with 2FSKDev476 or raw capture.
- Rolling codes present. Worth the effort if you own one or have access to test on your own.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against an Amarok you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's Amarok or the German ute next door.
- Rolling codes exist specifically to prevent exactly this type of attack. Most modern replays fail fast anyway.
- The repo maintainers and Jesse take **zero responsibility** for you misusing this information and getting charged.
If you're here looking to steal Amaroks: **leave now**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. you will be fine.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["vw_amarok"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations.

Now stop reading and go capture your own Amarok properly at 10 metres. **Start with 2FSKDev476** — OOK is useless here. Multiple presses. RAW first.

— JesseVerified only • Australian 433.92 specialist • No rules except the real ones

