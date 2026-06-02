# Holden Commodore (VF) — JESSE's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No bullshit, no made-up codes." — JESSE**

This folder is for **your own Holden Commodore** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- VF Commodore (2013-2017 AU)
- VF Calais, SS, etc.

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. GM/Opel derived but AU-specific tuning.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: ASK/OOK common on VF

One of the more approachable "modern-ish" cars for Sub-GHz work in Australia. The VF keys are generally straightforward ASK compared to some Korean and German stuff.

---

## Best Starting Preset + Why

**Recommended preset: OOK650**

**Why OOK650 first?**
- VF keys are generally straightforward ASK. Good targets for learning capture technique.
- Clean OOK captures often work well here.
- Excellent vehicle for practising the full workflow before moving to harder FSK-heavy targets like later Rangers or Hyundai/Kia.

**Alternative presets to try if OOK650 gives poor results:**
- 2FSKDev476

**Deviation notes from the verified database:**
"VF keys are generally straightforward ASK. Good targets for learning capture technique."

---

## Real Capture Advice (JESSE Method — Do This or Waste Your Time)

1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation fucks your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* Commodore. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: exact model (Calais/SS/etc), year, distance, preset used, button, conditions.

**FCC refs (traceable):** GM/Opel derived, AU specific 433.92

**Capture tips from database:** "Holden VF is one of the easier modern-ish cars for subghz work in AU. Still rolling code though."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `vf_commodore_2015_lock_8m_ook650_2026-05-30.sub`
- `vf_calais_2016_unlock_10m_ook650_raw.sub`
- `vf_ss_2014_boot_12m_ook650.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `holden_commodore_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **2013-2017 VF platform**: Generally more forgiving ASK implementation than some contemporaries. Excellent for learning.
- Still uses rolling code — the first capture is often useless for reliable replay. You need timing + distance technique or multiple captures.
- Calais and SS variants share the same key fob systems as base VF in most cases.
- One of the better "practice" modern-ish cars before tackling the FSK trap brands.

Rolling codes are present. Simple replay often works zero or one time before the key fob and car desynchronise.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a Commodore you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your mate's VF or the local burnout car.
- Rolling codes exist specifically to stop exactly this shit. Most modern replays fail fast anyway.
- The repo maintainers and JESSE take **zero responsibility** for you being a fuckwit and getting charged.

If you're here looking to steal Holdens: **fuck off**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. She'll be right.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["holden_commodore"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations.

Now stop reading and go capture your own VF Commodore properly at 10 metres. It's one of the easier ones for practice. Multiple presses. RAW first.

— JESSE
Verified only • Australian 433.92 specialist • No rules except the real ones

